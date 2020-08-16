#ifndef GLOBAL_H
#define GLOBAL_H

/* 头文件调用 */


/* 宏定义 */

/*  外设宏 */
#define     LED0_PORT        LED0_GPIO_Port,LED0_Pin                    /* led0GPIO定义 */
#define     LED0_ON()        HAL_GPIO_WritePin(LED0_PORT,GPIO_PIN_SET)  /* 点亮LED */
#define     LED0_OFF()       HAL_GPIO_WritePin(LED0_PORT,GPIO_PIN_RESET)/* 熄灭LED */
#define     LED0_TOGGLE()    HAL_GPIO_TogglePin(LED0_PORT)              /* 翻转LED状态 */

#define     LED1_PORT        LED1_GPIO_Port,LED1_Pin                    /* led1GPIO定义 */
#define     LED1_ON()        HAL_GPIO_WritePin(LED1_PORT,GPI0_PIN_SET)  /* 点亮LED */
#define     LED1_OFF()       HAL_GPIO_WritePin(LED1_PORT,GPIO_PIN_RESET)/* 熄灭LED */
#define     LED1_TOGGLE()    HAL_GPIO_TogglePin(LED1_PORT)              /* 翻转LED状态 */


/* 基本类型重定义 */
typedef     unsigned char  		u8;
typedef     signed char    		i8;
typedef     unsigned short 		u16;
typedef     signed short   		i16;
typedef     unsigned int   		u32;
typedef     signed int     		i32;
typedef     unsigned long long  u64;
typedef     signed long long    i64;
typedef     float          		f32;
typedef     double         		f64;


/* 全局变量声明 */

extern UART_HandleTypeDef huart1;

extern DMA_HandleTypeDef hdma_usart1_rx;


#endif

