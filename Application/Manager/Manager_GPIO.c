/*
 * @Description:
 * @Author: xiao
 * @Date: 2024-05-06 13:38:52
 * @LastEditTime: 2024-05-21 17:37:07
 * @LastEditors: xiao
 */
#include "Manager.h"

uint32_t count = 0;
extern uint8_t ledstate;
uint16_t led0pwmval = 0;
uint8_t dir = 1;

void Manager_GPIO_Init(void);
void Manager_GPIO_Mainloop(void);

void Manager_GPIO_ledlogie(void);
void Timer_breathe(void);

void Manager_GPIO_Init(void)
{
	// GPIO≥ı ºªØ
	SuperGpio_Init();
	SuperGpio_WriteOutput(0, 0);
	SuperGpio_WriteOutput(1, 0);
	HAL_Delay(500);
	SuperGpio_WriteOutput(0, 1);
	SuperGpio_WriteOutput(1, 1);
}

void Manager_GPIO_Mainloop(void)
{
	Manager_GPIO_ledlogie();
}

uint8_t tickcount = 0;
void Manager_GPIO_ledlogie(void)
{
	uint32_t tickstate = 0;


		if ((HAL_GetTick() - tickstate) % 500 == 0)
		{
			if (count <= 7)
			{
				count++;
			}
			else
			{
				count = 0;
			}

			// ??????
			if (tickcount < 50)
			{
				tickcount++;
			}
			else
			{
				tickcount = 0;
			}
			tickstate = HAL_GetTick();
		}
		
		/*HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_11);*/
		//HAL_Delay(500);
	
}
