/*
 * @Description:
 * @Author: xiao
 * @Date: 2022-10-15 14:01:01
 * @LastEditTime: 2025-09-03 10:27:26
 * @LastEditors: xiao
 */
#ifndef _VARABLE_DEFINE_H
#define _VARABLE_DEFINE_H

#include "main.h"

/*˽�к궨��-----------------------------------------------------------------------*/
#define FALSE 0
#define TRUE 1
#define Deal_Len1 18 // Э��1���ݳ���
#define Deal_Len2 50 // Э��2���ݳ���
#define Deal_Len3 100          // Э��3���ݳ���
#define TestPowerSta    0x3c
#define TestErrorSta    0x83
#define Symbol_Compute 0x10000  //����������
#define Min_Electricity 0x0064  //��С����
#define Min_Electricity1 0x0032 // ������С����

/*�ⲿ����*/
extern uint16_t Heartbeat;

typedef struct
{
    /*д��Ĵ�������*/
    uint8_t Heartcount[2];          // 0������
    uint8_t Panel_CurrentSta;       // ��嵱ǰ״̬
    uint8_t Current_BatterySOC;     // ��ǰ���SOC
    uint8_t Current_InpuPower[2];   // ��ǰ���빦��
    uint8_t Current_OutputPower[2]; // ��ǰ�������
    uint8_t Panel_ErrorCode;        // ��������
    uint8_t WriteStandby1;          // д��Ԥ��
    uint8_t WriteStandby2;          // д��Ԥ��
    uint8_t WriteStandby3;          // д��Ԥ��
    uint8_t WriteStandby4;          // д��Ԥ��
    uint8_t WriteStandby5;          // 13 д��Ԥ��

    /*��ȡ�Ĵ�������*/
    uint8_t Trigger_OrderSta;        // 14 ��������״̬
    uint8_t BatterySOC_SetValue;     // ���SOC�趨ֵ
    uint8_t InputPower_SetValue[2];  // ���빦���趨ֵ
    uint8_t OutputPower_SetValue[2]; // ��������趨ֵ
    uint8_t Trigger_ErrorCode;       // ����������
    uint8_t ReadStandby1;            // ��ȡԤ��
    uint8_t ReadStandby2;            // ��ȡԤ��
    uint8_t ReadStandby3;            // ��ȡԤ��
    uint8_t ReadStandby4;            // ��ȡԤ��
    uint8_t ReadStandby5;            // ��ȡԤ��
    uint8_t ReadStandby6;            // ��ȡԤ��
} Deal_Buf1_t;

typedef struct
{
    /*��ȡ�Ĵ�������*/
    uint8_t Total_Voltage[2];           // 0�ܵ�ѹ
    uint8_t Electricity[2];             // ����
    uint8_t Surplus_Capacity[2];        // ʣ������
    uint8_t Nominal_Capacity[2];        // �������
    uint8_t Circulation_Num[2];         // ѭ������
    uint8_t Date_of_Manufacture[2];     // ��������
    uint8_t EquilibriumSta[2];          // ����״̬
    uint8_t EquilibriumSta_High[2];     // ����״̬_��
    uint8_t ProtectSta[2];              // ����״̬
    uint8_t Software_Version;           // ����汾
    uint8_t RSOC;                       // RSOC
    uint8_t FET_ControlSat;             // FET����״̬
    uint8_t Battery_Num;                // ��ش���
    uint8_t NTC_Num;                    // NTC����
    uint8_t NTC_Temperature[2];         // N��NTC�¶�
    uint8_t Humidity[2];                // ʪ��
    uint8_t ReportSta[2];               // �澯״̬
    uint8_t Filled_full_Capacity[2];    // ��������
    uint8_t Surplus_Capacity1[2];       // ʣ������
    uint8_t Equilibrium_Electriciyt[2]; // �������
    uint8_t ReadStandby1;               // ��ȡԤ��
    uint8_t ReadStandby2;               // ��ȡԤ��
    uint8_t ReadStandby3;               // ��ȡԤ��
    uint8_t ReadStandby4;               // ��ȡԤ��
    uint8_t ReadStandby5;               // ��ȡԤ��
    uint8_t ReadStandby6;               // ��ȡԤ��
} Deal_Buf2_t;

typedef struct
{
    /*��ȡ�Ĵ�������*/
    uint16_t System_Sta;                        // 0ϵͳ״̬
    uint16_t Battery_Sta;                       // ���״̬
    uint16_t Level_1_Protect_Sta[2];            // 1������״̬
    uint16_t Level_2_Protect_Sta[2];            // 2������״̬
    uint16_t Level_3_Protect_Sta[2];            // 3������״̬
    uint16_t Error_Sta[2];                      // ����״̬
    uint16_t Swicth_Control_Sta[2];             // ���ؿ���״̬
    uint16_t Swicth_Response_Sta[2];            // ������Ӧ״̬
    uint16_t Relay_Real_Sta[2];                 // �̵�����ʵ״̬
    uint16_t Sensor_Sta[2];                     // ������״̬
    uint16_t Special_Sta[2];                    // ����״̬
    uint16_t RESERVE[2];                        // RESERVE
    uint16_t Monomer_Total_Voltage;             // �ܵ�ѹ�������ۼӣ�
    uint16_t Gather_Total_Voltage;              // �ܵ�ѹ��ֱ�Ӳɼ���
    uint16_t Load_Voltage;                      // ���ص�ѹ
    uint16_t Total_Electricity[2];              // �ܵ���
    uint16_t SOC;                               // SOC
    uint16_t SOH;                               // SOH
    uint16_t Battery_Surplus_Capacity[2];       // ���ʣ������
    uint16_t Battery_Sufficient_Capacity[2];    // �����������
    uint16_t Battery_Design_Capacity[2];        // ����������
    uint16_t Cycle_index;                       // ѭ������
    uint16_t BMU_Quantity;                      // BMU����
    uint16_t BMU_Online_Quantity;               // BMU��������
    uint16_t Positive_Insulation_Resistance[2]; // ������Ե��ֵ
    uint16_t Cathode_Insulation_Resistance[2];  // ������Ե��ֵ
} Deal_Buf3_t;

extern Deal_Buf1_t Deal_Buf1;
extern Deal_Buf2_t Deal_Buf2;
extern Deal_Buf3_t Deal_Buf3;

/*Э��1���ݴ�������*/
extern void Deal_WriteDataDispose(uint8_t *ReadData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose1(uint8_t *WriteData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose2(uint8_t *WriteData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose3(uint8_t *WriteData, uint16_t addr, uint8_t lent);
/*��д�Ĵ���λ������������*/
extern uint16_t Write_WordManage(uint8_t State, uint16_t data, uint8_t n);
extern uint8_t Read_WordManage(uint16_t data, uint8_t n);

/*���㻥��ת��ʮ������*/
extern float HextoFlot(uint16_t *Data);
void FloattoHex(uint16_t *Data, float value);
uint16_t Count_Bits(uint16_t Num);


#endif
