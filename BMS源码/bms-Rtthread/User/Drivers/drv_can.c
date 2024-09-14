#include <string.h>

#include "drv_can.h"

#include "can.h"


#define DBG_TAG "drv_can"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"



static rt_mq_t CanRxMq, CanTxMq;



static void CAN_SendTaskEntry(void *paramter);
static void CAN_OSResourceInit(void);
static void CAN_LLInit(void);



void CAN_Initialize(void)
{
	CAN_LLInit();
	CAN_OSResourceInit();
}



static void CAN_OSResourceInit(void)
{
	rt_thread_t thread;

	CanRxMq = rt_mq_create("can_rx", sizeof(CAN_RxMessageTypedef), 3, RT_IPC_FLAG_PRIO);
	if (CanRxMq == RT_NULL)
	{
		LOG_E("CAN mq create Fail!");
	}

	CanTxMq = rt_mq_create("can_tx", sizeof(CAN_TxMessageTypedef), 16, RT_IPC_FLAG_PRIO);
	if (CanTxMq == RT_NULL)
	{
		LOG_E("CAN mq create Fail!");
	}
	
	
	thread = rt_thread_create("can_send",
							   CAN_SendTaskEntry,
							   NULL,
							   512,
							   23,
							   20);

	if (thread == NULL)
	{
		LOG_E("Create Task Fail");
	}

	rt_thread_startup(thread);	
}


static void CAN_LLInit(void)
{
	CAN_FilterTypeDef filter = {0};

	CAN_PwrState(CAN_ENABLE_PWR);
	
    filter.FilterActivation = ENABLE;				// 是否使能筛选器
    filter.FilterMode = CAN_FILTERMODE_IDMASK;		// 筛选器模式
    filter.FilterScale = CAN_FILTERSCALE_32BIT;		// 筛选器的位数
    filter.FilterBank = 0;							// 筛选器编号
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;	// 设置经过筛选后数据存储到哪个接收 FIFO
    filter.FilterIdLow = 0;			// CAN_FxR1 寄存器的低 16 位
    filter.FilterIdHigh = 0;		// CAN_FxR1 寄存器的高 16 位
    filter.FilterMaskIdLow = 0;		// CAN_FxR2 寄存器的低 16 位
	filter.FilterMaskIdHigh = 0;	// CAN_FxR2 寄存器的高 16 位

	// 配置滤波器
	if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)
	{
		LOG_E("CAN Config Filter Fail!");
	}

	// 打开CAN RX0、TX  中断
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);  

	// 开启CAN
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		LOG_E("CAN Start Fail!");
	}
}




void CAN_PwrState(CAN_PwrStateTypedef NewState)
{
	HAL_GPIO_WritePin(CAN_PWREN_GPIO_Port, CAN_PWREN_Pin, (GPIO_PinState)NewState);
}




// 立即发送
void CAN_SendData(uint32_t IdType,uint32_t StdId,uint32_t ExtId,uint8_t *pData,uint32_t length)
{
    uint32_t TxMailbox;
	uint8_t buffer[8] = {0};
	
    CAN_TxHeaderTypeDef TxHeader;
    
    TxHeader.StdId = StdId;			// 标准ID
    TxHeader.ExtId = ExtId;			// 扩展ID
    TxHeader.IDE = IdType;			// ID类型
    TxHeader.RTR = CAN_RTR_DATA;	// 数据帧/远程帧
    TxHeader.DLC = length;			// 数据长度
	TxHeader.TransmitGlobalTime = DISABLE;
	
	memcpy(buffer, pData, length);
 
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, buffer, &TxMailbox) != HAL_OK)
	{
		LOG_E("CAN Send Data Fail, can errcode:%08x", hcan.ErrorCode);
	}
}




// 将需要传输的CAN消息发送至消息队列
rt_err_t CAN_SendMsgToMq(CAN_TxMessageTypedef *msg)
{
	rt_err_t err = RT_EOK;
	
	if ((err = rt_mq_send(CanTxMq, msg, sizeof(CAN_TxMessageTypedef))) != RT_EOK)
	{
		LOG_W("can send msg to mq fail, mailbox full");
	}

	return err;
}


// 从消息队列里面读取一个CAN消息
rt_err_t CAN_RecvMsgFromMq(CAN_RxMessageTypedef *msg, rt_int32_t timeout)
{
	rt_err_t err = RT_EOK;
	
	if ((err = rt_mq_recv(CanRxMq, msg, sizeof(CAN_RxMessageTypedef), timeout)) != RT_EOK)
	{
		LOG_W("can recv msg fail");
	}

	return err;
}


// 处理发送消息队列的消息
static void CAN_SendTaskEntry(void *paramter)
{
	uint32_t TxMailbox;
	CAN_TxMessageTypedef msg;

	while(1)
	{
		// 永远阻塞直至获取到消息
		rt_mq_recv(CanTxMq, &msg, sizeof(CAN_TxMessageTypedef), RT_WAITING_FOREVER);
	 
		if (HAL_CAN_AddTxMessage(&hcan, &msg.header, msg.buffer, &TxMailbox) != HAL_OK)
		{
			// 延时五个毫秒等待一个空的邮箱
			rt_thread_mdelay(5);
			
			// 将上一次未发送的消息重新发出
			if (HAL_CAN_AddTxMessage(&hcan, &msg.header, msg.buffer, &TxMailbox) != HAL_OK)
			{
				//LOG_E("retry send fail,errcode = %d", hcan.ErrorCode);
			}
		}
	}
}



// 中断处理接收消息
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t buffer[CAN_DATA_LEN_MAX];
	CAN_RxHeaderTypeDef hCAN1_RxHeader;
	CAN_RxMessageTypedef RxMessage;

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hCAN1_RxHeader, buffer) != HAL_OK)
    {
		LOG_E("can read message error");

		return;
    }

	RxMessage.RxHeader = hCAN1_RxHeader;
	memcpy(RxMessage.buffer, buffer, sizeof(buffer));

	if (rt_mq_send(CanRxMq, &RxMessage, sizeof(CAN_RxMessageTypedef)) != RT_EOK)
	{
		LOG_E("can send mq error");
	}
}



















void CAN_SendTest(void)
{
	static uint8_t send_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	static uint8_t id_type_std = 0;


	send_data[0]++;
	send_data[1]++;
	send_data[2]++;
	send_data[3]++;
	send_data[4]++;
	send_data[5]++;
	send_data[6]++;
	send_data[7]++;
	
	if(id_type_std == 1)
	{
		CAN_SendData(CAN_ID_STD, 0x123, 0x321, send_data, 8);
		id_type_std = 0;
	}
	else
	{
		CAN_SendData(CAN_ID_EXT, 0x123, 0x321, send_data, 8);
		id_type_std = 1;
	}
}


/*
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t buffer[8], i;
	CAN_RxHeaderTypeDef hCAN1_RxHeader;

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hCAN1_RxHeader, buffer) == HAL_OK)
    {
        rt_kprintf("\nGet Rx Message Success!!\r\nData:");


        for(i=0; i<8; i++)
        {
            rt_kprintf("%d", buffer[i]);
       	}
    }
}

*/






