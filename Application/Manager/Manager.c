/*
 * @Description: 
 * @Author: xiao
 * @Date: 2024-05-14 14:06:31
 * @LastEditTime: 2025-08-26 16:53:25
 * @LastEditors: xiao
 */
#include "Manager.h"
#include "stdio.h"
#include "string.h"

void Manager_Init(void);
void Manager_Activate(void);
void Manager_Mainloop(void);

//
void Manager_Init(void)
{
	HAL_Delay(100);
	Manager_Usart_Init();
	Manager_GPIO_Init();
	Manager_Timer_Init();
	Manager_LCD_Init();
	Manager_Sharing_Init();
	
	printf("初始化完成********************\n\r");
}

//激活函数
void Manager_Activate(void)
{
	Manager_Timer_Activate();
	Manager_Usart_Activate();
	Manager_Sharing_Activate();
}

//主函数调用
void Manager_Mainloop(void)
{
	Manager_GPIO_Mainloop();
	Manager_Usart_Mainloop();
	Manager_LCD_Mainloop();
	Manager_Sharing_Mainloop();
}
