/*
 * @Description:
 * @Author: xiao
 * @Date: 2022-10-10 11:50:30
 * @LastEditTime: 2024-05-21 16:25:19
 * @LastEditors: xiao
 */
#ifndef __SUPERTIMER_H__
#define __SUPERTIMER_H__

#include "main.h"


#define TIMER14_COUNT_CLK_FEQ 32 - 1 // 设置分频值
#define TIMER14_PERIOD_US 1000 - 1      // 计数周期

#define TIMER16_COUNT_CLK_FEQ 64 - 1 // 设置分频值
#define TIMER16_PERIOD_US 1000 - 1   // 计数周期

#define TIMER17_COUNT_CLK_FEQ 64 - 1 // 设置分频值
#define TIMER17_PERIOD_US 1000 - 1      // 计数周期

typedef enum
{
    Tim0 = 0,
    Tim14,
    Tim16,
    Tim17,
} TIM_COM_ID;

typedef void (*TimerCallBack)(void);

extern void BSP_Timer_Init(void);                                   // 定时器初始化
extern void BSP_Timer_SetCallback_Hearbeat(TimerCallBack callBack); // 定时器14
extern void BSP_Timer_SendCallBack(TimerCallBack callBack);         // 定时器16
extern void BSP_Timer_FlickerCallBack(TimerCallBack callBack);      // 定时器17

#endif
