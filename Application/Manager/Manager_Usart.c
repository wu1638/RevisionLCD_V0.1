/*
 * @Description:
 * @Author: xiao
 * @Date: 2024-05-06 13:38:52
 * @LastEditTime: 2024-07-08 14:14:20
 * @LastEditors: xiao
 */
#include "Manager.h"

/*˽�к궨��*/
#define ReadBaotou0 0x45 // Э��1��ͷ0
#define ReadBaotou1 0x47 // Э��1��ͷ1
#define ReadBaotou2 0xDD // Э��2��ͷ0
#define ReadBaotou3 0x03 // Э��2��ͷ1
// Э��3��ͷ-------------------------
#define ID 0x01					// վ��
#define Function_code 0x03		// ������
#define Register_address 0x0300 // �Ĵ�����ַ
#define Register_Len 0x002A		// �Ĵ�������
/*˽�б�������*/
uint8_t DealSendData1[Deal_Len1] = {0x00};
uint8_t DealGetData1[Deal_Len1] = {0x00};
uint8_t DealSendData2[7] = {0x00};
uint8_t DealGetData2[Deal_Len2] = {0x00};
uint8_t DealSendData3[8] = {0x00};
uint8_t DealGetData3[Deal_Len3] = {0x00};
uint8_t RealData[100] = {0x00};
uint8_t Verify_ComDataHihg = 0, Verify_ComDatalow = 0;
uint8_t Deal_TriggerMode1 = 0, Deal_TriggerMode2 = 0, Deal_TriggerMode3 = 0, PollCount = 0;

/*�ⲿ���ú���*/
extern uint16_t SendFag;
/*˽�к�������*/
void Manager_Usart_Init(void);
void Manager_Usart_Activate(void);
void Manager_Usart_Mainloop(void);
void Deal_SelectDispose(void);
void Pack_DealSendData1_Format(void);
void Pack_DealSendData2_Format(void);
void Pack_DealSendData3_Format(void);
void Pack_DealGetData1_Format(void);
void Pack_DealGetData2_Format(void);
void Pack_DealGetData3_Format(void);
void AppDataHandle(void);

void Manager_Usart_Init(void)
{
	BSP_UART_Init(eUART_RS232_com, 9600);
	BSP_UART_Init(eUART_RS485_com, 9600);
}

void Manager_Usart_Activate(void)
{
}

void Manager_Usart_Mainloop(void)
{
	Pack_DealSendData1_Format();
	Pack_DealSendData2_Format();
	Pack_DealSendData3_Format();
	Deal_SelectDispose();
	// ��ʱ����
	if (SendFag)
	{

		if (Deal_TriggerMode1)
		{
			BSP_UART_SendData(eUART_RS232_com, DealSendData1, Deal_Len1);
			BSP_UART_SendData(eUART_RS485_com, DealSendData1, Deal_Len1);
		}
		else if (Deal_TriggerMode2)
		{
			BSP_UART_SendData(eUART_RS232_com, DealGetData2, Deal_Len2);
			BSP_UART_SendData(eUART_RS485_com, DealSendData2, 7);
		}
		else if (Deal_TriggerMode3)
		{
			BSP_UART_SendData(eUART_RS232_com, DealSendData3, Deal_Len3);
			BSP_UART_SendData(eUART_RS485_com, DealSendData3, 8);
		}
		else
		{
			/*��ѯ���*/
			if (PollCount == 1)
			{
				BSP_UART_SendData(eUART_RS485_com, DealSendData1, Deal_Len1);
			}
			else if (PollCount == 2)
			{
				BSP_UART_SendData(eUART_RS485_com, DealSendData2, 7);
			}
			else if (PollCount == 3)
			{
				BSP_UART_SendData(eUART_RS485_com, DealSendData3, 8);
				PollCount = FALSE;
			}
		}
		PollCount++;
		SendFag = FALSE;
	}

	AppDataHandle();
}

/*Э��1�������� ���*/
void Pack_DealSendData1_Format(void)
{
	uint16_t usCRC16;

	DealSendData1[0] = 0x55;
	DealSendData1[1] = 0xAA;

	Deal_WriteDataDispose(DealSendData1, 0, 14);

	usCRC16 = usMBCRC16(DealSendData1, Deal_Len1 - 2);
	DealSendData1[16] = (uint8_t)(usCRC16 & 0xFF);
	DealSendData1[17] = (uint8_t)(usCRC16 >> 8);
}
/*Э��2�������� ���*/
void Pack_DealSendData2_Format(void)
{
	DealSendData2[0] = 0xDD;
	DealSendData2[1] = 0xA5;
	DealSendData2[2] = 0x03;
	DealSendData2[3] = 0x00;
	DealSendData2[4] = 0xFF;
	DealSendData2[5] = 0xFD;
	DealSendData2[6] = 0x77;
}
/*Э��3�������ݴ��*/
void Pack_DealSendData3_Format(void)
{
	/*03������*/
	uint16_t usCRC16;

	DealSendData3[0] = ID;
	DealSendData3[1] = Function_code;
	DealSendData3[2] = (uint8_t)(Register_address >> 8);
	DealSendData3[3] = (uint8_t)(Register_address & 0xff);
	DealSendData3[4] = (uint8_t)(Register_Len >> 8);
	DealSendData3[5] = (uint8_t)(Register_Len & 0xff);
	usCRC16 = usMBCRC16(DealSendData3, 6);
	DealSendData3[6] = (uint8_t)(usCRC16 & 0xFF);
	DealSendData3[7] = (uint8_t)(usCRC16 >> 8);
}
/*Э��ѡ����*/
void Deal_SelectDispose(void)
{
	if (BSP_UART_DataSize(eUART_RS485_com))
	{
		BSP_UART_GetData(eUART_RS485_com, RealData, 100);
	}

	if (RealData[0] == ReadBaotou0 && RealData[1] == ReadBaotou1)
	{
		memcpy(DealGetData1, RealData, Deal_Len1);
		Pack_DealGetData1_Format();
		Deal_TriggerMode1 = TRUE;
	}
	else if (RealData[0] == ReadBaotou2 && RealData[1] == ReadBaotou3)
	{
		memcpy(DealGetData2, RealData, Deal_Len2);
		Pack_DealGetData2_Format();
		Deal_TriggerMode2 = TRUE;
	}
	else if (RealData[0] == ID && RealData[1] == Function_code && RealData[2] == 2 * Register_Len)
	{
		memcpy(DealGetData3, RealData, Deal_Len3);
		Pack_DealGetData3_Format();
		Deal_TriggerMode3 = TRUE;
	}
}
/*Э��1��ȡ���ݴ���*/
void Pack_DealGetData1_Format(void)
{
	uint16_t usCRC16;
	uint8_t usCRC16_H, usCRC16_L;

	usCRC16 = usMBCRC16(DealGetData1, Deal_Len1 - 2);
	usCRC16_H = (uint8_t)(usCRC16 & 0xFF);
	usCRC16_L = (uint8_t)(usCRC16 >> 8);
	// ����������֤����
	if (DealGetData1[0] == ReadBaotou0 && DealGetData1[1] == ReadBaotou1)
	{
		if (DealGetData1[16] == usCRC16_H && DealGetData1[17] == usCRC16_L)
		{
			Deal_ReadDataDispose1(DealGetData1, 14, 13);
		}
	}
}

/*Э��2��ȡ���ݴ���*/
void Pack_DealGetData2_Format(void)
{
	uint16_t CRC_DataTotal = 0, CRC_DataResult = 0;

	if (BSP_UART_DataSize(eUART_RS485_com))
	{
		BSP_UART_GetData(eUART_RS485_com, DealGetData1, Deal_Len1);
	}

	for (uint8_t i = 2; i < 39; i++)
	{
		/* code */
		CRC_DataTotal += DealGetData2[i];
	}
	CRC_DataResult = (~CRC_DataTotal) + 0x01;
	Verify_ComDataHihg = (uint8_t)(CRC_DataResult >> 8);
	Verify_ComDatalow = (uint8_t)(CRC_DataResult & 0xff);

	// ����������֤����
	if (DealGetData2[0] == ReadBaotou2 && DealGetData2[1] == ReadBaotou3)
	{
		if (DealGetData2[39] == Verify_ComDataHihg && DealGetData2[40] == Verify_ComDatalow)
		{
			Deal_ReadDataDispose2(DealGetData2, 0, 38);
		}
	}
	DealGetData2[48] = Verify_ComDataHihg;
	DealGetData2[49] = Verify_ComDatalow;
}
/*Э��3��ȡ���ݴ���*/
void Pack_DealGetData3_Format(void)
{
	uint16_t usCRC16;
	uint8_t usCRC16_H, usCRC16_L;

	usCRC16 = usMBCRC16(DealGetData3, 2 * Register_Len + 3);
	usCRC16_H = (uint8_t)(usCRC16 & 0xFF);
	usCRC16_L = (uint8_t)(usCRC16 >> 8);

	// ����������֤����
	if (DealGetData3[0] == ID && DealGetData3[1] == Function_code && DealGetData3[2] == 2 * Register_Len)
	{
		if (DealGetData3[87] == usCRC16_H && DealGetData3[88] == usCRC16_L)
		{
			Deal_ReadDataDispose3(DealGetData3, 0, 42);
			DealGetData3[96] = usCRC16_H;
			DealGetData3[97] = usCRC16_L;
		}
	}
}

/*����Ӧ�ô���*/
void AppDataHandle(void)
{
	Deal_Buf1.Heartcount[0] = (uint8_t)(Heartbeat >> 8);				  // ���������ֽ�
	Deal_Buf1.Heartcount[1] = (uint8_t)(Heartbeat & 0xFF);				  // ���������ֽ�
	Deal_Buf1.Panel_CurrentSta = Deal_Buf1.Trigger_OrderSta;			  // ����״̬����
	Deal_Buf1.Current_BatterySOC = Deal_Buf1.BatterySOC_SetValue;		  // ��ǰ���SOC����
	Deal_Buf1.Current_InpuPower[0] = Deal_Buf1.InputPower_SetValue[0];	  // ��ǰ���빦�ʸ��ֽ�
	Deal_Buf1.Current_InpuPower[1] = Deal_Buf1.InputPower_SetValue[1];	  // ��ǰ���빦�ʵ��ֽ�
	Deal_Buf1.Current_OutputPower[0] = Deal_Buf1.OutputPower_SetValue[0]; // ��ǰ������ʸ��ֽ�
	Deal_Buf1.Current_OutputPower[1] = Deal_Buf1.OutputPower_SetValue[1]; // ��ǰ������ʵ��ֽ�
	Deal_Buf1.Panel_ErrorCode = Deal_Buf1.Trigger_ErrorCode;			  // ��ǰ����������
}
