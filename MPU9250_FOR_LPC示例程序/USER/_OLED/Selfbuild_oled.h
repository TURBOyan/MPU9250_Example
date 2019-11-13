#ifndef __SELFBUILD_OLED_H__
#define __SELFBUILD_OLED_H__
#include "headfile.h"

#define byte  char
#define uint unsigned int

#define SCL_PIN OLED_SCL_PIN
#define SDA_PIN OLED_SDA_PIN
#define RST_PIN OLED_RST_PIN 
#define DC_PIN  OLED_DC_PIN  

#define INIT 0

#define OLED_X_MAX 				128
#define OLED_Y_MAX 				64

#define LED_SCL_Init  gpio_init(SCL_PIN,GPO,0,PULLUP);;// 时钟初始化定义   
       
#define LED_SDA_Init  gpio_init(SDA_PIN,GPO,0,PULLUP);;//数据口D0

#define LED_RST_Init  gpio_init(RST_PIN,GPO,1,PULLUP);;//复位低能电平

#define LED_DC_Init   gpio_init(DC_PIN,GPO,1,PULLUP);;//偏置常低

#define LED_SCLH  gpio_set (SCL_PIN, 1); // 时钟定义 
#define LED_SCLL  gpio_set (SCL_PIN, 0);

#define LED_SDAH  gpio_set (SDA_PIN, 1);//数据口D0
#define LED_SDAL  gpio_set (SDA_PIN, 0);

#define LED_RSTH  gpio_set (RST_PIN, 1);//复位低能电平
#define LED_RSTL  gpio_set (RST_PIN, 0);

#define LED_DCH   gpio_set (DC_PIN, 1);
#define LED_DCL   gpio_set (DC_PIN, 0);//偏置常低

/************************************************/

void  LEDPIN_Init(void);   //LED控制引脚初始化

void  OLED_Init(void);

 void LED_CLS(void);
 void LED_WrDat(uint8 data);
 void LED_WrCmd(uint8 cmd); //写命令函数
 void LED_Set_Pos(byte x, byte y);
 void LED_P6x8Char(byte x,byte y,byte ch);
void LED_P6x8Str(byte x,byte y,const char* ch);
void OLED_P6x8Int(uint8_t x, uint8_t y, int16_t data1, int8 set);
void OLED_P6x8Flo(uint8_t x, uint8_t y, float data1, int8 set);
 void LED_P8x16Char(byte x,byte y,byte ch);
 void LED_P8x16Str(byte x,byte y,byte ch[]);
 
 void  LED_P14x16Str(byte x,byte y,byte ch[]);
 
 void LED_PrintBMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
 
 void LED_Fill(byte dat);
 
 void LED_PrintValueC(uint8 x, uint8 y,char data);
 void LED_PrintValueI(uint8 x, uint8 y, int data);
 void LED_PrintValueF(uint8 x, uint8 y, float data, uint8 num);

 void LED_Cursor(uint8 cursor_column, uint8 cursor_row);
 void LCD_PutPixel(byte x,byte y);
 void Image_set(byte x,byte y);
 void Draw_BinarySet(byte x,byte y,uint8* video,uint8 binary);
 void Image_clear(void);
 void LCD_PrintImage(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum);
 extern char logo[];

#endif
