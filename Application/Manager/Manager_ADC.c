#include "Manager.h"
#include <stdio.h>

float collcetdata[13] = {0};
uint8_t adcx; 

void Manager_ADC_Init(void);
void Manager_ADC_Activate(void);
void Manager_ADC_Mainloop(void);

void Manager_ADC_Init(void)
{
    BSP_ADC_Init();
}
void Manager_ADC_Activate(void)
{
}
void Manager_ADC_Mainloop(void)
{
    adcx = BSP_Lsens_Get_Val();             //采集光源亮度
    collcetdata[11] = BSP_Get_Temprate(11); //采集温度值
    for (uint8_t i = 0; i < 11; i++)
    {
        collcetdata[i] = BSP_Get_voltage(i); //采集电压值
    }

    printf("\r\n光的亮度为:%d\r\n", adcx);
    printf("\r\nMUC温度为:%0.2f ℃\t\n", collcetdata[11]);
    printf("\r\nPA0=%0.2fV\tPA1=%0.2fV\tPA2=%0.2fV\tPB1=%0.2fV\tPB2=%0.2fV\r\n", collcetdata[0], collcetdata[1], collcetdata[2], collcetdata[3], collcetdata[4]);
    printf("\r\nPc0=%0.2fV\tPc1=%0.2fV\tPc2=%0.2fV\tPc3=%0.2fV\tPc4=%0.2fV\r\n", collcetdata[5], collcetdata[6], collcetdata[7], collcetdata[8], collcetdata[9]);
    printf("\r\nPc5=%0.2fV\r\n", collcetdata[10]);
    printf("\r\n");
}

