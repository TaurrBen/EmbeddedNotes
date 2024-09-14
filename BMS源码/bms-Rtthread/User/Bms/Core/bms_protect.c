#include <stdio.h>
#include <stdbool.h>
#include <rtthread.h>

#include "bms_protect.h"

#include "bms_hal_monitor.h"
#include "bms_hal_control.h"
#include "bms_monitor.h"
#include "bms_global.h"


#define DBG_TAG "protect"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"



// thread config
#define PROTECT_TASK_STACK_SIZE	256
#define PROTECT_TASK_PRIORITY	20
#define PROTECT_TASK_TIMESLICE	25

#define PROTECT_TASK_PERIOD		200
 



BMS_ProtectTypedef BMS_Protect = 
{
	.alert = FlAG_ALERT_NO,
	.param = 
	{
		.ShoutdownVoltage = INIT_SHUTDOWN_VOLTAGE,

		.OVProtect	= INIT_OV_PROTECT,
		.OVRelieve	= INIT_OV_RELIEVE,
		.UVProtect	= INIT_UV_PROTECT,
		.UVRelieve	= INIT_UV_RELIEVE,

		.OCCProtect = INIT_OCC_MAX,
		.OCDProtect = INIT_OCD_MAX,

		.OVDelay	= INIT_OV_DELAY,
		.UVDelay	= INIT_UV_DELAY,
		.OCDDelay	= INIT_OCD_DELAY,
		.SCDDelay	= INIT_SCD_DELAY,

		.OCDRelieve = INIT_OCD_RELIEVE,
		.SCDRelieve = INIT_SCD_RELIEVE,
		.OCCDelay	= INIT_OCC_DELAY,
		.OCCRelieve = INIT_OCC_RELIEVE,

		.OTCProtect = INIT_OTC_PROTECT,
		.OTCRelieve = INIT_OTC_RELIEVE,
		.OTDProtect = INIT_OTD_PROTECT,
		.OTDRelieve = INIT_OTD_RELIEVE,

		.LTCProtect = INIT_LTC_PROTECT,
		.LTCRelieve = INIT_LTC_RELIEVE,
		.LTDProtect = INIT_LTD_PROTECT,
		.LTDRelieve = INIT_LTD_RELIEVE,
	}
};



static void BMS_ProtectTaskEntry(void *paramter);


static void BMS_ProtectTiggerMonitor(void);
static void BMS_ProtectRelieveMonitor(void);




// 保护任务的初始化
void BMS_ProtectInit(void)
{
	rt_thread_t thread;

	
	thread = rt_thread_create("protect", 
							   BMS_ProtectTaskEntry, 
							   NULL,
							   PROTECT_TASK_STACK_SIZE,
							   PROTECT_TASK_PRIORITY,
							   PROTECT_TASK_TIMESLICE);

	if (thread == NULL)
	{
		LOG_E("Create Task Fail");
	}

	rt_thread_startup(thread);
}



// 保护任务入口
static void BMS_ProtectTaskEntry(void *paramter)
{
	(void)paramter;
	
	while(1)
	{
		BMS_ProtectTiggerMonitor();
		BMS_ProtectRelieveMonitor();		
		rt_thread_mdelay(PROTECT_TASK_PERIOD);
	}
}









// 充电监控:过流、过温、低温
// 触发三种保护里面的一种就会关闭充电,直到达到恢复条件
static void BMS_ChargeMonitor(void)
{
	static uint32_t ProtectCount = 0;

	if (BMS_MonitorData.BatteryCurrent > BMS_Protect.param.OCCProtect)
	{
		// 过流
		ProtectCount += PROTECT_TASK_PERIOD;
		if (ProtectCount / 60 >= BMS_Protect.param.OCCDelay)
		{			
			BMS_HalCtrlCharge(BMS_STATE_DISABLE);
			BMS_Protect.alert = FlAG_ALERT_OCC;

			LOG_W("Charge:OCC Protect Tigger");
		}
	}
	else if (BMS_MonitorData.CellTempEffectiveNumber == 0)
	{
		// 无效的温度不进行比较
		return;
	}
	else if (BMS_MonitorData.CellTemp[BMS_MonitorData.CellTempEffectiveNumber-1] > BMS_Protect.param.OTCProtect)
	{
		// 过温
		BMS_HalCtrlCharge(BMS_STATE_DISABLE);
		BMS_Protect.alert = FlAG_ALERT_OTC;	
		
		LOG_W("Charge:OTC Protect Tigger");
	}
	else if (BMS_MonitorData.CellTemp[0] < BMS_Protect.param.LTCProtect)
	{
		// 低温
		BMS_HalCtrlCharge(BMS_STATE_DISABLE);
		BMS_Protect.alert = FlAG_ALERT_LTC;	

		LOG_W("Charge:LTC Protect Tigger");
	}
	else
	{
		// 复位计数
		ProtectCount = 0;
	}
}

// 放电监控
static void BMS_DischargeMonitor(void)
{
	if (BMS_MonitorData.CellTempEffectiveNumber == 0)
	{
		// 无无效的温度不进行比较
		return;
	}
	else if (BMS_MonitorData.CellTemp[BMS_MonitorData.CellTempEffectiveNumber-1] > BMS_Protect.param.OTDProtect)
	{
		// 过温
		BMS_HalCtrlDischarge(BMS_STATE_DISABLE);
		BMS_Protect.alert = FlAG_ALERT_OTD;

		LOG_W("Discharge:OTD Protect Tigger");
	}
	else if (BMS_MonitorData.CellTemp[0] < BMS_Protect.param.LTDProtect)
	{
		// 低温
		BMS_HalCtrlDischarge(BMS_STATE_DISABLE);		
		BMS_Protect.alert = FlAG_ALERT_LTD;	
		
		LOG_W("Discharge:LTD Protect Tigger");
	}
}


// 保护触发监控（这是软件触发，有一些保护是由硬件中断触发）
static void BMS_ProtectTiggerMonitor(void)
{
	switch(BMS_GlobalParam.SysMode)
	{
		case BMS_MODE_CHARGE:
		{
			BMS_ChargeMonitor();
		}break;

		case BMS_MODE_DISCHARGE:
		{
			BMS_DischargeMonitor();
		}break;

		case BMS_MODE_STANDBY:
		{
			//BMS_StandbyMonitor();
		}break;

		case BMS_MODE_SLEEP:
		{
			// 睡眠暂时没什么可监控的
		}break;
		
		default:;break;
	}
}


// 保护解除监控
static void BMS_ProtectRelieveMonitor(void)
{
	static uint32_t RelieveCountCHG = 0, RelieveCountDSG = 0;
	
	if (BMS_Protect.alert != FlAG_ALERT_NO)
	{
		if (BMS_Protect.alert & FlAG_ALERT_OV)
		{
			if (BMS_MonitorData.CellData[BMS_CELL_MAX-1].CellVoltage < BMS_Protect.param.OVRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_OV;
				
				LOG_I("Charge:OV Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_OTC)
		{
			if (BMS_MonitorData.CellTemp[BMS_TEMP_MAX-1] < BMS_Protect.param.OTCRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_OTC;
				BMS_HalCtrlCharge(BMS_GlobalParam.Charge);
				
				LOG_I("Charge:OTC Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_LTC)
		{
			if (BMS_MonitorData.CellTemp[0] > BMS_Protect.param.LTCRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_LTC;
				BMS_HalCtrlCharge(BMS_GlobalParam.Charge);
				
				LOG_I("Charge:LTC Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_OCC)
		{
			RelieveCountCHG += PROTECT_TASK_PERIOD;
			if (RelieveCountCHG / 60 >= BMS_Protect.param.OCCRelieve)
			{
				RelieveCountCHG = 0;

				BMS_Protect.alert &= ~FlAG_ALERT_OCC;
				BMS_HalCtrlCharge(BMS_GlobalParam.Charge);
				
				LOG_I("Charge:OCC Relieve");
			}
		}



		

		if (BMS_Protect.alert & FlAG_ALERT_UV)
		{
			if (BMS_MonitorData.CellData[0].CellVoltage > BMS_Protect.param.UVRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_UV;
				
				LOG_I("Discharge:UV Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_OTD)
		{
			if (BMS_MonitorData.CellTemp[BMS_TEMP_MAX-1] < BMS_Protect.param.OTDRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_OTD;
				BMS_HalCtrlDischarge(BMS_GlobalParam.Discharge);
				
				LOG_I("Discharge:OTD Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_LTD)
		{
			if (BMS_MonitorData.CellTemp[0] > BMS_Protect.param.LTDRelieve)
			{
				BMS_Protect.alert &= ~FlAG_ALERT_LTD;
				BMS_HalCtrlDischarge(BMS_GlobalParam.Discharge);
				
				LOG_I("Discharge:LTD Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_OCD)
		{
			RelieveCountDSG += PROTECT_TASK_PERIOD;
			if (RelieveCountDSG / 60 >= BMS_Protect.param.OCDRelieve)
			{
				RelieveCountDSG = 0;

				BMS_Protect.alert &= ~FlAG_ALERT_OCD;
				BMS_HalCtrlDischarge(BMS_GlobalParam.Discharge);

				LOG_I("Discharge:OCD Relieve");
			}
		}
		else if (BMS_Protect.alert & FlAG_ALERT_SCD)
		{
			RelieveCountDSG += PROTECT_TASK_PERIOD;
			if (RelieveCountDSG / 60 >= BMS_Protect.param.SCDRelieve)
			{
				RelieveCountDSG = 0;

				BMS_Protect.alert &= ~FlAG_ALERT_SCD;		
				BMS_HalCtrlDischarge(BMS_GlobalParam.Discharge);

				LOG_I("Discharge:SCD Relieve");
			}
		}
	}
}













// 放电过流(OCD)硬件触发
void BMS_ProtectHwOCD(void)
{
	if ((BMS_Protect.alert & FlAG_ALERT_OCD) == FlAG_ALERT_NO)
	{
		BMS_HalCtrlDischarge(BMS_STATE_DISABLE);
		BMS_Protect.alert |= FlAG_ALERT_OCD;
		LOG_W("Discharge:OCD Protect Tigger");
	}
}

// 放电短路(SCD)硬件触发
void BMS_ProtectHwSCD(void)
{
	if ((BMS_Protect.alert & FlAG_ALERT_SCD) == FlAG_ALERT_NO)
	{
		BMS_HalCtrlDischarge(BMS_STATE_DISABLE);
		BMS_Protect.alert |= FlAG_ALERT_SCD;
		LOG_W("Discharge:SCD Protect Tigger");
	}
}

// 充电过压(OV)硬件触发
void BMS_ProtectHwOV(void)
{
	if ((BMS_Protect.alert & FlAG_ALERT_OV) == FlAG_ALERT_NO)
	{
		BMS_HalCtrlCharge(BMS_STATE_DISABLE);
		BMS_Protect.alert |= FlAG_ALERT_OV;
		LOG_W("Charge:OV Protect Tigger");
	}
}

// 放欠过压(UV)硬件触发
void BMS_ProtectHwUV(void)
{
	if ((BMS_Protect.alert & FlAG_ALERT_UV) == FlAG_ALERT_NO)
	{
		BMS_HalCtrlDischarge(BMS_STATE_DISABLE);
		BMS_Protect.alert |= FlAG_ALERT_UV;
		LOG_W("Discharge:UV Protect Tigger");
	}
}






void BMS_ProtectHwDevice(void)
{
	LOG_W("BMS_ProtectHwDevice");
}


void BMS_ProtectHwOvrd(void)
{
	LOG_W("BMS_ProtectHwOvrd");
}



