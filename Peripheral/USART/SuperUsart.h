/**
 ******************************************************************************
 * @file    super_uart.h
 * @author
 * @version V1.0.0
 * @date
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SUPERUSART_H__
#define __SUPERUSART_H__

/* Includes ------------------------------------------------------------------*/

#define USART_NUMBER 2

#define RS485_RX HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET)
#define RS485_TX HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET)

#include "main.h"
/* Exported macro ------------------------------------------------------------*/

#define UART1_TX_PIN GPIO_PIN_6
#define UART1_TX_PORT GPIOB
#define UART1_TX_PORT_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define UART1_RX_PIN GPIO_PIN_7
#define UART1_RX_PORT GPIOB
#define UART1_RX_PORT_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART1_RX_BUF_SIZE 512

#define UART2_TX_PIN GPIO_PIN_2
#define UART2_TX_PORT GPIOA
#define UART2_TX_PORT_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define UART2_RX_PIN GPIO_PIN_3
#define UART2_RX_PORT GPIOA
#define UART2_RX_PORT_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART2_RX_BUF_SIZE 512

/* Exported types ------------------------------------------------------------*/

typedef enum
{
  eUART_RS232_com = 0, // uart1
  eUART_RS485_com,     // uart2
} eUART_COM_ID;

typedef struct UartParamStruct
{
  uint8_t *pRxBufPtr;
  uint32_t rxBufSize;
  uint8_t *pDataBufPtr;
  uint32_t dataSize;
  uint8_t txFlag;
} UartParamStruct_def;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
// local functions

// uart
void BSP_UART_Init(uint8_t uartId, uint32_t baudrate);

uint16_t BSP_UART_GetData(uint8_t uartId, uint8_t *pbRecevBuf, uint16_t bufSize);

uint8_t BSP_UART_SendData(uint8_t uartId, uint8_t *pbSendBuf, uint16_t size);

uint16_t BSP_UART_DataSize(uint8_t uartId);

void BSP_UART_Flush_RxBuff(uint8_t uartId);

#endif
