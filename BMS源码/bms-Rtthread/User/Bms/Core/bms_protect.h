#ifndef __BMS_PROTECT_H__
#define __BMS_PROTECT_H__



#include "bms_config.h"




// 跟保护相关的参数结构体
typedef struct
{
	float ShoutdownVoltage;	// 关机电压(V)
	
	float OVProtect;		// 充电过压保护电压(V)					由硬件完成
	float OVRelieve;		// 充电过压恢复电压(V)
	float UVProtect;		// 放电欠压保护电压(V)					由硬件完成
	float UVRelieve;		// 放电欠压恢复电压(V)
	
	float OCCProtect;				// 充电过流阈值(A)
	float OCDProtect;				// 放电过流阈值(A)			由硬件完成
	
	BMS_OVDelayTypedef OVDelay;		// 充电过压保护延时				由硬件完成
	BMS_UVDelayTypedef UVDelay;		// 放电欠压保护延时				由硬件完成
	BMS_OCDDelayTypedef OCDDelay;	// 放电过流保护延时				由硬件完成
	BMS_SCDDelayTypedef SCDDelay;	// 放电短路保护延时				由硬件完成
	
	uint8_t OCDRelieve;		// 放电过流恢复(S)
	uint8_t SCDRelieve;		// 放电短路恢复(S)
	
	uint8_t OCCDelay;		// 充电过流延时(S)
	uint8_t OCCRelieve;		// 充电过流恢复(S)
	
	float OTCProtect;		// 充电过温保护(℃)
	float OTCRelieve;		// 充电过温解除(℃)
	float OTDProtect;		// 放电过温保护(℃)
	float OTDRelieve;		// 放电过温解除(℃)
	
	float LTCProtect;		// 充电低温保护(℃)
	float LTCRelieve;		// 充电低温解除(℃)
	float LTDProtect;		// 放电低温解除(℃)
	float LTDRelieve;		// 放电低温解除(℃)
}BMS_ProtectParamTypedef;




// 报警枚举体
typedef enum
{
	FlAG_ALERT_NO	= 0x0000,		// 无报警触发
	
	FlAG_ALERT_OV	= 0X0001,		// 充电过压保护触发位				硬件触发
	FlAG_ALERT_OCC	= 0X0002,		// 充电过流保护触发位				软件触发
	FlAG_ALERT_OTC	= 0X0004,		// 充电过温保护触发位				软件触发
	FlAG_ALERT_LTC	= 0X0008,		// 充电低温保护触发位				软件触发

	FLAG_ALERT_CHG_MASK = 0x000F,	// 充电报警掩码
	
	FlAG_ALERT_UV	= 0X0010,		// 放电欠压保护触发位				硬件触发
	FlAG_ALERT_OCD	= 0X0020,		// 放电过流保护触发位				硬件触发
	FlAG_ALERT_SCD	= 0X0040,		// 放电短路保护触发位				硬件触发
	FlAG_ALERT_OTD	= 0X0080,		// 放电过温保护触发位				软件触发
	FlAG_ALERT_LTD	= 0X0100,		// 放电低温保护触发位				软件触发

	FLAG_ALERT_DSG_MASK = 0x01F0,	// 放电报警掩码
}BMS_ProtectAlertTypedef;




// 跟保护相关的参数结构体
typedef struct
{
	BMS_ProtectParamTypedef param;
	BMS_ProtectAlertTypedef alert;
}BMS_ProtectTypedef;


extern BMS_ProtectTypedef BMS_Protect;



void BMS_ProtectInit(void);

void BMS_ProtectHwOCD(void);
void BMS_ProtectHwSCD(void);
void BMS_ProtectHwOV(void);
void BMS_ProtectHwUV(void);


void BMS_ProtectHwDevice(void);
void BMS_ProtectHwOvrd(void);



#endif


