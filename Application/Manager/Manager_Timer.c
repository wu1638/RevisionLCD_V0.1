/*
 * @Description:
 * @Author: xiao
 * @Date: 2024-05-06 13:38:52
 * @LastEditTime: 2025-09-05 16:05:51
 * @LastEditors: xiao
 */
#include "Manager.h"
/*私有变量定义*/
uint16_t timecount = 0, Heartbeat = 0, FlickerFlg = 0, FlickerTime = 0, Cir_FlickerFlag = 0, Cir_FlickerTime = 0,FaultDiaTime = 0,PollFlag = 0;
uint16_t SendtimeCount = 0, SendFag = 0, ErrorFlickerFlag = 0, Sleep_Timing = 0, PollTime = 0, DormancyTime = 0, DormancySta = 0,
FaultBrightTime = 0,FaultDestroyTime = 0;
uint8_t ledstate = 0,FaultCount = 0,FaultFlickerFlag = 0,FaultDiaTimeFlag = 0;
/*外部引用*/
extern uint8_t RestartTime;
/*私有函数原型*/
void Manager_Timer_Init(void);
void Manager_Timer_Activate(void);

void Timer_100us_CallBack(void);
void Timer_free_Callback(void);
void Timer_Send_Callback(void);

void Manager_Timer_Init(void)
{
	BSP_Timer_Init();
}
void Manager_Timer_Activate(void)
{

	BSP_Timer_SetCallback_Hearbeat(Timer_free_Callback);
	BSP_Timer_SendCallBack(Timer_Send_Callback);
	BSP_Timer_FlickerCallBack(Timer_100us_CallBack);
}

void Timer_100us_CallBack(void)
{

	// HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);

	FlickerTime++;
	if (FlickerTime == 1500)
	{
		FlickerFlg = TRUE;
		FlickerTime = FALSE;
	}

	Cir_FlickerTime++;
	if (Cir_FlickerTime == 200)
	{
		Cir_FlickerFlag = TRUE;
		Cir_FlickerTime = FALSE;
	}
	
}

void Timer_free_Callback(void)
{
	if (RestartTime)
	{
		timecount++;
		if (timecount == 1000)
		{
			if (++DormancyTime == 1800)
			{
				DormancySta = TRUE;
				RestartTime = FALSE;
				DormancyTime = FALSE;
			}
			timecount = FALSE;
		}
	}
	else
	{
		timecount++;
		if (timecount == 1000)
		{
			if (Heartbeat <= 1000)
			{
				Heartbeat++;
			}
			else
			{
				Heartbeat = FALSE;
			}

			timecount = FALSE;
		}
	}
}

void Timer_Send_Callback(void)
{
	SendtimeCount++;
	PollTime++;
	if (SendtimeCount == 500)
	{
		ErrorFlickerFlag = TRUE;
		SendFag = TRUE;
		SendtimeCount = FALSE;
	}

	if (FaultFlickerFlag)
	{
		FaultBrightTime++;
		if (FaultBrightTime == 500)
		{
			FaultFlickerFlag = FALSE;
			FaultBrightTime = FALSE;
		}
	}
	else
	{
		FaultDestroyTime++;
		if (FaultDestroyTime == 500)
		{
			FaultFlickerFlag = TRUE;
			FaultDestroyTime = FALSE;
		}
		
	}
	
if (FaultDiaTimeFlag)
{
	FaultDiaTime++;
	if (FaultDiaTime == 1500)
	{
		FaultCount++;
		FaultDiaTime = FALSE;
	}
	if (FaultCount == 14)
	{
		FaultCount = TRUE;
	}
}

	
}
