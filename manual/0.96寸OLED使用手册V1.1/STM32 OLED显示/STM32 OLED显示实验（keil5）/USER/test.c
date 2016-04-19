#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "oled.h"
#include "picture.h" 
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��0.96'OLED��������
Author��pinggai    Version:1.0     Data:2015/11/10
Description: 
Others:
History��
**********************************************************/
int main(void)
{	
	u8 i;	  	
 	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,9600);  //���ڳ�ʼ�� 
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�											   
	OLED_SetInit();			//��ʼ��Һ��      
    OLED_Clear();					  
 	LED = 0;
	while(1) 
	{		
		OLED_Clear();
		OLED_Bright();	
		OLED_Refresh_Gram();
		delay_ms(1500);

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"�٣ƣңϣ£ϣ�",1);//GB2312������������ĸ��ע�����ĸ�ʽ��ASCII
		OLED_ShowGB2312String(0,16,"YFROBOT",1);   //���������ʾ8*16��ASCII�ַ���
		OLED_Show8X16BoldString(0,32,"YFROBOT",1); //���������ʾ8*16��ASCII�����ַ���
		OLED_Show7X8String(0,48,"YFROBOT",1);	   //���������ʾ7*8��ASCII�ַ���
		OLED_Show5X7String(0,57,"YFROBOT",1);	   //���������ʾ5*7��ASCII�ַ���
		OLED_Refresh_Gram();
		delay_ms(1500);

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"�������ǻƺ�¥��",1);
		OLED_ShowGB2312String(0,16,"�̻����������ݡ�",0);
		OLED_ShowGB2312String(0,32,"�·�ԶӰ�̿վ���",1);
		OLED_ShowGB2312String(0,48,"Ψ�������������",0);
		OLED_Refresh_Gram();
		delay_ms(1500);
		delay_ms(1500);	

		OLED_Clear();
		OLED_ShowGB2312String(0,0,"12345",0);   //���������ʾ8*16��ASCII�ַ���
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
			OLED_ShowGB2312String(127-i,47-i,"YFRobot������",1);
			OLED_ShowGB2312String(127-i,63-i,"��о�Ǵ��Ƽ�",1);
			OLED_Refresh_Gram();
			delay_ms(80);
		}
		for(i=0;i<81;i++)
		{	
			OLED_ShowGB2312String(80-i,0,"YFRobot������",1);
			OLED_ShowGB2312String(80-i,16,"��о�Ǵ��Ƽ�",1);
			OLED_Refresh_Gram();
			delay_ms(80);
		}
		delay_ms(1000);
		delay_ms(1000);

	}	  
}






