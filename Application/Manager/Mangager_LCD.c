/**
 ******************************************************************************
 * �ļ�����: Mannger_LCD.c
 * ��    ��: xiao
 * ��    ��: V1.0
 * ��д����: 2022-06-28
 * ��    ��: ��������LCD
 ******************************************************************************
 */

#include "Manager.h"

/*˽�к궨��--------------------------------------------------------------------------*/
#define DIG0 0
#define DIG1 16
#define DIG2 24
#define DIG3 32
#define DIG4 40
#define DIG5 104
#define DIG6 112
#define DIG7 1
#define DIG8 8
#define DIG9 80 // 72
#define DIGA 72 // 80
#define POWER_TR 49
#define WIFI_TR 97
#define Ero_TR 90
#define MAE_TR 57
#define MAE_RR 65
/*˽�б���----------------------------------------------------------------------------*/
//  ����1-8            0    1     2    3    4    5    6    7   8    9
uint8_t numdata[] = {0x5f, 0x06, 0x3d, 0x2f, 0x66, 0x6b, 0x7b, 0x0e, 0x7f, 0x6f}; //"0~9"char
//   ����9-10              0    1     2    3    4    5    6    7   8    9
uint8_t numdata1[] = {0xf5, 0x60, 0xb6, 0xf2, 0x63, 0xd3, 0xd7, 0x70, 0xf7, 0xf3}; //"0~9"char
//  �����R1 - R30         "R1/R13"��"R2/R12/R19/R25/R26"��"R3/R11/R18/R24/R27"��"R4/R10/R17/R23/R28"��"R5/R9/R16/R22/R29"��"R6/R14"��"R7/R15"��"R8/R20/R21/R30"
uint8_t Marqueedata[] = {0x10, 0x01, 0x02, 0x04, 0x08, 0x20, 0x40, 0x80};
//  �����  ��R30-26��R1������R25-22������R19-16������R15-9������R8-2��
uint8_t MarBright[] = {0x9f, 0xef, 0x7f, 0x0f};
// ����Ƶ������                              4Bit/0x8f:R30 - R26ȫ��   8bit/0x0f��R25-22  12bit/0x0f:R19-16ȫ��  19Bit/0x7F:R15-9ȫ��  26Bit/0xef��R8-2ȫ��
uint8_t MarConLamp[] = {0x80, 0x88, 0x8c, 0x8e, 0x8f, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x03, 0x07, 0x0f, 0x40, 0x60, 0x70, 0x71, 0x73, 0x77, 0x7f, 0x80, 0xc0, 0xe0, 0xe8, 0xec, 0xee, 0xef, 0x10, 0x80, 0x80};
/*������ʾT*/
uint8_t Numdata_T[] = {0x40, 0x20, 0x08, 0x80, 0x60}; //"T1"��"T2"��"T3/T4"��"T9/T10/T11/T12/T13/T14/T18"����T1/T2��
/*״̬��ʾT*/
uint8_t Stadata_T[5] = {0x10, 0x20, 0x40, 0x80, 0xe0};                                 // "T5/T15"��"T6/T16"��"T7/T17"��"T8/T19"��"T16/T17/T18"
uint8_t numadd[] = {DIG0, DIG1, DIG2, DIG3, DIG4, DIG5, DIG6, DIG7, DIG8, DIG9, DIGA}; //"dig"char

uint8_t disp_buff_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Display_Fag = 0, Fixed_Value = 0x90, RestartTime = 0;
uint8_t Count = 0, Dis_InitFlag = 0, Char_InitFlag = 0, Await_InitFlag = 0, TriggerFlickFlag = 0;
uint32_t Charge_Time = 0;
uint16_t Show_InputPower = 0, Show_OutputPower = 0, Show_Voltage = 0, Judge_Electricity = 0, Use_ReportSta = 0, Dormancy_Electricity = 0, Use_RunSta = 0;

/*�ⲿ���ñ���------------------------------------------------------------------------*/
extern uint16_t FlickerFlg, Cir_FlickerFlag;
extern uint16_t ErrorFlickerFlag;
extern uint8_t Deal_TriggerMode1, Deal_TriggerMode2, Deal_TriggerMode3;
extern uint16_t DormancySta;
/*˽�к���ԭ��------------------------------------------------------------------------*/
void Manager_LCD_Init(void);
void Manager_LCD_Mainloop(void);

void LightenLCD_Init(void);
void Init_Interface(void);
void Display_Frame(void);
// Э��1
void Deal_Battery_Power_Show_1(void);
void Circulation_ErrorFlickerMode_1(uint8_t Num, uint8_t Mode);
void Circulation_Marquee_1(uint8_t Indata, uint8_t PowerSta,uint8_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
void MarBrControl_1(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode);
// Э��2
void Deal_Battery_Power_Show_2(void);
void Circulation_ErrorFlickerMode_2(uint8_t Num, uint8_t Mode);
void Circulation_Marquee_2(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_2(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_2(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
void MarBrControl_2(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode);
// Э��3
void Deal_Battery_Power_Show_3(void);
void Circulation_Marquee_3(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_3(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_3(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
/*��ʼ��*/
void Manager_LCD_Init(void)
{
    HT1621_Init();
    LightenLCD_Init();
    Init_Interface();
}
/*��loop����*/
void Manager_LCD_Mainloop(void)
{
    Display_Frame();
}
// ������� ��ʮ��ǧλ��ʾ
void Output_Power(unsigned long a, unsigned long power, uint8_t Cha_Dis_Sta, uint8_t mode)
{
    int thousandplace = a / 1000 % 10;
    int hunderdplace = a / 100 % 10;
    int tenplace = a / 10 % 10;
    int unitplace = a / 1 % 10;

    if (Cha_Dis_Sta)
    {
        if (power == 20)
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace] + Marqueedata[7]); // ǧλ
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace]); // ǧλ
        }
    }
    else
    {
        /*����С��62% R20�ƴ���*/
        if (power <= 62)
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace]); // ǧλ
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace] + Marqueedata[7]); // ǧλ
        }
    }
    if (mode)
    {
        /* ��ʾ��ѹ���� */
        HT1621_WriteData8Bit(DIG2, numdata[hunderdplace] + Numdata_T[3]); // ��λ
        HT1621_WriteData8Bit(DIG3, numdata[tenplace] + Numdata_T[3]);     // ʮλ
        HT1621_WriteData8Bit(DIG4, numdata[unitplace]);                   // ��λ
    }
    else
    {
        /* ��ʾ���ʷ���*/
        HT1621_WriteData8Bit(DIG2, numdata[hunderdplace] + Numdata_T[3]); // ��λ
        HT1621_WriteData8Bit(DIG3, numdata[tenplace]);                    // ʮλ
        HT1621_WriteData8Bit(DIG4, numdata[unitplace] + Numdata_T[3]);    // ��λ
    }
}
/*���빦����ʾ����*/
void Input_Power(unsigned long a, unsigned long power, uint8_t Cha_Dis_Sta, uint8_t mode)
{
    int thousandplace = a / 1000 % 10;
    int hunderdplace = a / 100 % 10;
    int tenplace = a / 10 % 10;
    int unitplace = a / 1 % 10;

    if (Cha_Dis_Sta)
    {
        if (power == 21)
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace] + Marqueedata[7]); // ǧλ
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace]); // ǧλ
        }
    }
    else
    {
        /*����С��66% R21�ƴ���*/
        if (power <= 65)
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace]); // ǧλ
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace] + Marqueedata[7]); // ǧλ
        }
    }
    if (mode)
    {
        /* ��ʾ��ѹ���� */
        HT1621_WriteData8Bit(DIG6, numdata[hunderdplace] + Numdata_T[3]); // ��λ
        HT1621_WriteData8Bit(DIG7, numdata[tenplace] + Numdata_T[3]);     // ʮλ
        HT1621_WriteData8Bit(DIG8, numdata[unitplace]);                   // ��λ
    }
    else
    {
        /* ��ʾ���ʷ���*/
        HT1621_WriteData8Bit(DIG6, numdata[hunderdplace] + Numdata_T[3]); // ��λ
        HT1621_WriteData8Bit(DIG7, numdata[tenplace]);                    // ʮλ
        HT1621_WriteData8Bit(DIG8, numdata[unitplace] + Numdata_T[3]);    // ��λ
    }
}
/*��ص���������ʾ*/
void Battery_Dispose(unsigned long a)
{
    int Decade = a / 10 % 10;
    int Individual = a / 1 % 10;

    if (a >= 100)
    {
        HT1621_WriteData8Bit(DIG9, numdata1[0] + Numdata_T[2]); //
        HT1621_WriteData8Bit(DIGA, numdata1[0] + Numdata_T[2]); //
    }
    else
    {
        HT1621_WriteData8Bit(DIG9, numdata1[Decade]);                    //
        HT1621_WriteData8Bit(DIGA, numdata1[Individual] + Numdata_T[2]); //
    }
}

/*�����ʾ*/
void Charge_Dispose(uint16_t ErrorSta, unsigned long InputPower, unsigned long OutputPower, uint8_t mode, uint8_t Deal_Num)
{
    /*���������*/
    if (Cir_FlickerFlag)
    {
        Count++;
        Cir_FlickerFlag = FALSE;
    }
    if (Count == 31)
    {
        Count = FALSE;
    }
    /*���빦����ʾ*/
    Input_Power(InputPower, Count, 1, mode);
    /*���������ʾ*/
    Output_Power(OutputPower, Count, 1, mode);
    /*��������*/
    if (Deal_Num == 1)
    {
        Circulation_Marquee_1(Count, Deal_Buf1.Trigger_OrderSta, ErrorSta, InputPower, OutputPower);
    }
    else if (Deal_Num == 2)
    {
        Circulation_Marquee_2(Count, ErrorSta, InputPower, OutputPower);
    }
    else if (Deal_Num == 3)
    {
        Circulation_Marquee_3(Count, ErrorSta, InputPower, OutputPower);
    }
}
/*��ʾ���ܽ���*/
void Display_Frame(void)
{
    if (Deal_TriggerMode1)
    {
        /*Э��1ģʽ*/
        /*�����������*/
        Show_InputPower = (uint16_t)(Deal_Buf1.InputPower_SetValue[0] << 8 | Deal_Buf1.InputPower_SetValue[1]);
        Show_OutputPower = (uint16_t)(Deal_Buf1.OutputPower_SetValue[0] << 8 | Deal_Buf1.OutputPower_SetValue[1]);
        /*���������빦����ʾ*/
        Deal_Battery_Power_Show_1();
    }
    else if (Deal_TriggerMode2)
    {
        /*Э��2ģʽ*/
        Show_Voltage = (uint16_t)(Deal_Buf2.Total_Voltage[0] << 8 | Deal_Buf2.Total_Voltage[1]); // �ܵ�ѹ
        Show_Voltage = Show_Voltage / 100;
        Judge_Electricity = (uint16_t)(Deal_Buf2.Electricity[0] << 8 | Deal_Buf2.Electricity[1]); // �жϵ�������
        Use_ReportSta = (uint16_t)(Deal_Buf2.ReportSta[0] << 8 | Deal_Buf2.ReportSta[1]);         // �澯״̬����

        /*������������ʾ*/
        Deal_Battery_Power_Show_2();
    }
    else if (Deal_TriggerMode3)
    {
        /*Э��3ģʽ*/
        Use_RunSta = Deal_Buf3.System_Sta;             // ϵͳ״̬
        Show_Voltage = Deal_Buf3.Gather_Total_Voltage; // �ܵ�ѹ
        Show_Voltage = Show_Voltage / 10;
        Dormancy_Electricity = (Symbol_Compute - Deal_Buf3.Total_Electricity[1]); // ���ߵ���
        Use_ReportSta = Deal_Buf3.Level_1_Protect_Sta[1];                         // ����״̬���ֽ�

        /*������������ʾ*/
        Deal_Battery_Power_Show_3();
    }
}

/*Э��1��������ع�����ʾ*/
void Deal_Battery_Power_Show_1(void)
{

    /*�ŵ�״̬*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*���빦����ʾ*/
        Input_Power(Show_InputPower, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*���������ʾ*/
        Output_Power(Show_OutputPower, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_1(Deal_Buf1.BatterySOC_SetValue, Deal_Buf1.Trigger_OrderSta, Deal_Buf1.Trigger_ErrorCode, Show_OutputPower, FALSE);
    }
    /*���״̬*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0))
    {
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*�����ʾ*/
        Charge_Dispose(Deal_Buf1.Trigger_ErrorCode, Show_InputPower, Show_OutputPower, FALSE, 1);
    }
    /*����״̬*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 6) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*���빦����ʾ*/
        Input_Power(0, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*���������ʾ*/
        Output_Power(0, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_1(Deal_Buf1.BatterySOC_SetValue, Deal_Buf1.Trigger_OrderSta, Deal_Buf1.Trigger_ErrorCode, 0, TRUE);
    }

    if (!Read_WordManage(Deal_Buf1.Trigger_OrderSta, 6) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        Init_Interface();
        /*�ر���*/
        SuperGpio_WriteOutput(0, 0);
    }
}
/*Э��2��������ع�����ʾ*/
void Deal_Battery_Power_Show_2(void)
{

    /*�ŵ�״̬*/
    if (Read_WordManage(Judge_Electricity, 15) && (Symbol_Compute - Judge_Electricity > Min_Electricity))
    {
        /*�ر�����*/
        DormancySta = FALSE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*���빦����ʾ*/
        Input_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*���������ʾ*/
        Output_Power(Show_Voltage, Deal_Buf2.RSOC, 0, TRUE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_2(Deal_Buf2.RSOC, Use_ReportSta, Show_Voltage, FALSE);
    }
    /*���״̬*/
    if (!Read_WordManage(Judge_Electricity, 15) && Judge_Electricity > Min_Electricity)
    {
        /*�ر�����*/
        DormancySta = FALSE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*�����ʾ*/
        Charge_Dispose(Use_ReportSta, Show_Voltage, 0, TRUE, 2);
    }
    /*����״̬*/
    if ((Judge_Electricity == 0 || (Judge_Electricity > 0 && (Symbol_Compute - Judge_Electricity <= Min_Electricity)) || (Judge_Electricity > 0 && Judge_Electricity <= Min_Electricity)) && !DormancySta)
    {
        /*���¼�ʱ*/
        RestartTime = TRUE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*���빦����ʾ*/
        Input_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*���������ʾ*/
        Output_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_2(Deal_Buf2.RSOC, Use_ReportSta, Show_Voltage, TRUE);
    }
    /*3���ӽ�������״̬*/
    if (DormancySta)
    {
        Init_Interface();
        /*�ر���*/
        SuperGpio_WriteOutput(0, 0);
    }
    /*С��1A������������*/
    if ((Judge_Electricity > 0 && (Symbol_Compute - Judge_Electricity <= Min_Electricity)) ||
        (Judge_Electricity > 0 && Judge_Electricity <= Min_Electricity))
    {
        DormancySta = FALSE;
    }
}
/*Э��3��������ع�����ʾ*/
void Deal_Battery_Power_Show_3(void)
{

    /*�ŵ�״̬*/
    if (Use_RunSta == 0x03 && (Dormancy_Electricity > Min_Electricity1))
    {
        /*�ر�����*/
        DormancySta = FALSE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*���빦����ʾ*/
        Input_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*���������ʾ*/
        Output_Power(Show_Voltage, Deal_Buf3.SOC, 0, TRUE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_3(Deal_Buf3.SOC, Use_ReportSta, Show_Voltage, FALSE);
    }
    /*���״̬*/
    if (Use_RunSta == 0x02 && (Dormancy_Electricity > Min_Electricity1))
    {
        /*�ر�����*/
        DormancySta = FALSE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*�����ʾ*/
        Charge_Dispose(Use_ReportSta, Show_Voltage, 0, TRUE, 3);
    }
    /*����״̬*/
    if ((Use_RunSta == 0x01 || Use_RunSta == 0x02 || Use_RunSta == 0x03) &&
        (Dormancy_Electricity <= Min_Electricity1) && !DormancySta)
    {
        /*���¼�ʱ*/
        RestartTime = TRUE;
        /*������*/
        SuperGpio_WriteOutput(0, 1);
        /*��ʼ����ʾ*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*��ص�����ʾ%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*���빦����ʾ*/
        Input_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*���������ʾ*/
        Output_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*�ŵ�ģʽ*/
        Discharge_Marquee_3(Deal_Buf3.SOC, Use_ReportSta, Show_Voltage, TRUE);
    }
    /*3���ӽ�������״̬*/
    if (DormancySta)
    {
        Init_Interface();
        /*�ر���*/
        SuperGpio_WriteOutput(0, 0);
    }
    /*С��0.5A������������*/
    if (Dormancy_Electricity > 0 && (Dormancy_Electricity <= Min_Electricity1))
    {
        DormancySta = FALSE;
    }
}
/*��ʼ��LCD��ȫ��ʾ*/
void LightenLCD_Init(void)
{
    HT1621_WriteData8Bit(DIG0, 0xff);
    HT1621_WriteData8Bit(DIG1, 0xff);
    HT1621_WriteData8Bit(DIG2, 0xff);
    HT1621_WriteData8Bit(DIG3, 0xff);
    HT1621_WriteData8Bit(DIG4, 0xff);
    HT1621_WriteData8Bit(DIG5, 0xff);
    HT1621_WriteData8Bit(DIG6, 0xff);
    HT1621_WriteData8Bit(DIG7, 0xff);
    HT1621_WriteData8Bit(DIG8, 0xff);
    HT1621_WriteData8Bit(DIG9, 0xff);
    HT1621_WriteData8Bit(DIGA, 0xff);
    HT1621_WriteData8Bit(POWER_TR, 0xff);
    HT1621_WriteData8Bit(WIFI_TR, 0xff);
    HT1621_WriteData8Bit(Ero_TR, 0xff);
    HT1621_WriteData8Bit(MAE_TR, 0xff);
    HT1621_WriteData8Bit(MAE_RR, 0xff);
    HAL_Delay(3000);
}
/*��ʾ��ʼ����*/
void Init_Interface(void)
{
    for (uint8_t i = 0; i < 120; i++)
    {
        HT1621_WriteData8Bit(i, 0x00); // clear all display
    }
}

/*---------------------------------------------------------------------------------Э��1-----------------------------------------------------------------------------*/
/*Э��1���ѭ�������*/
void Circulation_Marquee_1(uint8_t Indata, uint8_t PowerSta,uint8_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)) && !Read_WordManage(ErrorSat, 7))
    {
        mode = 1;
    }
    /*��ع�����ʾ*/
    if (Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 2;
    }
    /*��ع��ϡ���ع�����ʾ*/
    if (Read_WordManage(ErrorSat, 7) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)))
    {
        mode = 3;
    }
    /*PV������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) 
    && !Read_WordManage(ErrorSat, 2))
    {
        mode = 4;
    }
    
    /*---------------------------------���ܴ�����ʾ-----------------------------------------------------*/
    if (Read_WordManage(PowerSta, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) 
    && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 5;
    }
    
    /*�޹���״̬*/
    if (!Read_WordManage(PowerSta, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) 
    && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 0;
    }
    /*----------------------------------------------�����--------------------------------------------*/
    if (Indata == 1)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 2)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[1]);
    }
    else if (Indata == 3)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[2]);
    }
    else if (Indata == 4)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[3]);
    }
    else if (Indata == 5)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[4]);
    }
    else if (Indata == 6)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[5]);
    }
    else if (Indata == 7)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[6]);
    }
    else if (Indata == 8)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[7]);
    }
    else if (Indata == 9)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, 0x00);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[4]);
    }
    else if (Indata == 10)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[3]);
    }
    else if (Indata == 11)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[2]);
    }
    else if (Indata == 12)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[1]);
    }
    else if (Indata == 13)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[0]);
    }
    else if (Indata == 14)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[5]);
    }
    else if (Indata == 15)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[6]);
    }
    else if (Indata == 16)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, 0x00);
    }
    else if (Indata == 17)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 18)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 19)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 20)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(DIG1, Marqueedata[7] + numdata[Out_thousandplace]);
    }
    else if (Indata == 21)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(DIG1, numdata[Out_thousandplace]);
        HT1621_WriteData8Bit(DIG5, Marqueedata[7] + numdata[in_thousandplace]);
    }
    else if (Indata == 22)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(DIG5, numdata[in_thousandplace]);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[4]);
    }
    else if (Indata == 23)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[3]);
    }
    else if (Indata == 24)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[2]);
    }
    else if (Indata == 25)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[1]);
    }
    else if (Indata == 26)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, 0x00);
    }
    else if (Indata == 27)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 28)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 29)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else if (Indata == 30)
    {
        Circulation_ErrorFlickerMode_1(Indata, mode);
    }
    else
    {
        Circulation_ErrorFlickerMode_1(1, mode);
    }
}

/*Э��1�����ϵ���˸����*/
void Circulation_ErrorFlickerMode_1(uint8_t Num, uint8_t Mode)
{
    if (Num == 1)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
        }
    }
    else if (Num == 16)
    {
        if (Mode == 4)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[0] + Stadata_T[1]);
            }
        }
        else if (Mode == 5)
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[1]);
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[4]);
        }  
    }
     else if (Num == 17)
    {
        if (Mode == 4)
        {
             if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[0] + Stadata_T[1]);
            }
        }
        else if (Mode == 5)
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[1]);
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[3]);
        }
    }
     else if (Num == 18)
    {
         if (Mode == 4)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[0] + Stadata_T[1]);
            }
        }
        else if (Mode == 5)
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[1]);
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[2]);
        }
    }
     else if (Num == 19)
    {
         if (Mode == 4)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[0] + Stadata_T[1]);
            }
        }
        else if (Mode == 5)
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[1]);
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[1]);
        }
    }
    else if (Num == 26)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
        }
    }
    else if (Num == 27)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
        }
    }
    else if (Num == 28)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
        }
    }
    else if (Num == 29)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
        }
    }
    else if (Num == 30)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[0] + Numdata_T[1]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
        }
    }
    else
    {
        /*������˸*/
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0x00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0x00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0x00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
            }
        }
        else if (Mode == 4)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR,  Stadata_T[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0] + Stadata_T[1]);
            }
        }
         else if (Mode == 5)
        {
            HT1621_WriteData8Bit(Ero_TR, 0x00);
            HT1621_WriteData8Bit(POWER_TR, Stadata_T[1]);
        }
        else
        {

            HT1621_WriteData8Bit(POWER_TR, 0x00);
            HT1621_WriteData8Bit(Ero_TR, 0x00);
        }
    }
}

/*Э��1�ŵ��������ʾ*/
void Discharge_Marquee_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1��˸ */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*Э��1������ʾ״̬����*/
void DisSta_Conrtol_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*��˸����*/
    if (FlickerFlg)
    {
        TriggerFlickFlag = TRUE;
        FlickerFlg = FALSE;
    }
    else
    {
        TriggerFlickFlag = FALSE;
    }

    if (Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV ��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1], 0, 3);
    }
    if (Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC ��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2], 0, 3);
    }
    if (Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*USB ��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Numdata_T[3], 0, 4);
    }
    if (Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*AC ��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��DC��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��USB��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��AC��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC��USB��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC��AC��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*USB��AC��ʾ*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[3], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��DC��USB*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��DC��AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV��USB��AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[3], Numdata_T[3], 8);
    }
    if (!Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC��USB��AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /* PV��DC��USB��AC */
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
    }
    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)) && !Read_WordManage(ErrorSat, 7))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Numdata_T[1], 0, 1);
        }
    }
    /*PV������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1], 0, 3);
        }
    }
    /*DC������ʾ*/
    if (Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[2], 0, 3);
        }
    }
    /*USB������ʾ*/
    if (Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0], Numdata_T[3], 8);
        }
    }
    /*AC������ʾ*/
    if (Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[3], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[3], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[3], 0, 3);
        }
    }
    /*PV��DC������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[2], 0, 3);
        }
    }
    /*PV��USB������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1], Numdata_T[3], 8);
        }
    }
    /*PV��AC������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[3], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[3], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[3], 0, 3);
        }
    }
    /*USB��DC������ʾ*/
    if (Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[2], Numdata_T[3], 8);
        }
    }
    /*USB��AC������ʾ*/
    if (Read_WordManage(ErrorSat, 5) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[3], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[3], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[3], Numdata_T[3], 8);
        }
    }
    /*DC��AC������ʾ*/
    if (Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2] + Stadata_T[3], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2] + Stadata_T[3], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[2] + Stadata_T[3], 0, 3);
        }
    }
    /*PV��DC��AC������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], 0, 3);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], 0, 3);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[2] + Stadata_T[3], 0, 3);
        }
    }
    /*PV��DC��USB������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[2], Numdata_T[3], 8);
        }
    }
    /*PV��AC��USB������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 5) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[3], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[3], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[3], Numdata_T[3], 8);
        }
    }
    /*DC��AC��USB������ʾ*/
    if (Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 5) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
        }
    }
    /*PV��DC��USB��AC������ʾ*/
    if (Read_WordManage(ErrorSat, 3) && Read_WordManage(ErrorSat, 4) && Read_WordManage(ErrorSat, 5) && Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Stadata_T[0] + Stadata_T[1] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
        }
    }
    /*��ع�����ʾ*/
    if (Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Numdata_T[0], 0, 1);
        }
    }
    /*��ع��ϡ���ع�����ʾ*/
    if (Read_WordManage(ErrorSat, 7) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_1(Electric, FALSE, Num, PowerNum, Numdata_T[0] + Numdata_T[1], 0, 1);
        }
    }

    if (!Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /* ��״̬���޹���״̬ */
        if (SteadySta)
        {
            MarBrControl_1(Electric, TRUE, Num, PowerNum, 0, 0, 10);
        }
        else
        {
            MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, 0, 0, 0);
        }
    }
}

/*Э��1LED�ƿ���*/
void MarBrControl_1(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode)
{
    // R30-26�ƿ���
    if (FickerSta && Num < 5)
    {
        if (mode == 1)
        {
            if (Electric > 99)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData); //  ��ӦR30����(����)
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num]); // ��ӦR30-26����
            }
        }
        else if (mode == 10 && Electric >= 97 && Electric < 99)
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0]); // ��ӦR30����(����)
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num]); // ��ӦR30-26����
        }
    }
    else
    {
        // ��ӦR30-26����
        if (Num > 0 && Num < 5)
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 5 && Num != 27)
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[4]);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[4]);
            }
        }
        else if (Num == 27)
        {
            HT1621_WriteData8Bit(Ero_TR, MarConLamp[Num]); // ��ӦR1����
        }
        else
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0]);
            }
        }
        // else
        // {
        //     HT1621_WriteData8Bit(Ero_TR, MarBright[0]);
        // }
    }
    // R25-22�ƿ���
    if (FickerSta && Num >= 5 && Num <= 8)
    {
        if (mode == 2)
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num]); // ��ӦR25-22����
        }
        else
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num]); // ��ӦR25-22����
        }
    }
    else
    {
        // ��ӦR25-22����
        if (Num > 5 && Num <= 8)
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num - 1]);
            }
        }
        else if (Num > 8)
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[8]);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[8]);
            }
        }
        else
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3]);
            }
        }
    }
    // R19-16�ƿ���
    if (FickerSta && Num >= 9 && Num <= 12)
    {
        if (mode == 3 || mode == 8)
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num]); // ��ӦR19-16����
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num]); // ��ӦR19-16����
        }
    }
    else
    {
        // ��ӦR19-16����
        if (Num > 9 && Num <= 12)
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num - 1]);
            }
        }
        else if (Num > 12 && Num != 28 && Num != 29)
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[12]);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[12]);
            }
        }
        else
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3]);
            }
        }
    }
    // R15-9�ƿ���
    if (FickerSta && Num >= 13 && Num <= 19)
    {
        if (mode == 4)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num]); // ��ӦR15-9����
        }
        else if (mode == 8)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num]); // ��ӦR15-9����
        }
        else
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num]); // ��ӦR15-9����
        }
    }
    else
    {
        // ��ӦR15-9����
        if (Num > 13 && Num <= 19)
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num - 1]);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 19 && Num != 28 && Num != 29)
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[19]);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[19]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[19]);
            }
        }
        else
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2]);
            }
        }
    }
    // R8-2�ƿ���
    if (FickerSta && Num >= 20 && Num <= 26)
    {
        if (mode == 5)
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num]); // ��ӦR8-2����
        }
        else
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num]); // ��ӦR8-2����
        }
    }
    else
    {
        // ��ӦR8-2����
        if (Num > 20 && Num <= 26)
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 26 && Num != 28 && Num != 29)
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[26]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[26]);
            }
        }
        else
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1]);
            }
        }
    }
    // R1�ƿ���
    if (FickerSta && Num == 27)
    {
        HT1621_WriteData8Bit(Ero_TR, 0x00); // ��ӦR1����
    }
    // R21�ƿ���
    if (FickerSta && Num == 28)
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, numdata[0] + AppData); // R21��
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[0]); // R21��
        }
    }
    else if (Num >= 0 && Num <= 8)
    {
        HT1621_WriteData8Bit(DIG5, MarConLamp[28] + numdata[0]); // R21��
    }
    else if ((Num >= 9 && Num <= 27) || Num == 29)
    {
        HT1621_WriteData8Bit(DIG5, numdata[0]); // R21��
    }
    else
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + AppData + numdata[0]); // R21��
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + numdata[0]); // R21��
        }
    }
    // R20�ƿ���
    if (FickerSta && Num == 29)
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum] + AppData); // R20��
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20��
        }
    }
    else if (Num >= 0 && Num <= 8 || Num == 28)
    {
        HT1621_WriteData8Bit(DIG1, MarConLamp[29] + numdata[PowerNum]); // R20��
    }
    else if (Num >= 9 && Num <= 27)
    {
        HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20��
    }
    else
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + AppData + numdata[PowerNum]); // R20��
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + numdata[PowerNum]); // R20��
        }
    }
}
/*--------------------------------------------------------Э��2------------------------------------------------------------------------------------*/
/*Э��2���ѭ�������*/
void Circulation_Marquee_2(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) ||
         Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)) &&
        !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7))
    {
        mode = 1;
    }
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) &&
        !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        mode = 2;
    }
    /*��ع��ϡ���ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) ||
                                                                           Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)))
    {
        mode = 3;
    }
    /*�޹���״̬*/
    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        mode = 0;
    }
    /*----------------------------------------------�����--------------------------------------------*/
    if (Indata == 1)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 2)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[1]);
    }
    else if (Indata == 3)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[2]);
    }
    else if (Indata == 4)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[3]);
    }
    else if (Indata == 5)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[4]);
    }
    else if (Indata == 6)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[5]);
    }
    else if (Indata == 7)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[6]);
    }
    else if (Indata == 8)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[7]);
    }
    else if (Indata == 9)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, 0x00);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[4]);
    }
    else if (Indata == 10)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[3]);
    }
    else if (Indata == 11)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[2]);
    }
    else if (Indata == 12)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[1]);
    }
    else if (Indata == 13)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[0]);
    }
    else if (Indata == 14)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[5]);
    }
    else if (Indata == 15)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[6]);
    }
    else if (Indata == 16)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, 0x00);
    }
    else if (Indata == 17)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 18)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 19)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 20)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG1, Marqueedata[7] + numdata[Out_thousandplace]);
    }
    else if (Indata == 21)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG1, numdata[Out_thousandplace]);
        HT1621_WriteData8Bit(DIG5, Marqueedata[7] + numdata[in_thousandplace]);
    }
    else if (Indata == 22)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG5, numdata[in_thousandplace]);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[4]);
    }
    else if (Indata == 23)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[3]);
    }
    else if (Indata == 24)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[2]);
    }
    else if (Indata == 25)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[1]);
    }
    else if (Indata == 26)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, 0x00);
    }
    else if (Indata == 27)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 28)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 29)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 30)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else
    {
        Circulation_ErrorFlickerMode_2(1, mode);
    }
}

/*Э��2�����ϵ���˸����*/
void Circulation_ErrorFlickerMode_2(uint8_t Num, uint8_t Mode)
{
    if (Num == 1)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[0] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[0]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else if (Num == 16)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[4] + Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[4]);
        }
    }
    else if (Num == 17)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[3] + Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[3]);
        }
    }
    else if (Num == 18)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[2] + Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[2]);
        }
    }
    else if (Num == 19)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1]);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Marqueedata[1] + Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, Marqueedata[1]);
        }
    }
    else if (Num == 26)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[1] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[1]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else if (Num == 27)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[2] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[2]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else if (Num == 28)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[3] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[3]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else if (Num == 29)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[4] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[4]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else if (Num == 30)
    {
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Marqueedata[7] + Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, Marqueedata[7]);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
    else
    {
        /*������˸*/
        if (Mode == 1)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 2)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else if (Mode == 3)
        {
            if (ErrorFlickerFlag)
            {
                HT1621_WriteData8Bit(Ero_TR, 0x00);
                HT1621_WriteData8Bit(POWER_TR, 0X00);
                ErrorFlickerFlag = FALSE;
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, Numdata_T[0] + Numdata_T[1]);
                HT1621_WriteData8Bit(POWER_TR, Stadata_T[0]);
            }
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, 0x00);
            HT1621_WriteData8Bit(POWER_TR, 0x00);
        }
    }
}

/*Э��2�ŵ��������ʾ*/
void Discharge_Marquee_2(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1��˸ */
        DisSta_Conrtol_2(Electric, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*Э��2������ʾ״̬����*/
void DisSta_Conrtol_2(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*��˸����*/
    if (FlickerFlg)
    {
        TriggerFlickFlag = TRUE;
        FlickerFlg = FALSE;
    }
    else
    {
        TriggerFlickFlag = FALSE;
    }
    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) ||
         Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)) &&
        !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[1], Stadata_T[0], 1);
        }
    }
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) &&
        !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[0], Stadata_T[0], 1);
        }
    }
    /*��ع��ϡ���ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) ||
                                                                           Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[0] + Numdata_T[1], Stadata_T[0], 1);
        }
    }

    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        /* ��״̬���޹���״̬ */
        if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 10);
        }
        else
        {
            MarBrControl_2(Electric, TriggerFlickFlag, Num, PowerNum, 0, 0, 0);
        }
    }
}

/*Э��2 LED�ƿ���*/
void MarBrControl_2(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode)
{
    // R30-26�ƿ���
    if (FickerSta && Num < 5)
    {
        if (mode == 1)
        {
            if (Electric > 99)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData); // ��ӦR30����(��״̬����)
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num]); // ��ӦR30-26����
            }
        }
        else if (mode == 10 && Electric >= 97 && Electric < 99)
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0]); // ��ӦR30����(��״̬����)
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num]); // ��ӦR30-26����
           
        }
    }
    else
    {
        // ��ӦR30-26����
        if (Num > 0 && Num < 5)
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 5 && Num != 27)
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[4]);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[4]);
            }
        }
        else if (Num == 27)
        {
            HT1621_WriteData8Bit(Ero_TR, MarConLamp[Num]); // ��ӦR1����
        }
        else
        {
            if (mode == 1)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0]);
            }
        }
        // else
        // {
        //     HT1621_WriteData8Bit(Ero_TR, MarBright[0]);
        // }
    }
    // R25-22�ƿ���
    if (FickerSta && Num >= 5 && Num <= 8)
    {
        if (mode == 2)
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num]); // ��ӦR25-22����
        }
        else
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num]); // ��ӦR25-22����
        }
    }
    else
    {
        // ��ӦR25-22����
        if (Num > 5 && Num <= 8)
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num - 1]);
            }
        }
        else if (Num > 8)
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[8]);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[8]);
            }
        }
        else
        {
            if (mode == 2)
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(WIFI_TR, MarBright[3]);
            }
        }
    }
    // R19-16�ƿ���
    if (FickerSta && Num >= 9 && Num <= 12)
    {
        if (mode == 3 || mode == 8)
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num]); // ��ӦR19-16����
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num]); // ��ӦR19-16����
        }
    }
    else
    {
        // ��ӦR19-16����
        if (Num > 9 && Num <= 12)
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num - 1]);
            }
            else if (mode == 1)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData1 - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num - 1]);
            }
        }
        else if (Num > 12 && Num != 28 && Num != 29)
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[12]);
            }
            else if (mode == 1)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData1 - MarConLamp[12]);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[12]);
            }
        }
        else
        {
            if (mode == 3 || mode == 8)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData);
            }
            else if (mode == 1)
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData1);
            }
            else
            {
                HT1621_WriteData8Bit(POWER_TR, MarBright[3]);
            }
        }
    }
    // R15-9�ƿ���
    if (FickerSta && Num >= 13 && Num <= 19)
    {
        if (mode == 4)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num]); // ��ӦR15-9����
        }
        else if (mode == 8)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num]); // ��ӦR15-9����
        }
        else
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num]); // ��ӦR15-9����
        }
    }
    else
    {
        // ��ӦR15-9����
        if (Num > 13 && Num <= 19)
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num - 1]);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 19 && Num != 28 && Num != 29)
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[19]);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[19]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[19]);
            }
        }
        else
        {
            if (mode == 4)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData);
            }
            else if (mode == 8)
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_TR, MarBright[2]);
            }
        }
    }
    // R8-2�ƿ���
    if (FickerSta && Num >= 20 && Num <= 26)
    {
        if (mode == 5)
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num]); // ��ӦR8-2����
        }
        else
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num]); // ��ӦR8-2����
        }
    }
    else
    {
        // ��ӦR8-2����
        if (Num > 20 && Num <= 26)
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num - 1]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num - 1]);
            }
        }
        else if (Num >= 26 && Num != 28 && Num != 29)
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[26]);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[26]);
            }
        }
        else
        {
            if (mode == 5)
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData);
            }
            else
            {
                HT1621_WriteData8Bit(MAE_RR, MarBright[1]);
            }
        }
    }
    // R1�ƿ���
    if (FickerSta && Num == 27)
    {
        HT1621_WriteData8Bit(Ero_TR, 0x00); // ��ӦR1����
    }
    // R21�ƿ���
    if (FickerSta && Num == 28)
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, numdata[0] + AppData); // R21��
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[0]); // R21��
        }
    }
    else if (Num >= 0 && Num <= 8)
    {
        HT1621_WriteData8Bit(DIG5, MarConLamp[28] + numdata[0]); // R21��
    }
    else if ((Num >= 9 && Num <= 27) || Num == 29)
    {
        HT1621_WriteData8Bit(DIG5, numdata[0]); // R21��
    }
    else
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + AppData + numdata[0]); // R21��
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + numdata[0]); // R21��
        }
    }
    // R20�ƿ���
    if (FickerSta && Num == 29)
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum] + AppData); // R20��
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20��
        }
    }
    else if (Num >= 0 && Num <= 8 || Num == 28)
    {
        HT1621_WriteData8Bit(DIG1, MarConLamp[29] + numdata[PowerNum]); // R20��
    }
    else if (Num >= 9 && Num <= 27)
    {
        HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20��
    }
    else
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + AppData + numdata[PowerNum]); // R20��
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + numdata[PowerNum]); // R20��
        }
    }
}

/*--------------------------------------------------------Э��3------------------------------------------------------------------------------------*/
/*Э��3���ѭ�������*/
void Circulation_Marquee_3(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 4)) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 8) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 12))
    {
        mode = 1;
    }
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 8) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 12)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4))
    {
        mode = 2;
    }
    /*��ع��ϡ���ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 4)) && (Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 8) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 12)))
    {
        mode = 3;
    }
    /*�޹���״̬*/
    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 8) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 12))
    {
        mode = 0;
    }
    /*----------------------------------------------�����--------------------------------------------*/
    if (Indata == 1)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 2)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[1]);
    }
    else if (Indata == 3)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[2]);
    }
    else if (Indata == 4)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[3]);
    }
    else if (Indata == 5)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[4]);
    }
    else if (Indata == 6)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[5]);
    }
    else if (Indata == 7)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[6]);
    }
    else if (Indata == 8)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, Marqueedata[7]);
    }
    else if (Indata == 9)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_RR, 0x00);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[4]);
    }
    else if (Indata == 10)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[3]);
    }
    else if (Indata == 11)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[2]);
    }
    else if (Indata == 12)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[1]);
    }
    else if (Indata == 13)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[0]);
    }
    else if (Indata == 14)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[5]);
    }
    else if (Indata == 15)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, Marqueedata[6]);
    }
    else if (Indata == 16)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(MAE_TR, 0x00);
    }
    else if (Indata == 17)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 18)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 19)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 20)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG1, Marqueedata[7] + numdata[Out_thousandplace]);
    }
    else if (Indata == 21)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG1, numdata[Out_thousandplace]);
        HT1621_WriteData8Bit(DIG5, Marqueedata[7] + numdata[in_thousandplace]);
    }
    else if (Indata == 22)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(DIG5, numdata[in_thousandplace]);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[4]);
    }
    else if (Indata == 23)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[3]);
    }
    else if (Indata == 24)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[2]);
    }
    else if (Indata == 25)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, Marqueedata[1]);
    }
    else if (Indata == 26)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
        HT1621_WriteData8Bit(WIFI_TR, 0x00);
    }
    else if (Indata == 27)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 28)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 29)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else if (Indata == 30)
    {
        Circulation_ErrorFlickerMode_2(Indata, mode);
    }
    else
    {
        Circulation_ErrorFlickerMode_2(1, mode);
    }
}

/*Э��3�ŵ��������ʾ*/
void Discharge_Marquee_3(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1��˸ */
        DisSta_Conrtol_3(Electric, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*Э��3������ʾ״̬����*/
void DisSta_Conrtol_3(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*��˸����*/
    if (FlickerFlg)
    {
        TriggerFlickFlag = TRUE;
        FlickerFlg = FALSE;
    }
    else
    {
        TriggerFlickFlag = FALSE;
    }
    /*----------------------------------Error����ʾ----------------------------------------------------*/
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 5)) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 13))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[1], Stadata_T[0], 1);
        }
    }
    /*��ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 7) || Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 13)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 5))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[0], Stadata_T[0], 1);
        }
    }
    /*��ع��ϡ���ع�����ʾ*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 5)) &&
        (Read_WordManage(ErrorSat, 7) || Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 13)))
    {
        if (ErrorFlickerFlag && !SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
            ErrorFlickerFlag = FALSE;
        }
        else if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 1);
        }
        else
        {
            MarBrControl_2(Electric, FALSE, Num, PowerNum, Numdata_T[0] + Numdata_T[1], Stadata_T[0], 1);
        }
    }

    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 13))
    {
        /* ��״̬���޹���״̬ */
        if (SteadySta)
        {
            MarBrControl_2(Electric, TRUE, Num, PowerNum, 0, 0, 10);
        }
        else
        {
            MarBrControl_2(Electric, TriggerFlickFlag, Num, PowerNum, 0, 0, 0);
        }
    }
}
