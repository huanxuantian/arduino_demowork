#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"
	
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File�� oled.h
Author��pinggai    Version:1.0     Data:2015/11/10
Description:  oled��������,���ֿ�оƬ�� 4�ߴ��ڣ�SPI��
Others:
History��
**********************************************************/    


		    						  
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CLK PCout(8)
#define OLED_DIN PCout(9)
#define OLED_DC  PCout(10)
#define OLED_CS  PCout(11)

#define GT20_CLK PCout(8)
#define GT20_SI  PCout(9)
#define GT20_SO  PCin(12)
#define GT20_CS  PCout(13)		     

//OLED�����ú���   
void OLED_WCMD(u8 cmd);	  //��SSD1306д����
void OLED_WDAT(u8 dat);	  //��SSD1306д����
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);//��OLED_GRAM�е����ݸ��µ�SSD1306��Graphic Display Data RAM (GDDRAM)		   
							   		    
void OLED_Init(void);	  //�����˿ڳ�ʼ��
void OLED_SetInit(void);  //OLED���ó�ʼ��
void OLED_Clear(void);
void OLED_Bright(void); //ȫ������
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);

void GT20_WCMD(u8 cmd);  //��GT20L16д����
static u8 GT20_RDAT(void);  //��GT20L16���ֿ�ĺ��ֻ��ַ�����
void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ

void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //���������ʾGB2312�ַ���
void OLED_Show8X16BoldString(u8 x,u8 y,u8 *text,u8 mode);//���������ʾ8*16��ASCII�����ַ���
void OLED_Show7X8String(u8 x,u8 y,u8 *text,u8 mode);	 //���������ʾ7*8��ASCII�ַ���
void OLED_Show5X7String(u8 x,u8 y,u8 *text,u8 mode);	 //���������ʾ5*7��ASCII�ַ���
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);	 //��ʾ����0~4294967295
void OLED_Picture( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode);	 //������㿪ʼ��ָ���С��ͼƬ
void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode);		//��ʾ��������

#endif  
	 



