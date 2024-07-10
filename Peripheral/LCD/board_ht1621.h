/*
 * @Description: 
 * @Author: xiao
 * @Date: 2024-05-10 09:11:08
 * @LastEditTime: 2024-05-14 18:37:14
 * @LastEditors: xiao
 */
#ifndef _BOARD_HT1621_H_
#define _BOARD_HT1621_H_

/*********************************************************************
 * INCLUDES
 */
 #include "main.h"


/*********************************************************************
 * DEFINITIONS
 */
#define HT1621_BACK_LIGHT_PORT              GPIOA                   // back light 端口
#define HT1621_BACK_LIGHT_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    // GPIO端口时钟
#define HT1621_BACK_LIGHT_PIN               GPIO_PIN_6             // 定义HT1621的CS管脚


#define HT1621_CS_PORT              GPIOA                   // GPIO端口
#define HT1621_CS_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    // GPIO端口时钟
#define HT1621_CS_PIN               GPIO_PIN_4             // 定义HT1621的CS管脚

#define HT1621_DATA_PORT            GPIOA                   // GPIO端口
#define HT1621_DATA_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    // GPIO端口时钟
#define HT1621_DATA_PIN             GPIO_PIN_5             // 定义HT1621的DATA管脚

#define HT1621_WR_PORT              GPIOA                   // GPIO端口
#define HT1621_WR_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    // GPIO端口时钟
#define HT1621_WR_PIN               GPIO_PIN_7             // 定义HT1621的WR管脚


#define LCD_BACK_LIGHT_0()  				HAL_GPIO_WritePin(HT1621_BACK_LIGHT_PORT, HT1621_BACK_LIGHT_PIN, GPIO_PIN_RESET)
#define LCD_BACK_LIGHT_1()  				HAL_GPIO_WritePin(HT1621_BACK_LIGHT_PORT, HT1621_BACK_LIGHT_PIN, GPIO_PIN_SET)


#define LCD_CS_0()  					      HAL_GPIO_WritePin(HT1621_CS_PORT, HT1621_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_1()  					      HAL_GPIO_WritePin(HT1621_CS_PORT, HT1621_CS_PIN, GPIO_PIN_SET)
																								
												
#define LCD_DATA_0()  					    HAL_GPIO_WritePin(HT1621_DATA_PORT, HT1621_DATA_PIN, GPIO_PIN_RESET)
#define LCD_DATA_1()  					    HAL_GPIO_WritePin(HT1621_DATA_PORT, HT1621_DATA_PIN, GPIO_PIN_SET)											

												
#define LCD_WR_0()  					      HAL_GPIO_WritePin(HT1621_WR_PORT, HT1621_WR_PIN, GPIO_PIN_RESET)
#define LCD_WR_1()  					      HAL_GPIO_WritePin(HT1621_WR_PORT, HT1621_WR_PIN, GPIO_PIN_SET)											
							

#define COMMAND_CODE                0x80                    // 命令码
#define WRITE_DATA_CODE             0xA0                    // 写命令

#define HT1621_BIAS                 0x52                    // 1/3duty 4com
#define HT1621_SYS_DIS              0x00                    // 关振系统荡器和LCD偏压发生器
#define HT1621_SYS_EN               0x02                    // 打开系统振荡器
#define HT1621_LCD_OFF              0x04                    // 关闭LCD偏压
#define HT1621_LCD_ON               0x06                    // 打开LCE偏压
#define HT1621_XTAL                 0x28                    // 外部接时钟
#define HT1621_RC256                0x30                    // 内部时钟
#define HT1621_WDT_DIS              0X0A                    // 关闭看门狗
#define HT1621_TONE_ON              0x12                    // 打开声音输出
#define HT1621_TONE_OFF             0x10                    // 关闭声音输出

#define Disp_start_addr             (0x0f<<2)  //显示起始地址

/*********************************************************************
 * API FUNCTIONS
 */
void HT1621_Init(void);
void HT1621_WriteCommand(uint8_t cmd);
extern void HT1621_WriteData4Bit(uint8_t addr, uint8_t data);
extern void HT1621_WriteData8Bit(uint8_t addr, uint8_t data);

#endif /* _BOARD_HT1621_H_ */
