#include <stdio.h>
#include <stdbool.h>


#include "bms_energy.h"

#include "bms_hal_control.h"
#include "bms_hal_monitor.h"

#include "bms_monitor.h"
#include "bms_analysis.h"
#include "bms_Protect.h"
#include "bms_global.h"



#define DBG_TAG "energy"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"


// thread config
#define ENERGY_TASK_STACK_SIZE	512
#define ENERGY_TASK_PRIORITY	22
#define ENERGY_TASK_TIMESLICE	25

#define ENERGY_TASK_PERIOD		200




BMS_EnergyDataTypedef BMS_EnergyData = 
{
	.SocStopChg   	= SOC_STOP_CHG_VALUE,
	.SocStartChg  	= SOC_START_CHG_VALUE,
	.SocStopDsg   	= SOC_STOP_DSG_VALUE,
	.SocStartDsg  	= SOC_START_DSG_VALUE,

	.BalanceStartVoltage = INIT_BALANCE_VOLTAGE,
	.BalanceDiffeVoltage = BALANCE_DIFFE_VOLTAGE,
	.BalanceCycleTime 	 = BALANCE_CYCLE_TIME,
	.BalanceRecord 		 = BMS_CELL_NULL,
};



static rt_timer_t pTimerBalance;
static bool BalanceStartFlag = false;
static uint32_t BalanceVoltRiseTime;

static BMS_StateTypedef BMS_CHGStateBackup;
static BMS_StateTypedef BMS_DSGStateBackup;



static void BMS_EnergyTaskEntry(void *paramter);
static void BMS_BalanceTimerEntry(void *paramter);

static void BMS_EnergyChgDsgManage(void);
static void BMS_EnergyBalanceManage(void);






void BMS_EnergyInit(void)
{
	rt_thread_t thread;

	
	thread = rt_thread_create("energy",
							   BMS_EnergyTaskEntry,
							   NULL,
							   ENERGY_TASK_STACK_SIZE,
							   ENERGY_TASK_PRIORITY,
							   ENERGY_TASK_TIMESLICE);

	if (thread == NULL)
	{
		LOG_E("Create Task Fail");
	}

	rt_thread_startup(thread);




	pTimerBalance = rt_timer_create("balance", 
									BMS_BalanceTimerEntry,
									NULL,
									20,
									RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);

	if (pTimerBalance == NULL)
	{
		LOG_E("Create Timer Fail");
	}
}


// 电池能量管理任务线程入口
static void BMS_EnergyTaskEntry(void *paramter)
{
	BMS_CHGStateBackup = BMS_GlobalParam.Charge;
	BMS_DSGStateBackup = BMS_GlobalParam.Discharge;

	rt_thread_mdelay(500);
	
	while(1)
	{
		BMS_EnergyBalanceManage();
		BMS_EnergyChgDsgManage();
		rt_thread_mdelay(ENERGY_TASK_PERIOD);
	}
}









// 充放电管理
static void BMS_EnergyChgDsgManage(void)
{
	switch(BMS_GlobalParam.SysMode)
	{
		case BMS_MODE_CHARGE:
		{
			if (BMS_AnalysisData.SOC >= BMS_EnergyData.SocStopChg)
			{
				BMS_HalCtrlCharge(BMS_STATE_DISABLE);

				LOG_I("Stop Charge");
			}
		}break;

		case BMS_MODE_DISCHARGE:
		{
			if (BMS_AnalysisData.SOC <= BMS_EnergyData.SocStopDsg)
			{
				BMS_HalCtrlDischarge(BMS_STATE_DISABLE);

				LOG_I("Stop Discharge");
			}
		}break;

		case BMS_MODE_STANDBY:
		{
			if (BMS_GlobalParam.Charge == BMS_STATE_ENABLE)	
			{
				if ((BMS_Protect.alert & FLAG_ALERT_CHG_MASK) == FlAG_ALERT_NO)
				{
					if (BMS_EnergyData.BalanceReleaseFlag != true)
					{
						if (BMS_AnalysisData.SOC < BMS_EnergyData.SocStartChg)
						{
							BMS_HalCtrlCharge(BMS_STATE_ENABLE);
							
							LOG_I("Start Charge");
						}
					}
				}
			}


			if (BMS_GlobalParam.Discharge == BMS_STATE_ENABLE) 
			{
				if ((BMS_Protect.alert & FLAG_ALERT_DSG_MASK) == FlAG_ALERT_NO)
				{
					if (BMS_AnalysisData.SOC > BMS_EnergyData.SocStartDsg)
					{
						BMS_HalCtrlDischarge(BMS_STATE_ENABLE);
						
						LOG_I("Start Discharge");
					}
				}
			}
		}break;	
		default:;break;
	}




	// 可通过命令快速关闭充放电
	if (BMS_CHGStateBackup != BMS_GlobalParam.Charge)
	{
		if (BMS_GlobalParam.Charge == BMS_STATE_DISABLE)
		{
			BMS_HalCtrlCharge(BMS_STATE_DISABLE);
		}
		else if (BMS_GlobalParam.SysMode == BMS_MODE_SLEEP)  // 睡眠模式下可开启充电
		{
			BMS_HalCtrlCharge(BMS_STATE_ENABLE);
		}
		BMS_CHGStateBackup = BMS_GlobalParam.Charge;
	}
	if (BMS_DSGStateBackup != BMS_GlobalParam.Discharge)
	{
		if (BMS_GlobalParam.Discharge == BMS_STATE_DISABLE)
		{
			BMS_HalCtrlDischarge(BMS_STATE_DISABLE);
		}
		else if (BMS_GlobalParam.SysMode == BMS_MODE_SLEEP)  // 睡眠模式下可开启放电
		{
			BMS_HalCtrlDischarge(BMS_STATE_ENABLE);
		}
		BMS_DSGStateBackup = BMS_GlobalParam.Discharge;
	}
}













// 用于均衡计数的定时器回调入口
static void BMS_BalanceTimerEntry(void *paramter)
{
	(void)paramter;

	BMS_HalCtrlCellsBalance(BMS_CELL_ALL, BMS_STATE_DISABLE);

	BMS_EnergyData.BalanceRecord = BMS_CELL_NULL;
	
	BalanceStartFlag = false;

	// 用于均衡电压回升计时
	BalanceVoltRiseTime = rt_tick_from_millisecond(BALANCE_VOLT_RISE_DELAY) + rt_tick_get();
	
	LOG_I("Balance Timer End");
}

// 启动均衡定时器计数任务
static void BMS_BalanceStartTimer(uint32_t sec)
{
	uint32_t tick;

	tick = rt_tick_from_millisecond(sec * 1000);
	rt_timer_control(pTimerBalance, RT_TIMER_CTRL_SET_TIME, &tick);
	rt_timer_start(pTimerBalance);

	LOG_I("Balance Timer Start");
}


// 均衡启动条件检查
static bool BMS_EnergyBalanceCheck(void)
{
	// 上一轮均衡时间等待还未结束
	if (BalanceVoltRiseTime >= rt_tick_get())
	{
		return false;
	}

	// 均衡定时器启动
	if (BalanceStartFlag != false)
	{
		return false;
	}

	// 未使能均衡
	if (BMS_GlobalParam.Balance != BMS_STATE_ENABLE)
	{
		BMS_EnergyData.BalanceReleaseFlag = false;
		
		return false;
	}

	// 未处于待机和充电模式
	if (BMS_GlobalParam.SysMode != BMS_MODE_STANDBY && BMS_GlobalParam.SysMode != BMS_MODE_CHARGE)		
	{
		BMS_EnergyData.BalanceReleaseFlag = false;
		
		return false;				
	}
	
	// 最高电池电压小于均衡起始电压
	if (BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number-1].CellVoltage < BMS_EnergyData.BalanceStartVoltage)
	{
		BMS_EnergyData.BalanceReleaseFlag = false;
		
		return false;				
	}

	// 最高和最低电池的电压差未达到均衡条件
	if (BMS_AnalysisData.MaxVoltageDifference < BMS_EnergyData.BalanceDiffeVoltage)
	{	
		BMS_EnergyData.BalanceReleaseFlag = false;
		
		return false;
	}

	BMS_EnergyData.BalanceReleaseFlag = true;

	return true;
}

// 均衡电池筛选
static void BMS_EnergyBalanceFilter(void)
{
	float CmpVoltage;
	float MinVoltage = BMS_MonitorData.CellData[0].CellVoltage;


	/*
	// 相邻单元能同时均衡的情况,BQ不能相邻同时均衡,未测试过
	for(index = 1; index < BMS_GlobalParam.Cell_Real_Number + 1; index++)
	{
		CmpVoltage = BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number-index].CellVoltage;
	
		// 是否达到均衡压差条件
		if (CmpVoltage - MinVoltage > BMS_EnergyData.BalanceDiffeVoltage)
		{
			BMS_EnergyData.BalanceRecord |= 1 << BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number-index].CellNumber;
		}
		else
		{
			break;
		}
	}
	*/
	
	
	
	
	/* 适用于相邻单元不能同时均衡且均衡顺序不按照从大到小进行
	for(index = 0; index < BMS_GlobalParam.Cell_Real_Number; index++)
	{
		if (BMS_MonitorData.CellVoltage[index] - MinVoltage > BMS_EnergyData.BalanceDiffeVoltage)
		{
			BMS_INFO("Balance Cell:%d", index + 1);
			BMS_EnergyData.BalanceRecord |= 1 << index++;
		}
	}
	*/
	
	
	
	
	/* 适用于相邻单元不能同时均衡且均衡顺序按照从大到小进行 */	
	for(uint8_t index = 1; index < BMS_GlobalParam.Cell_Real_Number + 1; index++)
	{
		CmpVoltage = BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number-index].CellVoltage;
	
		if (CmpVoltage - MinVoltage > BMS_EnergyData.BalanceDiffeVoltage)
		{
			bool result = false;
			uint8_t CellNumber = BMS_MonitorData.CellData[BMS_GlobalParam.Cell_Real_Number-index].CellNumber;
	
			if (CellNumber == 0)  
			{
				// 第一节电芯满足均衡压差情况,判断第二节是否添加了均衡标志
				if ((BMS_EnergyData.BalanceRecord & 0x02) == 0)
				{								
					result = true;
				}
			}
			else if (CellNumber + 1 == BMS_GlobalParam.Cell_Real_Number)
			{
				// 最后一节电芯满足均衡压差情况,判断前一节是否添加了均衡标志
				if ((BMS_EnergyData.BalanceRecord & (1 << (CellNumber - 1))) == 0)
				{
					result = true;
				}
			}
			else
			{
				// 其他电芯满足均衡压差情况
				if (((BMS_EnergyData.BalanceRecord & (1 << (CellNumber - 1))) == 0) &&
				   ((BMS_EnergyData.BalanceRecord & (1 << (CellNumber + 1))) == 0))
				{
					result = true;
				}
			}
			
			if (result == true)
			{
				LOG_I("Balance Cell:%d", CellNumber + 1);
				BMS_EnergyData.BalanceRecord |= 1 << CellNumber;
			}
		}
		else 
		{
			break;
		}
	}
	
}

// 均衡启动
static void BMS_EnergyBalanceStart(void)
{
	if (BMS_EnergyData.BalanceRecord != BMS_CELL_NULL)
	{
		// 操作实际硬件
		BMS_HalCtrlCellsBalance(BMS_EnergyData.BalanceRecord, BMS_STATE_ENABLE);
		BMS_BalanceStartTimer(BMS_EnergyData.BalanceCycleTime);
	
		BalanceStartFlag = true;
		
		LOG_I("Balance Start");
	}
}



// 均衡管理
static void BMS_EnergyBalanceManage(void)
{
	if (BMS_EnergyBalanceCheck() == true)
	{
		BMS_EnergyBalanceFilter();
		BMS_EnergyBalanceStart();
	}
}


