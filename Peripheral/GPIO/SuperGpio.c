#include "SuperGpio.h"

/*
		GPIO初始化
*/

void SuperGpio_Init(void);
uint8_t SuperGpio_ReadInput(uint8_t id);
void SuperGpio_WriteOutput(uint8_t id, uint8_t state);

void sg_Gpio_CLK_ENABLE(GPIO_TypeDef *gpio);
void sg_Output_Init(void);
void sg_Input_Init(void);

// GPIO初始化
void SuperGpio_Init(void)
{
	sg_Input_Init();
	sg_Output_Init();
	// GPIO_InitTypeDef GPIO_InitStruct = {0};

	// /* GPIO Ports Clock Enable */
	// __HAL_RCC_GPIOE_CLK_ENABLE();
	// __HAL_RCC_GPIOH_CLK_ENABLE();
	// __HAL_RCC_GPIOA_CLK_ENABLE();
	// __HAL_RCC_GPIOB_CLK_ENABLE();

	// /*Configure GPIO pin Output Level */
	// HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);

	// /*Configure GPIO pins : PE5 PE6 */
	// GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Pull = GPIO_NOPULL;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	// HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

uint8_t SuperGpio_ReadInput(uint8_t id)
{
	uint8_t count = 0;

	count = sizeof(superGpio_inputs) / sizeof(SuperGpio_InitTypeDef);
	if (id < count)
	{
		SuperGpio_InitTypeDef *gpio = &superGpio_inputs[id];
		if (gpio->Enable)
		{
			if (gpio->DefaultState == GPIO_PIN_RESET)
			{
				return HAL_GPIO_ReadPin(gpio->GpioX, gpio->Pin);
			}
			else
			{
				return !HAL_GPIO_ReadPin(gpio->GpioX, gpio->Pin);
			}
		}
	}
	return 0;
}

void SuperGpio_WriteOutput(uint8_t id, uint8_t state)
{
	uint8_t count = 0;
	count = sizeof(SuperGpio_outputs) / sizeof(SuperGpio_InitTypeDef);

	if (id < count)
	{
		SuperGpio_InitTypeDef *gpio = &SuperGpio_outputs[id];
		if (gpio->Enable)
		{
			if (gpio->DefaultState == GPIO_PIN_RESET)
			{
				if (state == 0)
				{
					HAL_GPIO_WritePin(gpio->GpioX, gpio->Pin, GPIO_PIN_RESET);
				}
				else
				{
					HAL_GPIO_WritePin(gpio->GpioX, gpio->Pin, GPIO_PIN_SET);
				}
			}
			else
			{
				if (state == 0)
				{
					HAL_GPIO_WritePin(gpio->GpioX, gpio->Pin, GPIO_PIN_SET);
				}
				else
				{
					HAL_GPIO_WritePin(gpio->GpioX, gpio->Pin, GPIO_PIN_RESET);
				}
			}
		}
	}
}

void sg_Gpio_CLK_ENABLE(GPIO_TypeDef *gpio)
{
	if (gpio == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (gpio == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (gpio == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
}

void sg_Input_Init(void)
{
	uint8_t i, count = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // 输入模式
	count = sizeof(superGpio_inputs) / sizeof(SuperGpio_InitTypeDef);
	for (i = 0; i < count; i++)
	{
		SuperGpio_InitTypeDef *gpio = &superGpio_inputs[i];
		if (gpio->Enable == 0)
			continue;

		sg_Gpio_CLK_ENABLE(gpio->GpioX); // GPIO使能

		GPIO_InitStruct.Pin = gpio->Pin;			  // pin
		GPIO_InitStruct.Pull = gpio->Pull;			  // pull
		HAL_GPIO_Init(gpio->GpioX, &GPIO_InitStruct); // GPIO初始化
	}
}
void sg_Output_Init(void)
{
	uint8_t i, count = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	  // 复用输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
	count = sizeof(SuperGpio_outputs) / sizeof(SuperGpio_InitTypeDef);

	for (i = 0; i < count; i++)
	{
		SuperGpio_InitTypeDef *gpio = &SuperGpio_outputs[i];
		if (gpio->Enable == 0)
			continue;

		sg_Gpio_CLK_ENABLE(gpio->GpioX); // GPIO使能

		GPIO_InitStruct.Pin = gpio->Pin;			  // pin
		GPIO_InitStruct.Pull = gpio->Pull;			  // pull
		HAL_GPIO_Init(gpio->GpioX, &GPIO_InitStruct); // GPIO初始化

		HAL_GPIO_WritePin(gpio->GpioX, gpio->Pin, gpio->DefaultState); // GPIO写入状态
	}
}
