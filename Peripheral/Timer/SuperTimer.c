/**
 ******************************************************************************
 * 文件名程: superTimer.c
 * 作    者: xiao
 * 版    本: V1.0
 * 编写日期: 2022-10-12
 * 功    能: 驱动定时器
 ******************************************************************************
 */
#include "SuperTimer.h"

TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

TimerCallBack Timer14_callBack = NULL;
TimerCallBack Timer16_callBack = NULL;
TimerCallBack Timer17_callBack = NULL;

void BSP_Timer_Init(void);

void BSP_Timer_SetCallback_Hearbeat(TimerCallBack callBack);
void BSP_Timer_SendCallBack(TimerCallBack callBack);
void BSP_Timer_FlickerCallBack(TimerCallBack callBack);


uint8_t TIM14_Init(uint32_t timerCountClk, uint16_t timerPeriod)
{
    __HAL_RCC_TIM14_CLK_ENABLE();


    htim14.Instance = TIM14;
    htim14.Init.Prescaler = timerCountClk;
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim14.Init.Period = timerPeriod;
    htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
    {
        while (1)
        {
        }
    }
    HAL_NVIC_SetPriority(TIM14_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
    /* USER CODE BEGIN TIM4_Init 2 */
    HAL_TIM_Base_Start_IT(&htim14); // 开启定时器14
    HAL_TIM_Base_Start(&htim14);

    return HAL_ERROR;
}

uint8_t TIM16_Init(uint32_t timerCountClk, uint16_t timerPeriod)
{

    /* USER CODE BEGIN TIM6_Init 0 */
    __HAL_RCC_TIM16_CLK_ENABLE();
    /* USER CODE END TIM6_Init 0 */

    htim16.Instance = TIM16;
    htim16.Init.Prescaler = timerCountClk;
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.Period =  timerPeriod;
    htim16.Init.RepetitionCounter = 0;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
    {
        while (1)
        {
        }
    }
   
    /* USER CODE BEGIN TIM6_Init 2 */
    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM16_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
    /* USER CODE END TIM6_Init 2 */
    HAL_TIM_Base_Start_IT(&htim16); // 开启定时器16
    return HAL_ERROR;
}

uint8_t TIM17_Init(uint32_t timerCountClk, uint16_t timerPeriod)
{

    /* USER CODE BEGIN TIM6_Init 0 */
    __HAL_RCC_TIM17_CLK_ENABLE();
    /* USER CODE END TIM6_Init 0 */

    htim17.Instance = TIM17;
    htim17.Init.Prescaler = timerCountClk;
    htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim17.Init.Period = timerPeriod;
    htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim17.Init.RepetitionCounter = 0;
    htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
    {
        while (1)
        {
        }
    }
   
    HAL_NVIC_SetPriority(TIM17_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(TIM17_IRQn);

    HAL_TIM_Base_Start_IT(&htim17); // 开启定时器17
    return HAL_ERROR;
}

void TIM14_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim14);
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}
/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */

  /* USER CODE END TIM17_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim14.Instance)
    {
        if (Timer14_callBack != NULL)
            Timer14_callBack();
    }
    else if (htim->Instance == htim16.Instance)
    {
        if (Timer16_callBack != NULL)
            Timer16_callBack();
    }
    else if (htim->Instance == htim17.Instance)
    {
        if (Timer17_callBack != NULL)
            Timer17_callBack();
    }
   
}


void BSP_Timer_SetCallback_Hearbeat(TimerCallBack callBack)
{
    Timer14_callBack = callBack;
}

void BSP_Timer_SendCallBack(TimerCallBack callBack)
{
    Timer16_callBack = callBack;
}

void BSP_Timer_FlickerCallBack(TimerCallBack callBack)
{
    Timer17_callBack = callBack;
}

/*定时器初始化*/
void BSP_Timer_Init(void)
{
    TIM14_Init(TIMER14_COUNT_CLK_FEQ, TIMER14_PERIOD_US);
    TIM16_Init(TIMER16_COUNT_CLK_FEQ, TIMER16_PERIOD_US); 
    TIM17_Init(TIMER17_COUNT_CLK_FEQ, TIMER17_PERIOD_US);

}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
