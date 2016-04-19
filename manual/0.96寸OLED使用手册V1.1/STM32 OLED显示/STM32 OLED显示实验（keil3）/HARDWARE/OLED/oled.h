#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"
	
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File： oled.h
Author：pinggai    Version:1.0     Data:2015/11/10
Description:  oled驱动代码,带字库芯片， 4线串口（SPI）
Others:
History：
**********************************************************/    


		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CLK PCout(8)
#define OLED_DIN PCout(9)
#define OLED_DC  PCout(10)
#define OLED_CS  PCout(11)

#define GT20_CLK PCout(8)
#define GT20_SI  PCout(9)
#define GT20_SO  PCin(12)
#define GT20_CS  PCout(13)		     

//OLED控制用函数   
void OLED_WCMD(u8 cmd);	  //向SSD1306写命令
void OLED_WDAT(u8 dat);	  //向SSD1306写数据
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);//将OLED_GRAM中的内容更新到SSD1306的Graphic Display Data RAM (GDDRAM)		   
							   		    
void OLED_Init(void);	  //驱动端口初始化
void OLED_SetInit(void);  //OLED设置初始化
void OLED_Clear(void);
void OLED_Bright(void); //全屏点亮
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);

void GT20_WCMD(u8 cmd);  //向GT20L16写命令
static u8 GT20_RDAT(void);  //读GT20L16中字库的汉字或字符数据
void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片

void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //在任意点显示GB2312字符串
void OLED_Show8X16BoldString(u8 x,u8 y,u8 *text,u8 mode);//在任意点显示8*16点ASCII粗体字符串
void OLED_Show7X8String(u8 x,u8 y,u8 *text,u8 mode);	 //在任意点显示7*8点ASCII字符串
void OLED_Show5X7String(u8 x,u8 y,u8 *text,u8 mode);	 //在任意点显示5*7点ASCII字符串
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);	 //显示数字0~4294967295
void OLED_Picture( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode);	 //从任意点开始画指点大小的图片
void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode);		//显示单个数字

#endif  
	 



