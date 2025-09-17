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

/*私有宏定义-----------------------------------------------------------------------*/
#define FALSE 0
#define TRUE 1
#define Deal_Len1 18 // 协议1数据长度
#define Deal_Len2 50 // 协议2数据长度
#define Deal_Len3 100          // 协议3数据长度
#define TestPowerSta    0x3c
#define TestErrorSta    0x83
#define Symbol_Compute 0x10000  //电流正负号
#define Min_Electricity 0x0064  //最小电流
#define Min_Electricity1 0x0032 // 休眠最小电流

/*外部引用*/
extern uint16_t Heartbeat;

typedef struct
{
    /*写入寄存器数据*/
    uint8_t Heartcount[2];          // 0心跳包
    uint8_t Panel_CurrentSta;       // 面板当前状态
    uint8_t Current_BatterySOC;     // 当前电池SOC
    uint8_t Current_InpuPower[2];   // 当前输入功率
    uint8_t Current_OutputPower[2]; // 当前输出功率
    uint8_t Panel_ErrorCode;        // 面板故障码
    uint8_t WriteStandby1;          // 写入预留
    uint8_t WriteStandby2;          // 写入预留
    uint8_t WriteStandby3;          // 写入预留
    uint8_t WriteStandby4;          // 写入预留
    uint8_t WriteStandby5;          // 13 写入预留

    /*读取寄存器数据*/
    uint8_t Trigger_OrderSta;        // 14 触发命令状态
    uint8_t BatterySOC_SetValue;     // 电池SOC设定值
    uint8_t InputPower_SetValue[2];  // 输入功率设定值
    uint8_t OutputPower_SetValue[2]; // 输出功率设定值
    uint8_t Trigger_ErrorCode;       // 触发故障码
    uint8_t ReadStandby1;            // 读取预留
    uint8_t ReadStandby2;            // 读取预留
    uint8_t ReadStandby3;            // 读取预留
    uint8_t ReadStandby4;            // 读取预留
    uint8_t ReadStandby5;            // 读取预留
    uint8_t ReadStandby6;            // 读取预留
} Deal_Buf1_t;

typedef struct
{
    /*读取寄存器数据*/
    uint8_t Total_Voltage[2];           // 0总电压
    uint8_t Electricity[2];             // 电流
    uint8_t Surplus_Capacity[2];        // 剩余容量
    uint8_t Nominal_Capacity[2];        // 标称容量
    uint8_t Circulation_Num[2];         // 循环次数
    uint8_t Date_of_Manufacture[2];     // 生产日期
    uint8_t EquilibriumSta[2];          // 均衡状态
    uint8_t EquilibriumSta_High[2];     // 均衡状态_高
    uint8_t ProtectSta[2];              // 保护状态
    uint8_t Software_Version;           // 软件版本
    uint8_t RSOC;                       // RSOC
    uint8_t FET_ControlSat;             // FET控制状态
    uint8_t Battery_Num;                // 电池串数
    uint8_t NTC_Num;                    // NTC个数
    uint8_t NTC_Temperature[2];         // N个NTC温度
    uint8_t Humidity[2];                // 湿度
    uint8_t ReportSta[2];               // 告警状态
    uint8_t Filled_full_Capacity[2];    // 满充容量
    uint8_t Surplus_Capacity1[2];       // 剩余容量
    uint8_t Equilibrium_Electriciyt[2]; // 均衡电流
    uint8_t ReadStandby1;               // 读取预留
    uint8_t ReadStandby2;               // 读取预留
    uint8_t ReadStandby3;               // 读取预留
    uint8_t ReadStandby4;               // 读取预留
    uint8_t ReadStandby5;               // 读取预留
    uint8_t ReadStandby6;               // 读取预留
} Deal_Buf2_t;

typedef struct
{
    /*读取寄存器数据*/
    uint16_t System_Sta;                        // 0系统状态
    uint16_t Battery_Sta;                       // 电池状态
    uint16_t Level_1_Protect_Sta[2];            // 1级保护状态
    uint16_t Level_2_Protect_Sta[2];            // 2级保护状态
    uint16_t Level_3_Protect_Sta[2];            // 3级保护状态
    uint16_t Error_Sta[2];                      // 故障状态
    uint16_t Swicth_Control_Sta[2];             // 开关控制状态
    uint16_t Swicth_Response_Sta[2];            // 开关响应状态
    uint16_t Relay_Real_Sta[2];                 // 继电器真实状态
    uint16_t Sensor_Sta[2];                     // 传感器状态
    uint16_t Special_Sta[2];                    // 特殊状态
    uint16_t RESERVE[2];                        // RESERVE
    uint16_t Monomer_Total_Voltage;             // 总电压（单体累加）
    uint16_t Gather_Total_Voltage;              // 总电压（直接采集）
    uint16_t Load_Voltage;                      // 负载电压
    uint16_t Total_Electricity[2];              // 总电流
    uint16_t SOC;                               // SOC
    uint16_t SOH;                               // SOH
    uint16_t Battery_Surplus_Capacity[2];       // 电池剩余容量
    uint16_t Battery_Sufficient_Capacity[2];    // 电池满充容量
    uint16_t Battery_Design_Capacity[2];        // 电池设计容量
    uint16_t Cycle_index;                       // 循环次数
    uint16_t BMU_Quantity;                      // BMU数量
    uint16_t BMU_Online_Quantity;               // BMU在线数量
    uint16_t Positive_Insulation_Resistance[2]; // 正极绝缘阻值
    uint16_t Cathode_Insulation_Resistance[2];  // 负极绝缘阻值
} Deal_Buf3_t;

extern Deal_Buf1_t Deal_Buf1;
extern Deal_Buf2_t Deal_Buf2;
extern Deal_Buf3_t Deal_Buf3;

/*协议1数据处理引用*/
extern void Deal_WriteDataDispose(uint8_t *ReadData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose1(uint8_t *WriteData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose2(uint8_t *WriteData, uint16_t addr, uint8_t lent);
extern void Deal_ReadDataDispose3(uint8_t *WriteData, uint16_t addr, uint8_t lent);
/*读写寄存器位操作函数调用*/
extern uint16_t Write_WordManage(uint8_t State, uint16_t data, uint8_t n);
extern uint8_t Read_WordManage(uint16_t data, uint8_t n);

/*浮点互相转换十六进制*/
extern float HextoFlot(uint16_t *Data);
void FloattoHex(uint16_t *Data, float value);
uint16_t Count_Bits(uint16_t Num);


#endif
