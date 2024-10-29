/**
 ******************************************************************************
 * 文件名程: Mannger_LCD.c
 * 作    者: xiao
 * 版    本: V1.0
 * 编写日期: 2022-06-28
 * 功    能: 管理外设LCD
 ******************************************************************************
 */

#include "Manager.h"

/*私有宏定义--------------------------------------------------------------------------*/
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
/*私有变量----------------------------------------------------------------------------*/
//  数码1-8            0    1     2    3    4    5    6    7   8    9
uint8_t numdata[] = {0x5f, 0x06, 0x3d, 0x2f, 0x66, 0x6b, 0x7b, 0x0e, 0x7f, 0x6f}; //"0~9"char
//   数码9-10              0    1     2    3    4    5    6    7   8    9
uint8_t numdata1[] = {0xf5, 0x60, 0xb6, 0xf2, 0x63, 0xd3, 0xd7, 0x70, 0xf7, 0xf3}; //"0~9"char
//  跑马灯R1 - R30         "R1/R13"、"R2/R12/R19/R25/R26"、"R3/R11/R18/R24/R27"、"R4/R10/R17/R23/R28"、"R5/R9/R16/R22/R29"、"R6/R14"、"R7/R15"、"R8/R20/R21/R30"
uint8_t Marqueedata[] = {0x10, 0x01, 0x02, 0x04, 0x08, 0x20, 0x40, 0x80};
//  跑马灯  “R30-26、R1”、“R25-22”、“R19-16”、“R15-9”、“R8-2”
uint8_t MarBright[] = {0x9f, 0xef, 0x7f, 0x0f};
// 跑马灯单体控制                              4Bit/0x8f:R30 - R26全灭   8bit/0x0f：R25-22  12bit/0x0f:R19-16全灭  19Bit/0x7F:R15-9全灭  26Bit/0xef：R8-2全灭
uint8_t MarConLamp[] = {0x80, 0x88, 0x8c, 0x8e, 0x8f, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x03, 0x07, 0x0f, 0x40, 0x60, 0x70, 0x71, 0x73, 0x77, 0x7f, 0x80, 0xc0, 0xe0, 0xe8, 0xec, 0xee, 0xef, 0x10, 0x80, 0x80};
/*符号显示T*/
uint8_t Numdata_T[] = {0x40, 0x20, 0x08, 0x80, 0x60}; //"T1"、"T2"、"T3/T4"、"T9/T10/T11/T12/T13/T14/T18"、“T1/T2”
/*状态显示T*/
uint8_t Stadata_T[5] = {0x10, 0x20, 0x40, 0x80, 0xe0};                                 // "T5/T15"、"T6/T16"、"T7/T17"、"T8/T19"、"T16/T17/T18"
uint8_t numadd[] = {DIG0, DIG1, DIG2, DIG3, DIG4, DIG5, DIG6, DIG7, DIG8, DIG9, DIGA}; //"dig"char

uint8_t disp_buff_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Display_Fag = 0, Fixed_Value = 0x90, RestartTime = 0;
uint8_t Count = 0, Dis_InitFlag = 0, Char_InitFlag = 0, Await_InitFlag = 0, TriggerFlickFlag = 0;
uint32_t Charge_Time = 0;
uint16_t Show_InputPower = 0, Show_OutputPower = 0, Show_Voltage = 0, Judge_Electricity = 0, Use_ReportSta = 0, Dormancy_Electricity = 0, Use_RunSta = 0;

/*外部引用变量------------------------------------------------------------------------*/
extern uint16_t FlickerFlg, Cir_FlickerFlag;
extern uint16_t ErrorFlickerFlag;
extern uint8_t Deal_TriggerMode1, Deal_TriggerMode2, Deal_TriggerMode3;
extern uint16_t DormancySta;
/*私有函数原型------------------------------------------------------------------------*/
void Manager_LCD_Init(void);
void Manager_LCD_Mainloop(void);

void LightenLCD_Init(void);
void Init_Interface(void);
void Display_Frame(void);
// 协议1
void Deal_Battery_Power_Show_1(void);
void Circulation_ErrorFlickerMode_1(uint8_t Num, uint8_t Mode);
void Circulation_Marquee_1(uint8_t Indata, uint8_t PowerSta,uint8_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
void MarBrControl_1(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode);
// 协议2
void Deal_Battery_Power_Show_2(void);
void Circulation_ErrorFlickerMode_2(uint8_t Num, uint8_t Mode);
void Circulation_Marquee_2(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_2(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_2(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
void MarBrControl_2(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode);
// 协议3
void Deal_Battery_Power_Show_3(void);
void Circulation_Marquee_3(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower);
void Discharge_Marquee_3(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta);
void DisSta_Conrtol_3(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta);
/*初始化*/
void Manager_LCD_Init(void)
{
    HT1621_Init();
    LightenLCD_Init();
    Init_Interface();
}
/*主loop函数*/
void Manager_LCD_Mainloop(void)
{
    Display_Frame();
}
// 输出功率 个十百千位显示
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
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace] + Marqueedata[7]); // 千位
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace]); // 千位
        }
    }
    else
    {
        /*电量小于62% R20灯处理*/
        if (power <= 62)
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace]); // 千位
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[thousandplace] + Marqueedata[7]); // 千位
        }
    }
    if (mode)
    {
        /* 显示电压符号 */
        HT1621_WriteData8Bit(DIG2, numdata[hunderdplace] + Numdata_T[3]); // 百位
        HT1621_WriteData8Bit(DIG3, numdata[tenplace] + Numdata_T[3]);     // 十位
        HT1621_WriteData8Bit(DIG4, numdata[unitplace]);                   // 个位
    }
    else
    {
        /* 显示功率符号*/
        HT1621_WriteData8Bit(DIG2, numdata[hunderdplace] + Numdata_T[3]); // 百位
        HT1621_WriteData8Bit(DIG3, numdata[tenplace]);                    // 十位
        HT1621_WriteData8Bit(DIG4, numdata[unitplace] + Numdata_T[3]);    // 个位
    }
}
/*输入功率显示换算*/
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
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace] + Marqueedata[7]); // 千位
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace]); // 千位
        }
    }
    else
    {
        /*电量小于66% R21灯处理*/
        if (power <= 65)
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace]); // 千位
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[thousandplace] + Marqueedata[7]); // 千位
        }
    }
    if (mode)
    {
        /* 显示电压符号 */
        HT1621_WriteData8Bit(DIG6, numdata[hunderdplace] + Numdata_T[3]); // 百位
        HT1621_WriteData8Bit(DIG7, numdata[tenplace] + Numdata_T[3]);     // 十位
        HT1621_WriteData8Bit(DIG8, numdata[unitplace]);                   // 个位
    }
    else
    {
        /* 显示功率符号*/
        HT1621_WriteData8Bit(DIG6, numdata[hunderdplace] + Numdata_T[3]); // 百位
        HT1621_WriteData8Bit(DIG7, numdata[tenplace]);                    // 十位
        HT1621_WriteData8Bit(DIG8, numdata[unitplace] + Numdata_T[3]);    // 个位
    }
}
/*电池电量处理及显示*/
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

/*充电显示*/
void Charge_Dispose(uint16_t ErrorSta, unsigned long InputPower, unsigned long OutputPower, uint8_t mode, uint8_t Deal_Num)
{
    /*跑马灯运行*/
    if (Cir_FlickerFlag)
    {
        Count++;
        Cir_FlickerFlag = FALSE;
    }
    if (Count == 31)
    {
        Count = FALSE;
    }
    /*输入功率显示*/
    Input_Power(InputPower, Count, 1, mode);
    /*输出功率显示*/
    Output_Power(OutputPower, Count, 1, mode);
    /*充电跑马灯*/
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
/*显示功能界面*/
void Display_Frame(void)
{
    if (Deal_TriggerMode1)
    {
        /*协议1模式*/
        /*输入输出功率*/
        Show_InputPower = (uint16_t)(Deal_Buf1.InputPower_SetValue[0] << 8 | Deal_Buf1.InputPower_SetValue[1]);
        Show_OutputPower = (uint16_t)(Deal_Buf1.OutputPower_SetValue[0] << 8 | Deal_Buf1.OutputPower_SetValue[1]);
        /*电量及输入功率显示*/
        Deal_Battery_Power_Show_1();
    }
    else if (Deal_TriggerMode2)
    {
        /*协议2模式*/
        Show_Voltage = (uint16_t)(Deal_Buf2.Total_Voltage[0] << 8 | Deal_Buf2.Total_Voltage[1]); // 总电压
        Show_Voltage = Show_Voltage / 100;
        Judge_Electricity = (uint16_t)(Deal_Buf2.Electricity[0] << 8 | Deal_Buf2.Electricity[1]); // 判断电流方向
        Use_ReportSta = (uint16_t)(Deal_Buf2.ReportSta[0] << 8 | Deal_Buf2.ReportSta[1]);         // 告警状态组码

        /*电量及功率显示*/
        Deal_Battery_Power_Show_2();
    }
    else if (Deal_TriggerMode3)
    {
        /*协议3模式*/
        Use_RunSta = Deal_Buf3.System_Sta;             // 系统状态
        Show_Voltage = Deal_Buf3.Gather_Total_Voltage; // 总电压
        Show_Voltage = Show_Voltage / 10;
        Dormancy_Electricity = (Symbol_Compute - Deal_Buf3.Total_Electricity[1]); // 休眠电流
        Use_ReportSta = Deal_Buf3.Level_1_Protect_Sta[1];                         // 保护状态低字节

        /*电量及功率显示*/
        Deal_Battery_Power_Show_3();
    }
}

/*协议1电量及电池功率显示*/
void Deal_Battery_Power_Show_1(void)
{

    /*放电状态*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*输入功率显示*/
        Input_Power(Show_InputPower, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*输出功率显示*/
        Output_Power(Show_OutputPower, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*放电模式*/
        Discharge_Marquee_1(Deal_Buf1.BatterySOC_SetValue, Deal_Buf1.Trigger_OrderSta, Deal_Buf1.Trigger_ErrorCode, Show_OutputPower, FALSE);
    }
    /*充电状态*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0))
    {
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*充电显示*/
        Charge_Dispose(Deal_Buf1.Trigger_ErrorCode, Show_InputPower, Show_OutputPower, FALSE, 1);
    }
    /*开机状态*/
    if (Read_WordManage(Deal_Buf1.Trigger_OrderSta, 6) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf1.BatterySOC_SetValue);
        /*输入功率显示*/
        Input_Power(0, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*输出功率显示*/
        Output_Power(0, Deal_Buf1.BatterySOC_SetValue, 0, FALSE);
        /*放电模式*/
        Discharge_Marquee_1(Deal_Buf1.BatterySOC_SetValue, Deal_Buf1.Trigger_OrderSta, Deal_Buf1.Trigger_ErrorCode, 0, TRUE);
    }

    if (!Read_WordManage(Deal_Buf1.Trigger_OrderSta, 6) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 0) && !Read_WordManage(Deal_Buf1.Trigger_OrderSta, 1))
    {
        Init_Interface();
        /*关背光*/
        SuperGpio_WriteOutput(0, 0);
    }
}
/*协议2电量及电池功率显示*/
void Deal_Battery_Power_Show_2(void)
{

    /*放电状态*/
    if (Read_WordManage(Judge_Electricity, 15) && (Symbol_Compute - Judge_Electricity > Min_Electricity))
    {
        /*关闭休眠*/
        DormancySta = FALSE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*输入功率显示*/
        Input_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*输出功率显示*/
        Output_Power(Show_Voltage, Deal_Buf2.RSOC, 0, TRUE);
        /*放电模式*/
        Discharge_Marquee_2(Deal_Buf2.RSOC, Use_ReportSta, Show_Voltage, FALSE);
    }
    /*充电状态*/
    if (!Read_WordManage(Judge_Electricity, 15) && Judge_Electricity > Min_Electricity)
    {
        /*关闭休眠*/
        DormancySta = FALSE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*充电显示*/
        Charge_Dispose(Use_ReportSta, Show_Voltage, 0, TRUE, 2);
    }
    /*唤醒状态*/
    if ((Judge_Electricity == 0 || (Judge_Electricity > 0 && (Symbol_Compute - Judge_Electricity <= Min_Electricity)) || (Judge_Electricity > 0 && Judge_Electricity <= Min_Electricity)) && !DormancySta)
    {
        /*重新计时*/
        RestartTime = TRUE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf2.RSOC);
        /*输入功率显示*/
        Input_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*输出功率显示*/
        Output_Power(0, Deal_Buf2.RSOC, 0, TRUE);
        /*放电模式*/
        Discharge_Marquee_2(Deal_Buf2.RSOC, Use_ReportSta, Show_Voltage, TRUE);
    }
    /*3分钟进入休眠状态*/
    if (DormancySta)
    {
        Init_Interface();
        /*关背光*/
        SuperGpio_WriteOutput(0, 0);
    }
    /*小于1A电流唤醒休眠*/
    if ((Judge_Electricity > 0 && (Symbol_Compute - Judge_Electricity <= Min_Electricity)) ||
        (Judge_Electricity > 0 && Judge_Electricity <= Min_Electricity))
    {
        DormancySta = FALSE;
    }
}
/*协议3电量及电池功率显示*/
void Deal_Battery_Power_Show_3(void)
{

    /*放电状态*/
    if (Use_RunSta == 0x03 && (Dormancy_Electricity > Min_Electricity1))
    {
        /*关闭休眠*/
        DormancySta = FALSE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Dis_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Dis_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*输入功率显示*/
        Input_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*输出功率显示*/
        Output_Power(Show_Voltage, Deal_Buf3.SOC, 0, TRUE);
        /*放电模式*/
        Discharge_Marquee_3(Deal_Buf3.SOC, Use_ReportSta, Show_Voltage, FALSE);
    }
    /*充电状态*/
    if (Use_RunSta == 0x02 && (Dormancy_Electricity > Min_Electricity1))
    {
        /*关闭休眠*/
        DormancySta = FALSE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Char_InitFlag)
        {
            Init_Interface();
            Dis_InitFlag = FALSE;
            Await_InitFlag = FALSE;
            Char_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*充电显示*/
        Charge_Dispose(Use_ReportSta, Show_Voltage, 0, TRUE, 3);
    }
    /*唤醒状态*/
    if ((Use_RunSta == 0x01 || Use_RunSta == 0x02 || Use_RunSta == 0x03) &&
        (Dormancy_Electricity <= Min_Electricity1) && !DormancySta)
    {
        /*重新计时*/
        RestartTime = TRUE;
        /*开背光*/
        SuperGpio_WriteOutput(0, 1);
        /*初始化显示*/
        if (!Await_InitFlag)
        {
            Init_Interface();
            Char_InitFlag = FALSE;
            Dis_InitFlag = FALSE;
            Await_InitFlag = TRUE;
        }
        /*电池电量显示%*/
        Battery_Dispose(Deal_Buf3.SOC);
        /*输入功率显示*/
        Input_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*输出功率显示*/
        Output_Power(0, Deal_Buf3.SOC, 0, TRUE);
        /*放电模式*/
        Discharge_Marquee_3(Deal_Buf3.SOC, Use_ReportSta, Show_Voltage, TRUE);
    }
    /*3分钟进入休眠状态*/
    if (DormancySta)
    {
        Init_Interface();
        /*关背光*/
        SuperGpio_WriteOutput(0, 0);
    }
    /*小于0.5A电流唤醒休眠*/
    if (Dormancy_Electricity > 0 && (Dormancy_Electricity <= Min_Electricity1))
    {
        DormancySta = FALSE;
    }
}
/*初始化LCD屏全显示*/
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
/*显示初始界面*/
void Init_Interface(void)
{
    for (uint8_t i = 0; i < 120; i++)
    {
        HT1621_WriteData8Bit(i, 0x00); // clear all display
    }
}

/*---------------------------------------------------------------------------------协议1-----------------------------------------------------------------------------*/
/*协议1充电循环跑马灯*/
void Circulation_Marquee_1(uint8_t Indata, uint8_t PowerSta,uint8_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)) && !Read_WordManage(ErrorSat, 7))
    {
        mode = 1;
    }
    /*电池过温显示*/
    if (Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 2;
    }
    /*电池故障、电池过温显示*/
    if (Read_WordManage(ErrorSat, 7) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2)))
    {
        mode = 3;
    }
    /*PV过载显示*/
    if (Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) 
    && !Read_WordManage(ErrorSat, 2))
    {
        mode = 4;
    }
    
    /*---------------------------------功能触发显示-----------------------------------------------------*/
    if (Read_WordManage(PowerSta, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) 
    && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 5;
    }
    
    /*无故障状态*/
    if (!Read_WordManage(PowerSta, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) 
    && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        mode = 0;
    }
    /*----------------------------------------------跑马灯--------------------------------------------*/
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

/*协议1充电故障灯闪烁处理*/
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
        /*故障闪烁*/
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

/*协议1放电跑马灯显示*/
void Discharge_Marquee_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1闪烁 */
        DisSta_Conrtol_1(Electric, PowerSta, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*协议1功能显示状态控制*/
void DisSta_Conrtol_1(unsigned long Electric, uint8_t PowerSta, uint8_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*闪烁条件*/
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
        /*PV 显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1], 0, 3);
    }
    if (Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC 显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2], 0, 3);
    }
    if (Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*USB 显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Numdata_T[3], 0, 4);
    }
    if (Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*AC 显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、DC显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、USB显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、AC显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC、USB显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 4) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC、AC显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*USB、AC显示*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[3], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && !Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、DC、USB*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && !Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、DC、AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], 0, 3);
    }
    if (Read_WordManage(PowerSta, 2) && !Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*PV、USB、AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[3], Numdata_T[3], 8);
    }
    if (!Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /*DC、USB、AC*/
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
    }
    if (Read_WordManage(PowerSta, 2) && Read_WordManage(PowerSta, 3) && Read_WordManage(PowerSta, 4) && Read_WordManage(PowerSta, 5) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 5) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2))
    {
        /* PV、DC、USB、AC */
        MarBrControl_1(Electric, TriggerFlickFlag, Num, PowerNum, Stadata_T[1] + Stadata_T[2] + Stadata_T[3], Numdata_T[3], 8);
    }
    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
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
    /*PV过载显示*/
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
    /*DC过载显示*/
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
    /*USB过载显示*/
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
    /*AC过载显示*/
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
    /*PV、DC过载显示*/
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
    /*PV、USB过载显示*/
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
    /*PV、AC过载显示*/
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
    /*USB、DC过载显示*/
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
    /*USB、AC过载显示*/
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
    /*DC、AC过载显示*/
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
    /*PV、DC、AC过载显示*/
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
    /*PV、DC、USB过载显示*/
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
    /*PV、AC、USB过载显示*/
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
    /*DC、AC、USB过载显示*/
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
    /*PV、DC、USB、AC过载显示*/
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
    /*电池过温显示*/
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
    /*电池故障、电池过温显示*/
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
        /* 无状态、无故障状态 */
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

/*协议1LED灯控制*/
void MarBrControl_1(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode)
{
    // R30-26灯控制
    if (FickerSta && Num < 5)
    {
        if (mode == 1)
        {
            if (Electric > 99)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData); //  对应R30灯亮(休眠)
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num]); // 对应R30-26灯亮
            }
        }
        else if (mode == 10 && Electric >= 97 && Electric < 99)
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0]); // 对应R30灯亮(休眠)
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num]); // 对应R30-26灯亮
        }
    }
    else
    {
        // 对应R30-26灯灭
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
            HT1621_WriteData8Bit(Ero_TR, MarConLamp[Num]); // 对应R1灯亮
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
    // R25-22灯控制
    if (FickerSta && Num >= 5 && Num <= 8)
    {
        if (mode == 2)
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num]); // 对应R25-22灯亮
        }
        else
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num]); // 对应R25-22灯亮
        }
    }
    else
    {
        // 对应R25-22灯灭
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
    // R19-16灯控制
    if (FickerSta && Num >= 9 && Num <= 12)
    {
        if (mode == 3 || mode == 8)
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num]); // 对应R19-16灯亮
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num]); // 对应R19-16灯亮
        }
    }
    else
    {
        // 对应R19-16灯灭
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
    // R15-9灯控制
    if (FickerSta && Num >= 13 && Num <= 19)
    {
        if (mode == 4)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num]); // 对应R15-9灯亮
        }
        else if (mode == 8)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num]); // 对应R15-9灯亮
        }
        else
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num]); // 对应R15-9灯亮
        }
    }
    else
    {
        // 对应R15-9灯灭
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
    // R8-2灯控制
    if (FickerSta && Num >= 20 && Num <= 26)
    {
        if (mode == 5)
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num]); // 对应R8-2灯亮
        }
        else
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num]); // 对应R8-2灯亮
        }
    }
    else
    {
        // 对应R8-2灯灭
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
    // R1灯控制
    if (FickerSta && Num == 27)
    {
        HT1621_WriteData8Bit(Ero_TR, 0x00); // 对应R1灯灭
    }
    // R21灯控制
    if (FickerSta && Num == 28)
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, numdata[0] + AppData); // R21灭
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[0]); // R21灭
        }
    }
    else if (Num >= 0 && Num <= 8)
    {
        HT1621_WriteData8Bit(DIG5, MarConLamp[28] + numdata[0]); // R21亮
    }
    else if ((Num >= 9 && Num <= 27) || Num == 29)
    {
        HT1621_WriteData8Bit(DIG5, numdata[0]); // R21灭
    }
    else
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + AppData + numdata[0]); // R21亮
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + numdata[0]); // R21亮
        }
    }
    // R20灯控制
    if (FickerSta && Num == 29)
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum] + AppData); // R20灭
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20灭
        }
    }
    else if (Num >= 0 && Num <= 8 || Num == 28)
    {
        HT1621_WriteData8Bit(DIG1, MarConLamp[29] + numdata[PowerNum]); // R20亮
    }
    else if (Num >= 9 && Num <= 27)
    {
        HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20灭
    }
    else
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + AppData + numdata[PowerNum]); // R20亮
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + numdata[PowerNum]); // R20亮
        }
    }
}
/*--------------------------------------------------------协议2------------------------------------------------------------------------------------*/
/*协议2充电循环跑马灯*/
void Circulation_Marquee_2(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) ||
         Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)) &&
        !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7))
    {
        mode = 1;
    }
    /*电池过温显示*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) &&
        !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        mode = 2;
    }
    /*电池故障、电池过温显示*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 7)) && (Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) ||
                                                                           Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 9) || Read_WordManage(ErrorSat, 10)))
    {
        mode = 3;
    }
    /*无故障状态*/
    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 7) && !Read_WordManage(ErrorSat, 9) && !Read_WordManage(ErrorSat, 10))
    {
        mode = 0;
    }
    /*----------------------------------------------跑马灯--------------------------------------------*/
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

/*协议2充电故障灯闪烁处理*/
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
        /*故障闪烁*/
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

/*协议2放电跑马灯显示*/
void Discharge_Marquee_2(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1闪烁 */
        DisSta_Conrtol_2(Electric, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*协议2功能显示状态控制*/
void DisSta_Conrtol_2(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*闪烁条件*/
    if (FlickerFlg)
    {
        TriggerFlickFlag = TRUE;
        FlickerFlg = FALSE;
    }
    else
    {
        TriggerFlickFlag = FALSE;
    }
    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
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
    /*电池过温显示*/
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
    /*电池故障、电池过温显示*/
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
        /* 无状态、无故障状态 */
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

/*协议2 LED灯控制*/
void MarBrControl_2(unsigned long Electric, uint8_t FickerSta, uint8_t Num, uint8_t PowerNum, uint8_t AppData, uint8_t AppData1, uint8_t mode)
{
    // R30-26灯控制
    if (FickerSta && Num < 5)
    {
        if (mode == 1)
        {
            if (Electric > 99)
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData); // 对应R30灯亮(无状态休眠)
            }
            else
            {
                HT1621_WriteData8Bit(Ero_TR, MarBright[0] + AppData - MarConLamp[Num]); // 对应R30-26灯亮
            }
        }
        else if (mode == 10 && Electric >= 97 && Electric < 99)
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0]); // 对应R30灯亮(无状态休眠)
        }
        else
        {
            HT1621_WriteData8Bit(Ero_TR, MarBright[0] - MarConLamp[Num]); // 对应R30-26灯亮
           
        }
    }
    else
    {
        // 对应R30-26灯灭
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
            HT1621_WriteData8Bit(Ero_TR, MarConLamp[Num]); // 对应R1灯亮
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
    // R25-22灯控制
    if (FickerSta && Num >= 5 && Num <= 8)
    {
        if (mode == 2)
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] + AppData - MarConLamp[Num]); // 对应R25-22灯亮
        }
        else
        {
            HT1621_WriteData8Bit(WIFI_TR, MarBright[3] - MarConLamp[Num]); // 对应R25-22灯亮
        }
    }
    else
    {
        // 对应R25-22灯灭
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
    // R19-16灯控制
    if (FickerSta && Num >= 9 && Num <= 12)
    {
        if (mode == 3 || mode == 8)
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] + AppData - MarConLamp[Num]); // 对应R19-16灯亮
        }
        else
        {
            HT1621_WriteData8Bit(POWER_TR, MarBright[3] - MarConLamp[Num]); // 对应R19-16灯亮
        }
    }
    else
    {
        // 对应R19-16灯灭
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
    // R15-9灯控制
    if (FickerSta && Num >= 13 && Num <= 19)
    {
        if (mode == 4)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData - MarConLamp[Num]); // 对应R15-9灯亮
        }
        else if (mode == 8)
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] + AppData1 - MarConLamp[Num]); // 对应R15-9灯亮
        }
        else
        {
            HT1621_WriteData8Bit(MAE_TR, MarBright[2] - MarConLamp[Num]); // 对应R15-9灯亮
        }
    }
    else
    {
        // 对应R15-9灯灭
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
    // R8-2灯控制
    if (FickerSta && Num >= 20 && Num <= 26)
    {
        if (mode == 5)
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] + AppData - MarConLamp[Num]); // 对应R8-2灯亮
        }
        else
        {
            HT1621_WriteData8Bit(MAE_RR, MarBright[1] - MarConLamp[Num]); // 对应R8-2灯亮
        }
    }
    else
    {
        // 对应R8-2灯灭
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
    // R1灯控制
    if (FickerSta && Num == 27)
    {
        HT1621_WriteData8Bit(Ero_TR, 0x00); // 对应R1灯灭
    }
    // R21灯控制
    if (FickerSta && Num == 28)
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, numdata[0] + AppData); // R21灭
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, numdata[0]); // R21灭
        }
    }
    else if (Num >= 0 && Num <= 8)
    {
        HT1621_WriteData8Bit(DIG5, MarConLamp[28] + numdata[0]); // R21亮
    }
    else if ((Num >= 9 && Num <= 27) || Num == 29)
    {
        HT1621_WriteData8Bit(DIG5, numdata[0]); // R21灭
    }
    else
    {
        if (mode == 6)
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + AppData + numdata[0]); // R21亮
        }
        else
        {
            HT1621_WriteData8Bit(DIG5, MarConLamp[Num] + numdata[0]); // R21亮
        }
    }
    // R20灯控制
    if (FickerSta && Num == 29)
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum] + AppData); // R20灭
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20灭
        }
    }
    else if (Num >= 0 && Num <= 8 || Num == 28)
    {
        HT1621_WriteData8Bit(DIG1, MarConLamp[29] + numdata[PowerNum]); // R20亮
    }
    else if (Num >= 9 && Num <= 27)
    {
        HT1621_WriteData8Bit(DIG1, numdata[PowerNum]); // R20灭
    }
    else
    {
        if (mode == 7)
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + AppData + numdata[PowerNum]); // R20亮
        }
        else
        {
            HT1621_WriteData8Bit(DIG1, MarConLamp[Num] + numdata[PowerNum]); // R20亮
        }
    }
}

/*--------------------------------------------------------协议3------------------------------------------------------------------------------------*/
/*协议3充电循环跑马灯*/
void Circulation_Marquee_3(uint8_t Indata, uint16_t ErrorSat, unsigned long Inputpower, unsigned long Outputpower)
{
    int in_thousandplace = Inputpower / 1000 % 10;
    int Out_thousandplace = Outputpower / 1000 % 10;
    uint8_t mode = 0;

    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 4)) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 8) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 12))
    {
        mode = 1;
    }
    /*电池过温显示*/
    if ((Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 8) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 12)) && !Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4))
    {
        mode = 2;
    }
    /*电池故障、电池过温显示*/
    if ((Read_WordManage(ErrorSat, 0) || Read_WordManage(ErrorSat, 1) || Read_WordManage(ErrorSat, 2) || Read_WordManage(ErrorSat, 3) || Read_WordManage(ErrorSat, 4)) && (Read_WordManage(ErrorSat, 6) || Read_WordManage(ErrorSat, 8) || Read_WordManage(ErrorSat, 10) || Read_WordManage(ErrorSat, 11) || Read_WordManage(ErrorSat, 12)))
    {
        mode = 3;
    }
    /*无故障状态*/
    if (!Read_WordManage(ErrorSat, 0) && !Read_WordManage(ErrorSat, 1) && !Read_WordManage(ErrorSat, 2) && !Read_WordManage(ErrorSat, 3) && !Read_WordManage(ErrorSat, 4) && !Read_WordManage(ErrorSat, 6) && !Read_WordManage(ErrorSat, 8) && !Read_WordManage(ErrorSat, 10) && !Read_WordManage(ErrorSat, 11) && !Read_WordManage(ErrorSat, 12))
    {
        mode = 0;
    }
    /*----------------------------------------------跑马灯--------------------------------------------*/
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

/*协议3放电跑马灯显示*/
void Discharge_Marquee_3(unsigned long Electric, uint16_t ErrorSta, unsigned long Outputpower, uint8_t SteadySta)
{
    int Out_thousandplace = Outputpower / 1000 % 10;

    if (Electric >= 95)
    {
        /* R30闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 0, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 92 && Electric < 95)
    {
        /* R29闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 1, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 89 && Electric < 92)
    {
        /* R28闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 2, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 85 && Electric < 89)
    {
        /* R27闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 3, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 82 && Electric < 85)
    {
        /* R26闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 4, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 79 && Electric < 82)
    {
        /* R25闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 5, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 75 && Electric < 79)
    {
        /* R24闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 6, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 72 && Electric < 75)
    {
        /* R23闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 7, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 69 && Electric < 72)
    {
        /* R22闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 8, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 66 && Electric < 69)
    {
        /* R21闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 28, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 62 && Electric < 66)
    {
        /* R20闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 29, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 59 && Electric < 62)
    {
        /* R19闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 9, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 56 && Electric < 59)
    {
        /* R18闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 10, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 52 && Electric < 56)
    {
        /* R17闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 11, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 49 && Electric < 52)
    {
        /* R16闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 12, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 46 && Electric < 49)
    {
        /* R15闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 13, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 42 && Electric < 46)
    {
        /* R14闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 14, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 39 && Electric < 42)
    {
        /* R13闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 15, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 36 && Electric < 39)
    {
        /* R12闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 16, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 33 && Electric < 36)
    {
        /* R11闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 17, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 29 && Electric < 33)
    {
        /* R10闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 18, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 26 && Electric < 29)
    {
        /* R9闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 19, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 23 && Electric < 26)
    {
        /* R8闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 20, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 19 && Electric < 23)
    {
        /* R7闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 21, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 16 && Electric < 19)
    {
        /* R6闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 22, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 13 && Electric < 16)
    {
        /* R5闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 23, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 9 && Electric < 13)
    {
        /* R4闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 24, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 6 && Electric < 9)
    {
        /* R3闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 25, Out_thousandplace, SteadySta);
    }
    else if (Electric >= 3 && Electric < 6)
    {
        /* R2闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 26, Out_thousandplace, SteadySta);
    }
    else if (Electric < 3)
    {
        /* R1闪烁 */
        DisSta_Conrtol_3(Electric, ErrorSta, 27, Out_thousandplace, SteadySta);
    }
}

/*协议3功能显示状态控制*/
void DisSta_Conrtol_3(unsigned long Electric, uint16_t ErrorSat, uint8_t Num, uint8_t PowerNum, uint8_t SteadySta)
{
    /*闪烁条件*/
    if (FlickerFlg)
    {
        TriggerFlickFlag = TRUE;
        FlickerFlg = FALSE;
    }
    else
    {
        TriggerFlickFlag = FALSE;
    }
    /*----------------------------------Error码显示----------------------------------------------------*/
    /*电池故障显示*/
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
    /*电池过温显示*/
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
    /*电池故障、电池过温显示*/
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
        /* 无状态、无故障状态 */
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
