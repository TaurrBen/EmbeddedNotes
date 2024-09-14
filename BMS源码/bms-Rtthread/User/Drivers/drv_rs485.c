#include "drv_rs485.h"

#include "main.h"

#include "usart.h"

#include <rtthread.h>



#define DBG_TAG "drv_rs485"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"



uint8_t rc;

void RS485_Initialize(void)
{
	RS485_PwrState(RS485_ENABLE_PWR);



	RS485_TransmitState(RS485_RX);
	HAL_UART_Receive_IT(&huart2, &rc, 1);
}

void RS485_PwrState(RS485_PwrStateTypedef NewState)
{
	HAL_GPIO_WritePin(RS485_PWREN_GPIO_Port, RS485_PWREN_Pin, (GPIO_PinState)NewState);
}


void RS485_TransmitState(RS485_TransmitStateTypedef NewState)
{
	HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, (GPIO_PinState)NewState);
}







void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		LOG_D("rs485 rx done");
		RS485_TransmitState(RS485_TX);
		HAL_UART_Transmit_IT(&huart2, &rc, 1);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		LOG_D("rs485 tx done");
		RS485_TransmitState(RS485_RX);
		HAL_UART_Receive_IT(&huart2, &rc, 1);
	}
}



