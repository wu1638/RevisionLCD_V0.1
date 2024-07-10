/******************************************************************************
 * �ļ�����: Mannger_Sharing.c
 * ��    ��: xiao
 * ��    ��: V1.0
 * ��д����: 2022-10-14
 * ��    ��: ���ú����ӿڹ���
 ******************************************************************************
 */

#include "Manager.h"
#include <stdlib.h>

/*�ⲿ���ñ���*/

/*˽�к궨��--------------------------------------------------------------------*/
#define N 12

/*˽�б�������----------------------------------------------------------------------*/
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
    IWDG_Init(IWDG_PRESCALER_64, 4095); // ���Ź���ʼ��
}

void Manager_Sharing_Activate(void)
{
}

void Manager_Sharing_Mainloop(void)
{
    FeedDog();

    // ReadRegistertomappingBuf(usSRegHoldBuf, 20, 23); // ��ȡ�Ĵ�����ʹ�ö���buf
    // Mapp_WriteToRegister(usSRegHoldBuf, 0, 20);      // ʹ�ö���bufд�뵽�Ĵ���
    //*(DealSendData1 + 2) = (uint8_t)*(Deal_Buf1.Heartcount);
    //*(DealSendData1 + 3) = (uint8_t)*(Deal_Buf1.Heartcount + 1);
}

/*��ӳ���ַд�뵽�Ĵ�����ַ*/
void Deal_WriteDataDispose(uint8_t *ReadData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(ReadData + i + 2) = (uint8_t) * (Deal_Buf1.Heartcount + addr + i);
        i++;
    }
}
/*Э��1�ӼĴ�����ȡ��ӳ���ַ*/
void Deal_ReadDataDispose1(uint8_t *WriteData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(Deal_Buf1.Heartcount + addr + i) = *(WriteData + i + 3);
        i++;
    }
}
/*Э��2�ӼĴ�����ȡ��ӳ���ַ*/
void Deal_ReadDataDispose2(uint8_t *WriteData, uint16_t addr, uint8_t lent)
{
    uint8_t i = 0;

    while (lent--)
    {
        *(Deal_Buf2.Total_Voltage + addr + i) = *(WriteData + i + 4);
        i++;
    }
}
/*Э��3�ӼĴ�����ȡ��ӳ���ַ*/
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

/*д��Ĵ���λ״̬����*/
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

/*��ȡ�Ĵ���λ״̬�жϴ���*/
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

/*ʮ��������ת��������*/
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
/*������ת��ʮ��������*/
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
