#ifndef GLOBAL_H
#define GLOBAL_H

/* ͷ�ļ����� */


/* �궨�� */

/*  ����� */
#define     LED0_PORT        LED0_GPIO_Port,LED0_Pin                    /* led0GPIO���� */
#define     LED0_ON()        HAL_GPIO_WritePin(LED0_PORT,GPIO_PIN_SET)  /* ����LED */
#define     LED0_OFF()       HAL_GPIO_WritePin(LED0_PORT,GPIO_PIN_RESET)/* Ϩ��LED */
#define     LED0_TOGGLE()    HAL_GPIO_TogglePin(LED0_PORT)              /* ��תLED״̬ */

#define     LED1_PORT        LED1_GPIO_Port,LED1_Pin                    /* led1GPIO���� */
#define     LED1_ON()        HAL_GPIO_WritePin(LED1_PORT,GPI0_PIN_SET)  /* ����LED */
#define     LED1_OFF()       HAL_GPIO_WritePin(LED1_PORT,GPIO_PIN_RESET)/* Ϩ��LED */
#define     LED1_TOGGLE()    HAL_GPIO_TogglePin(LED1_PORT)              /* ��תLED״̬ */


/* ���������ض��� */
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


/* ȫ�ֱ������� */

extern UART_HandleTypeDef huart1;

extern DMA_HandleTypeDef hdma_usart1_rx;


#endif

