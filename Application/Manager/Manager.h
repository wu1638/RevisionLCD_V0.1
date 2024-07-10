/*
 * @Description:
 * @Author: xiao
 * @Date: 2024-05-06 13:38:52
 * @LastEditTime: 2024-05-10 15:00:52
 * @LastEditors: xiao
 */
#ifndef _MANAGER_H
#define _MANAGER_H

#include "string.h"
#include "stdio.h"
#include <ctype.h>

#include "SuperGpio.h"
#include "SuperUsart.h"
#include "SuperTimer.h"
#include "board_ht1621.h"
#include "bsp_IWDG.h"
#include "crc.h"
#include "Variable_Define.h"

extern void Manager_Init(void);
extern void Manager_Activate(void);
extern void Manager_Mainloop(void);

extern void Manager_GPIO_Init(void);
extern void Manager_Usart_Init(void);
extern void Manager_Timer_Init(void);
extern void Manager_LCD_Init(void);
extern void Manager_Sharing_Init(void);

extern void Manager_Usart_Activate(void);
extern void Manager_Timer_Activate(void);
extern void Manager_Sharing_Activate(void);

extern void Manager_GPIO_Mainloop(void);
extern void Manager_Usart_Mainloop(void);
extern void Manager_LCD_Mainloop(void);
extern void Manager_Sharing_Mainloop(void);

#endif
