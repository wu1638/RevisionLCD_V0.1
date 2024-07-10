/******************************************************************************
 * 文件名程: Mannger_Sharing.c
 * 作    者: xiao
 * 版    本: V1.0
 * 编写日期: 2022-10-14
 * 功    能: 共用函数接口管理
 ******************************************************************************
 */

#include "Manager.h"
#include <stdlib.h>

/*外部引用变量*/

/*私有宏定义--------------------------------------------------------------------*/
#define N 12

/*私有变量定义----------------------------------------------------------------------*/
Deal_Buf1_t Deal_Buf1;
Deal_Buf2_t Deal_Buf2;
Deal_Buf3_t Deal_Buf3;

uint16_t Value_buf[N];
uint8_t num = 0;
uint16_t Temp1 = 0;

void Manager_Sharing_Init(void);
void Manager_Sharing_Activate(void);
void Manager_Sharing_Mainloop(void);

// void ReadRegistertomappingBuf(uint16_t *ReadData, uint16_t addr, uint8_t lent);
// void Mapp_WriteToRegister(uint16_t *WriteData, uint16_t addr, uint16_t lent);
void Deal_WriteDataDispose(uint8_t *ReadData, uint16_t addr, uint8_t lent);
void Deal_ReadDataDispose1(uint8_t *WriteData, uint16_t addr, uint8_t lent);
void Deal_ReadDataDispose2(uint8_t *WriteData, uint16_t addr, uint8_t lent);
void Deal_ReadDataDispose3(uint8_t *WriteData, uint16_t addr, uint8_t lent);

void Manager_Sharing_Init(void)
{
    IWDG_Init(IWDG_PRESCALER_64, 4095); // 看门狗初始化
}

void Manager_Sharing_Activate(void)
{
}

void Manager_Sharing_Mainloop(void)
{
    FeedDog();

    // ReadRegistertomappingBuf(usSRegHoldBuf, 20, 23); // 读取寄存器到使用定义buf
    // Mapp_WriteToRegister(usSRegHoldBuf, 0, 20);      // 使用定义buf写入到寄存器
    //*(DealSendData1 + 2) = (uint8_t)*(Deal_Buf1.Heartcount);
    //*(DealSendData1 + 3) = (uint8_t)*(Deal_Buf1.Heartcount + 1);
}

/*从映射地址写入到寄存器地址*/
void Deal_WriteDataDispose(uint8_t *ReadData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(ReadData + i + 2) = (uint8_t) * (Deal_Buf1.Heartcount + addr + i);
        i++;
    }
}
/*协议1从寄存器读取到映射地址*/
void Deal_ReadDataDispose1(uint8_t *WriteData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(Deal_Buf1.Heartcount + addr + i) = *(WriteData + i + 3);
        i++;
    }
}
/*协议2从寄存器读取到映射地址*/
void Deal_ReadDataDispose2(uint8_t *WriteData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(Deal_Buf2.Total_Voltage + addr + i) = *(WriteData + i + 4);
        i++;
    }
}
/*协议3从寄存器读取到映射地址*/
void Deal_ReadDataDispose3(uint8_t *WriteData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(&(Deal_Buf3.System_Sta) + addr + i) = (uint16_t)*(WriteData + i + 3);
        *(&(Deal_Buf3.System_Sta) + addr + i) = (uint16_t)((*(WriteData + i*2 + 3) << 8) | (*(WriteData + i*2 + 4) & 0xff));
        i++;
    }
}

/*写入寄存器位状态处理*/
uint16_t Write_WordManage(uint8_t State, uint16_t data, uint8_t n)
{
    if (State)
    {
        return data = data | 0x01 << n;
    }
    else
    {
        return data = data & ~(0x01 << n);
    }
}

/*读取寄存器位状态判断处理*/
uint8_t Read_WordManage(uint16_t data, uint8_t n)
{
    if (data & 0x01 << n)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*十六进制数转换浮点数*/
float HextoFlot(uint16_t *Data)
{
    uint8_t paress[4];
    uint8_t i = 0, len = 2;

    while (len--)
    {
        paress[i * 2] = *(Data + i) >> 8;
        paress[i * 2 + 1] = *(Data + i) & 0xFF;
        i++;
    }

    float *fa = (float *)paress;

    return *fa;

    /*float value;

    memcpy(&value, Data, sizeof(value));

    return value;*/
}
/*浮点数转换十六进制数*/
void FloattoHex(uint16_t *Data, float value)
{
    uint8_t farray[4];
    uint8_t i = 0, len = 2;

    memcpy(farray, &value, sizeof(farray));
    memcpy(&value, &farray, sizeof(farray));

    while (len--)
    {
        *(Data + i) = (uint16_t)farray[i * 2] << 8 | farray[i * 2 + 1];
        i++;
    }
}
