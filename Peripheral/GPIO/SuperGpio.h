/*
 * @Description: 
 * @Author: xiao
 * @Date: 2024-05-06 13:38:52
 * @LastEditTime: 2024-05-06 14:37:34
 * @LastEditors: xiao
 */
#ifndef _SUPERGPIO_H
#define _SUPERGPIO_H

#include "main.h"

typedef struct 
{
	uint8_t Enable;
	GPIO_TypeDef* GpioX;
	uint32_t Pin;
	uint32_t Pull;
	GPIO_PinState DefaultState;
}SuperGpio_InitTypeDef;

static SuperGpio_InitTypeDef SuperGpio_outputs[]=
{
	{1,GPIOA,GPIO_PIN_11,GPIO_NOPULL,GPIO_PIN_RESET},//0
	{1,GPIOA,GPIO_PIN_12,GPIO_NOPULL,GPIO_PIN_RESET},//1
};

static SuperGpio_InitTypeDef superGpio_inputs[]=
{
	{1,GPIOB,GPIO_PIN_0,GPIO_NOPULL,GPIO_PIN_SET},//0
};

//函数原型外部引用
extern void SuperGpio_Init(void);
extern uint8_t SuperGpio_ReadInput(uint8_t id);
extern void SuperGpio_WriteOutput(uint8_t id,uint8_t state);

#endif

