/**
 ******************************************************************************
 * @file    bsp_uart.c
 * @author
 * @version V1.0.0
 * @date
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include "SuperUsart.h"
#include "stdio.h"
#include <string.h>
#include <ctype.h>

UART_HandleTypeDef gUart1_Handle;
UART_HandleTypeDef gUart2_Handle;

DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

uint8_t UART1_RX_buf_A[UART1_RX_BUF_SIZE];
uint8_t UART1_RX_buf_B[UART1_RX_BUF_SIZE];

uint8_t UART2_RX_buf_A[UART2_RX_BUF_SIZE];
uint8_t UART2_RX_buf_B[UART2_RX_BUF_SIZE];

volatile static UartParamStruct_def gUartParamStruct[2];

void UART_DMA_Rx_Cfg(USART_TypeDef *Instance, uint8_t *RxBuf, uint32_t bufSize);

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&gUart1_Handle, (uint8_t *)&ch, 1, 0X000A);
  return ch;
}

void UART1_init(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

  /* Peripheral clock enable */
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();

  UART1_TX_PORT_CLK_ENABLE();
  UART1_RX_PORT_CLK_ENABLE();
  /**USART1 GPIO Configuration*/
  GPIO_InitStruct.Pin = UART1_TX_PIN | UART1_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
  HAL_GPIO_Init(UART1_TX_PORT, &GPIO_InitStruct);
  HAL_GPIO_Init(UART1_RX_PORT, &GPIO_InitStruct);

  /* USER CODE END USART1_Init 1 */

  gUart1_Handle.Instance = USART1;
  gUart1_Handle.Init.BaudRate = baudrate;
  gUart1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  gUart1_Handle.Init.StopBits = UART_STOPBITS_1;
  gUart1_Handle.Init.Parity = UART_PARITY_NONE;
  gUart1_Handle.Init.Mode = UART_MODE_TX_RX;
  gUart1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  gUart1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  gUart1_Handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  gUart1_Handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  gUart1_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&gUart1_Handle) != HAL_OK)
  {
    while (1)
    {
    }
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&gUart1_Handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&gUart1_Handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&gUart1_Handle) != HAL_OK)
  {
    Error_Handler();
  }

  /* USART1 DMA Init */

  hdma_usart1_rx.Instance = DMA1_Channel1;
  hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
  {
    while (1)
    {
    }
  }

  __HAL_LINKDMA(&gUart1_Handle, hdmarx, hdma_usart1_rx);

  hdma_usart1_tx.Instance = DMA1_Channel2;
  hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
  hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_tx.Init.Mode = DMA_NORMAL;
  hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
  if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
  {
    while (1)
    {
    }
  }

  __HAL_LINKDMA(&gUart1_Handle, hdmatx, hdma_usart1_tx);
   /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

   /* NVIC configuration for USART*/
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

  gUartParamStruct[0].pRxBufPtr = UART1_RX_buf_A;
  gUartParamStruct[0].rxBufSize = UART1_RX_BUF_SIZE;
  gUartParamStruct[0].pDataBufPtr = UART1_RX_buf_B;
  gUartParamStruct[0].dataSize = 0;
  gUartParamStruct[0].txFlag = 0;

  __HAL_UART_ENABLE_IT(&gUart1_Handle, UART_IT_IDLE);
  UART_DMA_Rx_Cfg(USART1, gUartParamStruct[0].pRxBufPtr, gUartParamStruct[0].rxBufSize);
}

void UART2_init(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  /** Initializes the peripherals clock
   */

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();

  UART2_TX_PORT_CLK_ENABLE();
  UART2_RX_PORT_CLK_ENABLE();


  /**USART1 GPIO Configuration*/
  GPIO_InitStruct.Pin = UART2_TX_PIN | UART2_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  HAL_GPIO_Init(UART2_TX_PORT, &GPIO_InitStruct);
  HAL_GPIO_Init(UART2_RX_PORT, &GPIO_InitStruct);

  /* USER CODE END USART1_Init 1 */

  gUart2_Handle.Instance = USART2;
  gUart2_Handle.Init.BaudRate = baudrate;
  gUart2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  gUart2_Handle.Init.StopBits = UART_STOPBITS_1;
  gUart2_Handle.Init.Parity = UART_PARITY_NONE;
  gUart2_Handle.Init.Mode = UART_MODE_TX_RX;
  gUart2_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  gUart2_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  gUart2_Handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  gUart2_Handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  gUart2_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&gUart2_Handle) != HAL_OK)
  {
    while (1)
    {
    }
  }

  /* USART1 DMA Init */
  hdma_usart2_rx.Instance = DMA1_Channel3;
  hdma_usart2_rx.Init.Request = DMA_REQUEST_USART2_RX;
  hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_rx.Init.Mode = DMA_NORMAL;
  hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
  {
    while (1)
    {
    }
  }

  __HAL_LINKDMA(&gUart2_Handle, hdmarx, hdma_usart2_rx);

  /* USART3_TX Init */
  hdma_usart2_tx.Instance = DMA1_Channel4;
  hdma_usart2_tx.Init.Request = DMA_REQUEST_USART2_TX;
  hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.Mode = DMA_NORMAL;
  hdma_usart2_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
  if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
  {
    while (1)
    {
    }
  }
  __HAL_LINKDMA(&gUart2_Handle, hdmatx, hdma_usart2_tx);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  HAL_NVIC_SetPriority(DMA1_Ch4_5_DMAMUX1_OVR_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Ch4_5_DMAMUX1_OVR_IRQn);

   /* NVIC configuration for USART*/
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  gUartParamStruct[1].pRxBufPtr = UART2_RX_buf_A;
  gUartParamStruct[1].rxBufSize = UART2_RX_BUF_SIZE;
  gUartParamStruct[1].pDataBufPtr = UART2_RX_buf_B;
  gUartParamStruct[1].dataSize = 0;
  gUartParamStruct[1].txFlag = 0;

  __HAL_UART_ENABLE_IT(&gUart2_Handle, UART_IT_IDLE);
  UART_DMA_Rx_Cfg(USART2, gUartParamStruct[1].pRxBufPtr, gUartParamStruct[1].rxBufSize);
}

void BSP_UART_Init(uint8_t uartId, uint32_t baudrate)
{
  if (uartId == eUART_RS232_com) // uart1
  {
    UART1_init(baudrate);
  }
  else if (uartId == eUART_RS485_com) // uart2
  {
    UART2_init(baudrate);
  }
}
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&gUart1_Handle);
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&gUart2_Handle);
  //RS485_RX;
}
/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}
/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4, channel 5 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    UART_DMA_Rx_Cfg(USART1, gUartParamStruct[0].pRxBufPtr, gUartParamStruct[0].rxBufSize);
  }

  else if (huart->Instance == USART2)
  {
    UART_DMA_Rx_Cfg(USART2, gUartParamStruct[1].pRxBufPtr, gUartParamStruct[1].rxBufSize);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1)
  {
    gUartParamStruct[0].dataSize = Size;
    gUartParamStruct[0].pDataBufPtr = gUartParamStruct[0].pRxBufPtr;
    if (gUartParamStruct[0].pRxBufPtr == UART1_RX_buf_A)
      gUartParamStruct[0].pRxBufPtr = UART1_RX_buf_B;
    else
      gUartParamStruct[0].pRxBufPtr = UART1_RX_buf_A;
    gUartParamStruct[0].rxBufSize = UART1_RX_BUF_SIZE;

    UART_DMA_Rx_Cfg(USART1, gUartParamStruct[0].pRxBufPtr, gUartParamStruct[0].rxBufSize);
  }
  else if (huart->Instance == USART2)
  {
    gUartParamStruct[1].dataSize = Size;
    gUartParamStruct[1].pDataBufPtr = gUartParamStruct[1].pRxBufPtr;
    if (gUartParamStruct[1].pRxBufPtr == UART2_RX_buf_A)
      gUartParamStruct[1].pRxBufPtr = UART2_RX_buf_B;
    else
      gUartParamStruct[1].pRxBufPtr = UART2_RX_buf_A;

    gUartParamStruct[1].rxBufSize = UART2_RX_BUF_SIZE;
    UART_DMA_Rx_Cfg(USART2, gUartParamStruct[1].pRxBufPtr, gUartParamStruct[1].rxBufSize);
  }
}

void UART_DMA_Rx_Cfg(USART_TypeDef *Instance, uint8_t *RxBuf, uint32_t bufSize)
{
  if (Instance == USART1)
  {
    HAL_UART_DMAStop(&gUart1_Handle);
    gUart1_Handle.Instance->RDR; // clean catche data;
    HAL_UARTEx_ReceiveToIdle_DMA(&gUart1_Handle, RxBuf, bufSize);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  }
  else if (Instance == USART2)
  {
    //RS485_TX;
    RS485_RX;
    HAL_UART_DMAStop(&gUart2_Handle);
    gUart2_Handle.Instance->RDR; // clean catche data;
    HAL_UARTEx_ReceiveToIdle_DMA(&gUart2_Handle, RxBuf, bufSize);
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
  }
}

uint16_t BSP_UART_GetData(uint8_t uartId, uint8_t *pbRecevBuf, uint16_t bufSize)
{
  uint16_t size = 0;

  if (uartId == eUART_RS232_com) // uart1
  {
    if (gUartParamStruct[0].dataSize > 0)
    {
      if (gUartParamStruct[0].dataSize > bufSize)
        size = bufSize;
      else
        size = gUartParamStruct[0].dataSize;
      memcpy(pbRecevBuf, gUartParamStruct[0].pDataBufPtr, size);

      memset(gUartParamStruct[0].pDataBufPtr, 0, gUartParamStruct[0].dataSize);
      gUartParamStruct[0].dataSize = 0;
    }
  }
  else if (uartId == eUART_RS485_com) // uart2
  {
    if (gUartParamStruct[1].dataSize > 0)
    {
      if (gUartParamStruct[1].dataSize > bufSize)
        size = bufSize;
      else
        size = gUartParamStruct[1].dataSize;
      memcpy(pbRecevBuf, gUartParamStruct[1].pDataBufPtr, size);

      memset(gUartParamStruct[1].pDataBufPtr, 0, gUartParamStruct[1].dataSize);
      gUartParamStruct[1].dataSize = 0;
    }
  }

  return size;
}

uint8_t BSP_UART_SendData(uint8_t uartId, uint8_t *pbSendBuf, uint16_t size)
{
  uint8_t status = 0;

  if (uartId == eUART_RS232_com) // uart1
  {
    HAL_UART_DMAStop(&gUart1_Handle);
    status = HAL_UART_Transmit(&gUart1_Handle, pbSendBuf, size,1000);
    UART_DMA_Rx_Cfg(USART1, gUartParamStruct[0].pRxBufPtr, gUartParamStruct[0].rxBufSize);
  }
  else if (uartId == eUART_RS485_com) // uart2
  {
    RS485_TX;
    HAL_UART_DMAStop(&gUart2_Handle);
    //RS485_RX;
    status = HAL_UART_Transmit(&gUart2_Handle, pbSendBuf, size,1000);
    UART_DMA_Rx_Cfg(USART2, gUartParamStruct[1].pRxBufPtr, gUartParamStruct[1].rxBufSize);
  }

  return status;
}

uint16_t BSP_UART_DataSize(uint8_t uartId)
{
  if (uartId == eUART_RS232_com) // uart1
  {
    return gUartParamStruct[0].dataSize;
  }
  else if (uartId == eUART_RS485_com) // uart2
  {
    return gUartParamStruct[1].dataSize;
  }

  return 0;
}

void BSP_UART_Flush_RxBuff(uint8_t uartId)
{
  if (uartId == eUART_RS232_com) // uart1
  {
    memset(gUartParamStruct[0].pDataBufPtr, 0, UART1_RX_BUF_SIZE);
    gUartParamStruct[0].dataSize = 0;
  }
  else if (uartId == eUART_RS485_com) // uart2
  {
    memset(gUartParamStruct[1].pDataBufPtr, 0, UART2_RX_BUF_SIZE);
    gUartParamStruct[1].dataSize = 0;
  }
}
