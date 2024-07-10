/*********************************************************************
 * INCLUDES
 */

#include "board_ht1621.h"

/*函数原型*/
void HT1621_WriteData4Bit(uint8_t addr, uint8_t data);
void HT1621_WriteData8Bit(uint8_t addr, uint8_t data);
static void gpioConfig(void);

void delay_time(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 10;
        while (i--)
            ;
    }
}
/*********************************************************************
 *
 * PUBLIC FUNCTIONS
 */
/**
 @brief 初始化LCD屏，HT1621芯片
 @param 无
 @return 无
*/
void HT1621_Init(void)
{
    gpioConfig();

    delay_time(10);

    LCD_CS_1();
    LCD_DATA_1();
    LCD_WR_1();

    delay_time(100); // 延时使LCD工作电压稳定

    HT1621_WriteCommand(HT1621_SYS_EN); // 打开系统振荡器
    HT1621_WriteCommand(HT1621_BIAS);   // BIAS 13 4个公共口
    HT1621_WriteCommand(HT1621_RC256);  // 使用RC_256K系统时钟源，片内RC振荡器
    HT1621_WriteCommand(HT1621_WDT_DIS);
    HT1621_WriteCommand(HT1621_LCD_ON);
}

/**
 @brief 写入命令，命令标志100
 @param cmd -[in] 命令数据
 @return 无
*/
void HT1621_WriteCommand(uint8_t cmd)
{
    uint8_t i;

    LCD_CS_0(); // CS = 0
    delay_time(1);

    // 写入命令标志,DATA:100
    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_1(); // DATA = 1
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_0(); // DATA = 0
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_0(); // DATA = 0
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    // Datasheet中命令后的C8为0
    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_0(); // DATA = 0
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    // Datasheet中命令后的C7~C0
    for (i = 0; i < 8; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);
        if ((cmd << i) & 0x80)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
        delay_time(1);
        LCD_WR_1(); // WR = 1
        delay_time(1);
    }

    LCD_CS_1(); // CS = 1
    delay_time(1);
}

/**
 @brief 写入4位数据，写入数据标志101
 @param addr -[in] 写入起始地址
 @param data -[in] 写入数据，因为HT1621的数据位4位，所以实际写入数据为参数的后4位
 @return 无
*/
void HT1621_WriteData4Bit(uint8_t addr, uint8_t data)
{
    uint8_t i;

    LCD_CS_0(); // CS = 0
    delay_time(1);

    // 写入数据标志,DATA:101
    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_1(); // DATA = 1
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_0(); // DATA = 0
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    LCD_WR_0(); // WR = 0
    delay_time(1);
    LCD_DATA_1(); // DATA = 1
    delay_time(1);
    ;
    LCD_WR_1(); // WR = 1
    delay_time(1);

    // 写入地址,Datasheet中A5~A0
    for (i = 0; i < 6; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);
        if ((addr << i) & 0x80)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
        delay_time(1);
        LCD_WR_1(); // WR = 1
        delay_time(1);
    }
    // 写入数据,Datasheet中D0~D3
    for (i = 0; i < 4; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);
        if ((data >> (3 - i)) & 0x01)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
        delay_time(1);
        LCD_WR_1(); // WR = 1
        delay_time(1);
    }

    LCD_CS_1(); // CS = 1
    delay_time(1);
}

/**
 @brief 写入8位数据，写入数据标志101
 @param addr -[in] 写入起始地址
 @param data -[in] 写入数据
 @return 无
*/
void HT1621_WriteData8Bit(uint8_t addr, uint8_t data)
{
    uint8_t i;

    LCD_CS_0(); // CS = 0
    delay_time(1);
  
    // 写入数据标志,DATA:101
    LCD_WR_0(); // WR = 0
             
    delay_time(1);
    LCD_DATA_1(); // DATA = 1
                
    delay_time(1);
    LCD_WR_1(); // WR = 1
               
    delay_time(1);

    LCD_WR_0(); // WR = 0
               
    delay_time(1);
    LCD_DATA_0(); // DATA = 0
              
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);
    LCD_WR_0(); // WR = 0
             
    delay_time(1);
    LCD_DATA_1(); // DATA = 1
              
    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);
  

    // 写入地址,Datasheet中A5~A0
    for (i = 0; i < 6; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);
       
        if ((addr << i) & 0x80)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
       
        delay_time(1);
        LCD_WR_1(); // WR = 1
                    //				 __NOP;
                    //	  __NOP;
                    //	  __NOP;
        delay_time(1);
    }
    // 写入数据,Datasheet中两组D0~D3
    for (i = 0; i < 8; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);
       
        if ((data >> (7 - i)) & 0x01)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
        delay_time(1);
      
        LCD_WR_1(); // WR = 1
                  
        delay_time(1);
    }

    LCD_CS_1(); // CS = 1
    delay_time(1);
}

void HT1621_clear_All(void)
{
    uint8_t i;
    uint8_t addr, data;
    LCD_CS_0(); // CS = 0
    delay_time(1);

    // 写入数据标志,DATA:101
    LCD_WR_0(); // WR = 0

    delay_time(1);
    LCD_DATA_1(); // DATA = 1

    delay_time(1);
    LCD_WR_1(); // WR = 1

    delay_time(1);

    LCD_WR_0(); // WR = 0

    delay_time(1);
    LCD_DATA_0(); // DATA = 0

    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    LCD_WR_0(); // WR = 0

    delay_time(1);
    LCD_DATA_1(); // DATA = 1

    delay_time(1);
    LCD_WR_1(); // WR = 1
    delay_time(1);

    for (i = 0; i < 6; i++)
        // 写入地址,Datasheet中A5~A0
        for (i = 0; i < 6; i++)
        {
            LCD_WR_0(); // WR = 0
            delay_time(1);

            if ((addr << i) & 0x80)
            {
                LCD_DATA_1(); // DATA = 1
            }
            else
            {
                LCD_DATA_0(); // DATA = 0
            }

            delay_time(1);
            LCD_WR_1(); // WR = 1

            delay_time(1);
        }
    // 写入数据,Datasheet中两组D0~D3
    for (i = 0; i < 8; i++)
    {
        LCD_WR_0(); // WR = 0
        delay_time(1);

        if ((data >> (7 - i)) & 0x01)
        {
            LCD_DATA_1(); // DATA = 1
        }
        else
        {
            LCD_DATA_0(); // DATA = 0
        }
        delay_time(1);

        LCD_WR_1(); // WR = 1

        delay_time(1);
    }

    LCD_CS_1(); // CS = 1
    delay_time(1);
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */
/**
 @brief 引脚初始化
 @param 无
 @return 无
*/
static void gpioConfig(void)
{

    GPIO_InitTypeDef gpio_init_struct;
    HT1621_CS_CLK_ENABLE();
    HT1621_DATA_CLK_ENABLE();
    HT1621_WR_CLK_ENABLE();

    gpio_init_struct.Pin = HT1621_CS_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(HT1621_CS_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = HT1621_DATA_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(HT1621_DATA_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = HT1621_WR_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(HT1621_WR_PORT, &gpio_init_struct);

    HT1621_BACK_LIGHT_CLK_ENABLE();
    gpio_init_struct.Pin = HT1621_BACK_LIGHT_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(HT1621_BACK_LIGHT_PORT, &gpio_init_struct);
}

/****************************************************END OF FILE****************************************************/
