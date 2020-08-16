#ifndef OLED_H
#define OLED_H

/* 头文件调用 */
#include "global.h"


/* 宏定义 */

/* 串行EEPROM数据线宏定义 */
#define SDA_HIGH()      HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_SET)
#define SDA_LOW()       HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_RESET)

#define SCL_HIGH()      HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_SET)
#define SCL_LOW()       HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_RESET)

#define SDA_RD()        HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)


#define OLED_MODE   0
#define SIZE        8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

enum OLED_WRITE_MODE
{
    OLED_CMD = 0,
    OLED_DATA
};

/* 全局函数原型声明 */

extern void OLEDInit(void);
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_On(void);  
extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
extern void OLED_Clear(void);  
extern void OLED_ShowCHinese(u8 x,u8 y,u8 no);
extern void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
extern void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);

#endif
