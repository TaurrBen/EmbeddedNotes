#include <rtthread.h>
#include <stdio.h>

#include "bms_hal_control.h"
#include "bms_hal_monitor.h"

#include "bms_monitor.h"
#include "bms_energy.h"
#include "bms_info.h"
#include "bms_global.h"



#define DBG_TAG "cmd"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"



/***************************************** 控制类 *********************************/

/* 睡眠唤醒由命令控制不合理，应该由系统控制
static void BMS_CmdWakeup(void)
{
	BMS_HalCtrlWakeup();
}
MSH_CMD_EXPORT(BMS_CmdWakeup, wakeup);


static void BMS_CmdSleep(void)
{
	BMS_HalCtrlSleep();
}
MSH_CMD_EXPORT(BMS_CmdSleep, sleep);
*/


static void BMS_CmdOpenDSG(void)
{
	BMS_GlobalParam.Discharge = BMS_STATE_ENABLE;
}
MSH_CMD_EXPORT(BMS_CmdOpenDSG, Open DSG);



static void BMS_CmdCloseDSG(void)
{
	BMS_GlobalParam.Discharge = BMS_STATE_DISABLE;
}
MSH_CMD_EXPORT(BMS_CmdCloseDSG, Close DSG);



static void BMS_CmdOpenCHG(void)
{
	BMS_GlobalParam.Charge = BMS_STATE_ENABLE;
}
MSH_CMD_EXPORT(BMS_CmdOpenCHG, Open CHG);



static void BMS_CmdCloseCHG(void)
{
	BMS_GlobalParam.Charge = BMS_STATE_DISABLE;
}
MSH_CMD_EXPORT(BMS_CmdCloseCHG, Close CHG);





static void BMS_CmdOpenBalance(void)
{
	BMS_GlobalParam.Balance = BMS_STATE_ENABLE;
}
MSH_CMD_EXPORT(BMS_CmdOpenBalance, Open Balance);



static void BMS_CmdCloseBalance(void)
{
	BMS_GlobalParam.Balance = BMS_STATE_DISABLE;
}
MSH_CMD_EXPORT(BMS_CmdCloseBalance, Close Balance);





static void BMS_CmdLoadDetect(void)
{
	if (Bms_HalMonitorLoadDetect() == true)
	{
		LOG_I("Load Detected");
	}
	else
	{		
		LOG_I("No Load Was Detected");
	}
}
MSH_CMD_EXPORT(BMS_CmdLoadDetect, Load Detect);





static void BMS_CmdOpenInfo(void)
{
	BMS_InfoSetPrintf(BMS_STATE_ENABLE);
}
MSH_CMD_EXPORT(BMS_CmdOpenInfo, Open Info Printf);


static void BMS_CmdCloseInfo(void)
{
	BMS_InfoSetPrintf(BMS_STATE_DISABLE);
}
MSH_CMD_EXPORT(BMS_CmdCloseInfo, Close Info Printf);
/**********************************************************************************/










/***************************************** 传感数据 *********************************/

/**********************************************************************************/





