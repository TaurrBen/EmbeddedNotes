#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__

#include <stdint.h>
#include <rtthread.h>
#include "main.h"


#define CAN_DATA_LEN_MAX	8


typedef enum
{
	CAN_ENABLE_PWR = 0u,
	CAN_DISABLE_PWR,
}CAN_PwrStateTypedef;


typedef struct
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t buffer[CAN_DATA_LEN_MAX];
}CAN_RxMessageTypedef;


typedef struct
{
	CAN_TxHeaderTypeDef header;
	uint8_t buffer[CAN_DATA_LEN_MAX];
}CAN_TxMessageTypedef;



void CAN_Initialize(void);
void CAN_PwrState(CAN_PwrStateTypedef NewState);


void CAN_SendData(uint32_t IdType,uint32_t StdId,uint32_t ExtId,uint8_t *pData,uint32_t length);

rt_err_t CAN_SendMsgToMq(CAN_TxMessageTypedef *msg);
rt_err_t CAN_RecvMsgFromMq(CAN_RxMessageTypedef *msg, rt_int32_t timeout);


void CAN_SendTest(void);


#endif


