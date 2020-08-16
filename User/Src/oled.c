#include "oled.h"

/* ͷ�ļ����� */
#include "oledfont.h"

/* ȫ�ֱ������� */



/* ��̬�������� */




/* �������� */


/* static function declaration */
					
static void I2CWaitAck(void);               /* I2C�ȴ�Ӧ���ź� */

static void I2CStop(void);                  /* I2Cֹͣ */

static void I2CStart(void);                 /* I2C���� */

static void I2CAck(void);                   /* I2C����Ӧ���ź� */

static void I2CNAck(void);                  /* I2C������Ӧ���ź� */

static u8 I2CReadByte(u8 ask);              /* I2C��ȡһ�ֽ����� */

static void I2CWriteByte(u8 SendByte);      /* I2Cд��һ�ֽ����� */

static void GpioModeCfg(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,u8 Mode);  /* GPIO�������ģʽ���� */

static void Write_IIC_Command(u8 IIC_Command);

static void Write_IIC_Data(u8 IIC_Data);

static void OLED_WR_Byte(u8 dat,u8 cmd);


/* function area */

/**************************************************
                    Ӳ��IIC�ӿ�
***************************************************/

/********************
��ʱ1ms����
*********************/
static void I2CDelay(void)
{	
    u32 i = 5;
    while(i--);
}


/**********************
I2C����
**********************/
static void I2CStart(void)
{
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_OUTPUT_PP);
    SDA_HIGH();
    SCL_HIGH();
    I2CDelay();
    SDA_LOW();
    I2CDelay();
    SCL_LOW();
    I2CDelay();
}


/**********************
I2Cֹͣ
**********************/
static void I2CStop(void)
{
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_OUTPUT_PP);
    SDA_LOW();
    SCL_LOW();
    I2CDelay();
    SCL_HIGH();
    SDA_HIGH();
    I2CDelay();
}


/**********************
�ȴ�I2CӦ���ź�ASK
**********************/
static void I2CWaitAck(void)
{
	u8 errtime=0xFF;

    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_INPUT);
	SDA_HIGH();
	I2CDelay();
	SCL_HIGH();
	I2CDelay();
	while(SDA_RD())
	{
		errtime--;
		if(!errtime)
        {
            I2CStop();
            return ;
        }
	}
    SCL_LOW();		
    I2CDelay();
}

/**********************
I2C����Ӧ���ź�ASK
**********************/
static void I2CAck(void)
{
	SCL_LOW();
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_OUTPUT_PP);
	SDA_LOW();
	I2CDelay();
	SCL_HIGH();
	I2CDelay();
	SCL_LOW();
}


/**********************
I2C������Ӧ���ź�ASK
**********************/   
static void I2CNAck(void)
{
	SCL_LOW();
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_OUTPUT_PP);
	SDA_HIGH();
	I2CDelay();
	SCL_HIGH();
	I2CDelay();
	SCL_LOW();
}		

/**********************
I2C��ȡһ���ֽ�
ask = 1ʱ������ASK
ask = 0ʱ������NASK
**********************/   
static u8 I2CReadByte(u8 ask)
{
    u8 i,recdat;
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_INPUT);

    for(i=0;i<8;i++)
    {
        SCL_LOW();
        I2CDelay();
        SCL_HIGH();
        I2CDelay();
        recdat <<= 1;
        if(SDA_RD() == 1)
        {
            recdat = 1;
        }
        else
        {
            recdat = 0;
        }
        I2CDelay();
    }

    if(ask == 1)
    {
        I2CAck();
    }
    else if(ask == 0)
    {
        I2CNAck(); 
    }
    
    return recdat;
}

/**********************
I2C����һ���ֽ�
���شӻ�����Ӧ��
**********************/   
static void I2CWriteByte(u8 SendByte)
{
    u8 i;
    GpioModeCfg(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_MODE_OUTPUT_PP);
    SCL_LOW();
    for(i=0;i<8;i++)
    {
        if(((SendByte & 0x80)>>7) == 1)
        {
            SDA_HIGH();
        }
        else
        {
            SDA_LOW();
        }
        SendByte <<= 1;
        I2CDelay();
        SCL_HIGH();
        I2CDelay();
        SCL_LOW();
        I2CDelay();
    }
}


/*
����ԭ�ͣ�void GpioModeCfg(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,u8 Mode)
�������ܣ�GPIO�������ģʽ����
���������Mode��GPIO_MODE_INPUT������ģʽ
                GPIO_MODE_OUTPUT_PP���������ģʽ
�����������
����ֵ����
���ڣ�2020-07-09
���ߣ�����Ԭ
*/
static void GpioModeCfg(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,u8 Mode)
{
    GPIO_InitTypeDef GpioInitStruct;

    GpioInitStruct.Pin = GPIO_Pin;
    GpioInitStruct.Mode = Mode;

    HAL_GPIO_Init(GPIOx, &GpioInitStruct);
}

/**************************************************
                    0.96�� IIC�ӿ�OLED
***************************************************/

/**********************************************
IICд����
**********************************************/
static void Write_IIC_Command(u8 IIC_Command)
{
    I2CStart();
    I2CWriteByte(0x78);            //Slave address,SA0=0
    I2CWaitAck();	
    I2CWriteByte(0x00);			//write command
    I2CWaitAck();	
    I2CWriteByte(IIC_Command); 
    I2CWaitAck();	
    I2CStop();
}


/**********************************************
IICд����
**********************************************/
static void Write_IIC_Data(u8 IIC_Data)
{
    I2CStart();
    I2CWriteByte(0x78);			//D/C#=0; R/W#=0
    I2CWaitAck();	
    I2CWriteByte(0x40);			//write data
    I2CWaitAck();	
    I2CWriteByte(IIC_Data);
    I2CWaitAck();	
    I2CStop();
}

/**********************************************
OLEDд����/����
**********************************************/
static void OLED_WR_Byte(u8 dat,u8 cmd)
{
    if(cmd)
    {
        Write_IIC_Data(dat);
    }
    else 
    {
        Write_IIC_Command(dat);
    }
}


void OLEDInit(void)
{
    HAL_Delay(200);
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
    OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
    OLED_WR_Byte(0x81,OLED_CMD); // contract control
    OLED_WR_Byte(0xFF,OLED_CMD);//--128   
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
    OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00,OLED_CMD);//

    OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
    OLED_WR_Byte(0x80,OLED_CMD);//

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1,OLED_CMD);//

    OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12,OLED_CMD);//

    OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//

    OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14,OLED_CMD);//

    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
    HAL_Delay(10);
}

/********************************************
ͼƬ���
********************************************/
void fill_picture(u8 fill_Data)
{
	u8 m,n;
    for(m=0;m<8;m++)
    {
        OLED_WR_Byte(0xb0+m,0);		//page0-page1
        OLED_WR_Byte(0x00,0);		//low column start address
        OLED_WR_Byte(0x10,0);		//high column start address
        for(n=0;n<128;n++)
        {
            OLED_WR_Byte(fill_Data,1);
        }
    }
}

/********************************************
��������
********************************************/
void OLED_Set_Pos(u8 x, u8 y) 
{ 	
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 

/********************************************
����OLED��ʾ
********************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/********************************************
�ر�OLED��ʾ  
********************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

/********************************************
��������,������,������Ļ�Ǻ�ɫ��!��û����һ��
********************************************/
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);     //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);       //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);       //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}                                       //������ʾ
}

/********************************************
����OLED��ʾ  
********************************************/
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);     //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);       //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);       //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	}                                       //������ʾ
}

/********************************************
��ָ��λ����ʾһ���ַ�,���������ַ�
x:0~127
y:0~63
mode:0,������ʾ;1,������ʾ				 
size:ѡ������ 16/12 
********************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	u8 c=0,i=0;	
    c=chr-' ';//�õ�ƫ�ƺ��ֵ			
    if(x>Max_Column-1){x=0;y=y+2;}
    if(Char_Size ==16)
    {
        OLED_Set_Pos(x,y);	
        for(i=0;i<8;i++)
        {
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); 
        }
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        {
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);  
        }
    }
    else 
    {	
        OLED_Set_Pos(x,y);
        for(i=0;i<6;i++)
        OLED_WR_Byte(F6x8[c][i],OLED_DATA);
    }
}

/********************************************
m^n����
********************************************/
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/********************************************
��ʾ2������
x,y :�������	 
len :���ֵ�λ��
size:�����С
mode:ģʽ	0,���ģʽ;1,����ģʽ
num:��ֵ(0~4294967295);	 
********************************************/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 


/********************************************
��ʾ�ַ�
x,y :�������	 
chr :�ַ��׵�ַ
size:�����С 12/16 
********************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	u8 j=0;
    while (chr[j]!='\0')
    {		
        OLED_ShowChar(x,y,chr[j],Char_Size);
        x+=8;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

/********************************************
��ʾ����
********************************************/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
            OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
            adder+=1;
        }	
		OLED_Set_Pos(x,y+1);	
        for(t=0;t<16;t++)
        {	
            OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
            adder+=1;
        }					
}

/********************************************
��ʾ��ʾBMPͼƬ128��64
��ʼ������(x,y)
x�ķ�Χ0��127
yΪҳ�ķ�Χ0��7
********************************************/
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
{ 	
 unsigned int j=0;
 u8 x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
    {      
        OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
    }
	}
} 
