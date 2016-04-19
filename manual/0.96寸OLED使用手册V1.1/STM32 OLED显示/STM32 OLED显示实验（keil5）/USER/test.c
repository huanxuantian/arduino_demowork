#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "oled.h"
#include "picture.h" 
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：0.96'OLED驱动例程
Author：pinggai    Version:1.0     Data:2015/11/10
Description: 
Others:
History：
**********************************************************/
int main(void)
{	
	u8 i;	  	
 	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,9600);  //串口初始化 
	LED_Init();		  	 //初始化与LED连接的硬件接口											   
	OLED_SetInit();			//初始化液晶      
    OLED_Clear();					  
 	LED = 0;
	while(1) 
	{		
		OLED_Clear();
		OLED_Bright();	
		OLED_Refresh_Gram();
		delay_ms(1500);

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"ＹＦＲＯＢＯＴ",1);//GB2312简体中文中字母，注意它的格式和ASCII
		OLED_ShowGB2312String(0,16,"YFROBOT",1);   //在任意点显示8*16点ASCII字符串
		OLED_Show8X16BoldString(0,32,"YFROBOT",1); //在任意点显示8*16点ASCII粗体字符串
		OLED_Show7X8String(0,48,"YFROBOT",1);	   //在任意点显示7*8点ASCII字符串
		OLED_Show5X7String(0,57,"YFROBOT",1);	   //在任意点显示5*7点ASCII字符串
		OLED_Refresh_Gram();
		delay_ms(1500);

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"故人西辞黄鹤楼，",1);
		OLED_ShowGB2312String(0,16,"烟花三月下扬州。",0);
		OLED_ShowGB2312String(0,32,"孤帆远影碧空尽，",1);
		OLED_ShowGB2312String(0,48,"唯见长江天际流。",0);
		OLED_Refresh_Gram();
		delay_ms(1500);
		delay_ms(1500);	

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"12345",0);   //在任意点显示8*16点ASCII字符串
		OLED_ShowNum(0,16,12345,3,1);
		OLED_ShowNum(0,32,12345,5,1);
		OLED_ShowNum(0,48,12345,7,1);
		OLED_Refresh_Gram();
		delay_ms(1500);

		OLED_Clear();
		OLED_Picture( 25, 0, 60, 64, YFpicture, 1);
	 	OLED_Show5X7String(72,57,"YFROBOT",1);
		OLED_Refresh_Gram();
		delay_ms(1500);

		for(i=0;i<48;i++)
		{	
			OLED_Clear();
			OLED_ShowGB2312String(127-i,47-i,"YFRobot工作室",1);
			OLED_ShowGB2312String(127-i,63-i,"聚芯智创科技",1);
			OLED_Refresh_Gram();
			delay_ms(80);
		}
		for(i=0;i<81;i++)
		{	
			OLED_ShowGB2312String(80-i,0,"YFRobot工作室",1);
			OLED_ShowGB2312String(80-i,16,"聚芯智创科技",1);
			OLED_Refresh_Gram();
			delay_ms(80);
		}
		delay_ms(1000);
		delay_ms(1000);

	}	  
}






