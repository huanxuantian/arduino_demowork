#include "oled.h"
#include "stdlib.h"	 
#include "delay.h"
#include "usart.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File�� oled.c
Author��pinggai    Version:1.0     Data:2015/11/10
Description:  oled��������,���ֿ�оƬ�� 4�ߴ��ڣ�SPI��
Others:
History��
**********************************************************/

/******************************************************
Function:	 void OLED_Init(void)
Description: �����˿ڳ�ʼ����OLED��GT20����
             C8	 (���) -- CLK	  ʱ��
			 C9	 (���) -- DIN	  ����	SSD1306���롢GT20L16����
			 C10 (���) -- D/S	  ����/����
			 C11 (���) -- CS1	  SSD1306Ƭѡ
			 C12 (����) -- SO	  �������룬GT20L16���
			 C13 (���) -- CS2	  GT20L16Ƭѡ
Calls by:	 void OLED_SetInit(void) 
******************************************************/	
void OLED_Init(void)
{ 	 				 	 					    
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ�� 	  
 	GPIOC->CRH&=0XFF000000;	   
 	GPIOC->CRH|=0X00383333;	
	 
	GPIOC->ODR|=0XF<<8;	   //�ߵ�ƽ
	GPIOC->ODR&=~(1<<12);
}

//OLED���Դ�
//��Ÿ�ʽ����.
//PAGE0 [0]0 1 2 3 ... 127		  λ�ߵ�˳��  D0
//PAGE1 [1]0 1 2 3 ... 127		              D1
//PAGE2 [2]0 1 2 3 ... 127		              D2
//PAGE3 [3]0 1 2 3 ... 127		              D3
//PAGE4 [4]0 1 2 3 ... 127		              D4
//PAGE5 [5]0 1 2 3 ... 127		              D5
//PAGE6 [6]0 1 2 3 ... 127		              D6
//PAGE7 [7]0 1 2 3 ... 127 		              D7 
u8 OLED_GRAM[128][8];	 

/******************************************************
Function:	 void OLED_Refresh_Gram(void)
Description:  �����Դ浽OLED����OLED_GRAM�е����ݸ��µ�
              SSD1306��Graphic Display Data RAM (GDDRAM)
              ÿ����ʾ������ʱ����Ҫ���ô˺�����
******************************************************/		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WCMD(0xB0+i);	 //����ҳ��ַ��0~7��ÿҳ�е�GDDRAM�е�ַ���Զ����ӵġ�
		OLED_WCMD(0x00);	 //������ʾλ�á��е͵�ַ
		OLED_WCMD(0x10);	 //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++) 
			OLED_WDAT(OLED_GRAM[n][i]); //#define OLED_DATA 1	//д����
	}   
}
/******************************************************
Function:	 void OLED_WCMD(u8 cmd)
Description: ��SSD1306 д����
Input:		 cmd��Ҫд�������
Others:		 OLED_DC=0����ʾд���=1д����
******************************************************/	
void OLED_WCMD(u8 cmd)
{	
	u8 i;			  
	OLED_DC=0; //д���� 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_CLK=0;
		if(cmd&0x80)OLED_DIN=1;
		else OLED_DIN=0;
		OLED_CLK=1;
		cmd<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_DC=1;   	  
}
/******************************************************
Function:	 void OLED_WDAT(u8 dat)
Description: ��SSD1306 д����
Input:		 dat��Ҫд�������
Others:		 OLED_DC=0����ʾд���=1д����
******************************************************/
void OLED_WDAT(u8 dat)
{	
	u8 i;			  
	OLED_DC=1; //д���� 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_CLK=0;
		if(dat&0x80)OLED_DIN=1;
		else OLED_DIN=0;
		OLED_CLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_DC=1;   	  
}
//����OLED��ʾ
void OLED_Display_On(void)
{
	OLED_WCMD(0x8D);   //SET DCDC����
	OLED_WCMD(0x14);   //DCDC ON
	OLED_WCMD(0xAF);   //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WCMD(0x8D);	 //SET DCDC����
	OLED_WCMD(0x10);	 //DCDC OFF
	OLED_WCMD(0xAE);	 //DISPLAY OFF
}
//OLED_GRAM[128][8]д0
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
	    for(n=0;n<128;n++)
	        OLED_GRAM[n][i]=0X00;  
//	OLED_Refresh_Gram();//������ʾ
}
//ȫ������
void OLED_Bright(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
	    for(n=0;n<128;n++)
	        OLED_GRAM[n][i]=0XFF;  
	OLED_Refresh_Gram();//������ʾ
}
/******************************************************
Function:	 void OLED_DrawPoint(u8 x,u8 y,u8 t)
Description: ����Ļ����һλ�û��㣬�������
Input:	  x��0~127
		  y:0~63
		  t:1 ��� 0,���
******************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

/******************************************************
Function:	 void OLED_Init(void)
Description: �����˿ڳ�ʼ����SSD1306��ʼ����
Calls:		 void OLED_Init(void);	  //�����˿ڳ�ʼ��
			 void OLED_WCMD(u8 cmd)�� //��SSD1306д����
******************************************************/				    
void OLED_SetInit(void)
{ 	 				 	 					    
	OLED_Init();		//�˿ڳ�ʼ��				  

	OLED_WCMD(0xAE);    //�ر���ʾ
	OLED_WCMD(0xD5);    //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WCMD(0x50);	    //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WCMD(0xA8);    //��������·��
	OLED_WCMD(0x3F);        //Ĭ��0X3F(1/64)
	
	OLED_WCMD(0xD3);    //������ʾƫ��
	OLED_WCMD(0x00);       //Ĭ��Ϊ0
	OLED_WCMD(0x40);    //������ʾ��ʼ�� [5:0],����.
	OLED_WCMD(0x8D);    //��ɱ�����
	OLED_WCMD(0x14);    //bit2������/�ر�
	
	OLED_WCMD(0x20);	//�����ڴ�Ѱַģʽ
	OLED_WCMD(0x02);	//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;	
	OLED_WCMD(0xA1);	//���ض�������,bit0: 0,column 0->SEG 0;  1,column 0->SEG 127;
	OLED_WCMD(0xC8);	//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WCMD(0xDA);	//����COMӲ����������  [5:4]����  ����COM��SSD1306����ROW��Һ���������ӹ�ϵ
	OLED_WCMD(0x12);        //[5:4]����  ����COM��SSD1306����ROW��Һ���������ӹ�ϵ
	
	OLED_WCMD(0x81);	//�Աȶ�����
	OLED_WCMD(0xEF);	//1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WCMD(0xD9);	//����Ԥ�������
	OLED_WCMD(0xF1);	//[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WCMD(0xDB);	//����VCOMH ��ѹ����
	OLED_WCMD(0x30);	//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WCMD(0xA4);	//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WCMD(0xA6);	//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLED_WCMD(0xAF);	//������ʾ

	OLED_Clear();		//����
}



/******************************************************
Function:	  void GT20_WCMD(u8 cmd)
Description:  ��GT20L16д����  
Input:		  u8 cmd �����
Others:		  ʱ�������أ���������GT20
******************************************************/
void GT20_WCMD(u8 cmd)
{
	u8 i;			  		  
	for(i=0;i<8;i++)
	{	
		GT20_CLK=0;	  		
		if(cmd&0x80) GT20_SI=1;
		else GT20_SI=0;		
		GT20_CLK=1;
		cmd<<=1; 
	}				 
}

/******************************************************
Function:	  static u8 GT20_RDAT( )
Description:  ��GT20L16���ֿ�ĺ��ֻ��ַ�����
Output:		  data һ���ֽڣ�������ʾ���ֻ��ַ�
Others:		  ʱ���½��أ���GT20����
******************************************************/
static u8 GT20_RDAT(void)
{
	u16 i;
	u8 data=0x00;
	GT20_CLK = 1;	
	for(i=0;i<8;i++)
	{
		GT20_CLK = 1; 
		data = data<<1;
		GT20_CLK = 0;					
		if(GT20_SO)
			data = data+1;			
	}
	return data;
}
/******************************************************
Function:	 void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen)
Description: ��ȡ������ģ
Calls:		 void GT20_WCMD(u8 cmd)		//��GT20L16д���� 
         	 static u8 GT20_RDAT(void)	//��GT20L16���ֿ�ĺ��ֻ��ַ�����
Input:	 u8 addrH  �ߵ�ַ
		 u8 addrM  �е�ַ
		 u8 addrL  �͵�ַ
		 u8 *fontBuf  ����洢ָ��
		 u8 dataLen	  ���ֻ��ַ� ������ģ�ֽڸ���
Others:	���ֺ��ַ��洢���ֽڲ�ͬ������ʵ����������޸�
******************************************************/	
void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen)
{
	u8 i;
	GT20_CS = 1;
	OLED_CS = 1;
	GT20_CS = 0;   //Ƭѡ �͵�ƽΪ��Ч��ƽ		
	GT20_CLK = 0;
	GT20_WCMD(0x03);  //ͨѶ��ʼ�����֣�03H��
	GT20_WCMD(addrH);
	GT20_WCMD(addrM);
	GT20_WCMD(addrL);
	for(i=0;i<dataLen; i++)
	{
		*(fontBuf+i) = GT20_RDAT( );
	}
	delay_us(2);	
	GT20_CS = 1;	 
}

/******************************************************
Function:	void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode)
Description:������㿪ʼ��ָ���С��ͼƬ
Calls�� void OLED_DrawPoint(u8 x,u8 y,u8 t) //�������껭��
Input:	u8 x��u8 y����ʼ�㡣
        u8 len������x�᷽��
		u8 wide����y�᷽�� 
		u8 *pic��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode)
{
	u8 x_pos,y_pos,temp;
	u8 i,j;
	x_pos = x;
	y_pos = y;
	for(j=0; j<len; j++)
	{
		for(i=0;i<wide;i++)
		{
		    if(i%8==0)
			{
				temp = *(pic+(i/8)*len+j);
			}
			if(temp&0x01)	OLED_DrawPoint( x_pos+j,y_pos+i,mode);
			else	OLED_DrawPoint( x_pos+j,y_pos+i,!mode);		  //ĳ���㡱д0����д1
			temp >>=1;			
		}
	}
}
u32	 FONTADDR=0; //GT20�еĴ洢��ַ
/******************************************************
Function:	void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode)
Description: ���������ʾGB2312�ַ���
Calls�� void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ
Input:	u8 x��u8 y����ʼ�㡣 
		u8 *text��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_ShowGB2312String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
//	u8 K; 
	u8 addrH, addrM, addrL;
	u8 FONTBUF[32];
	while(text[i]>0x00)
	{
//		printf("%X,%X\r\n",text[i],text[i+1]);	//��������ַ�����
		//A9��
		if(((text[i]==0xA9))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312��������GT20�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = (282 + (LSB - 0xA1 ))*32+BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i+1]-0xA1)+282;
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//ȡ32���ֽڵ����ݣ��浽"FONTBUF[32]"
//			printf("%X,%X\r\n",text[i],text[i+1]);	//��������ַ�����
//			for(K=0;K<32;K++)				        //��������ַ�������Ϣ����
//				printf("%x��",FONTBUF[K]);
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i+=2;
			x+=16;
		}
		//A1~A3��
		else if(((text[i]>=0xA1) &&(text[i]<=0xA3))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312��������GT20�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0xA1)*94; 
			FONTADDR += (text[i+1]-0xA1);
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//ȡ32���ֽڵ����ݣ��浽"FONTBUF[32]"
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i+=2;
			x+=16;
		}
		//B0~F7��
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312��������GT20�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0xB0)*94; 
			FONTADDR += (text[i+1]-0xA1)+846;
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//ȡ32���ֽڵ����ݣ��浽"FONTBUF[32]"
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i+=2;
			x+=16;
		}
		//8*16��ASSII�ַ�
		else if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16��ASCII��GT20�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = (ASCIICode �C0x20 ) * 16+BaseAdd;BaseAdd=0x3B7C0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0x20)*16+0x3B7C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//ȡ32���ֽڵ����ݣ��浽"FONTBUF[32]"
			OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i++;
			x+=8;
		}
		else
		i++;
	}			
}
/******************************************************
Function:	 void OLED_Show8*16BoldString(u8 x,u8 y,u8 *p,u8 mode)
Description: ���������ʾ8*16��ASCII�����ַ���
Calls�� void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ
Input:	u8 x��u8 y����ʼ�㡣 
		u8 *text��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_Show8X16BoldString(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[16];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16��ASCII�������ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = (ASCIICode �C0x20 ) * 16+BaseAdd;BaseAdd=0x3CF80*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0x20)*16+0x3CF80; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//ȡ16���ֽڵ����ݣ��浽"FONTBUF[16]"
			OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i++;
			x+=8;
		}
		else 
			i++;
	}
}
/******************************************************
Function:	 void OLED_Show7*8String(u8 x,u8 y,u8 *p,u8 mode)
Description: ���������ʾ7*8��ASCII�ַ���
Calls�� void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ
Input:	u8 x��u8 y����ʼ�㡣 
		u8 *text��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_Show7X8String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[7];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*7*8��ASCII���ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = (ASCIICode �C0x20 ) * 8+BaseAdd;  BaseAdd=0x66C0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0x20)*8+0x66C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,7);//ȡ7���ֽڵ����ݣ��浽"FONTBUF[7]"
			OLED_GT20Photo( x, y, 7, 8, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i++;
			x+=7;
		}
		else 
			i++;
	}
}

/******************************************************
Function:	 void OLED_Show5*7String(u8 x,u8 y,u8 *p,u8 mode)
Description: ���������ʾ5*7��ASCII�ַ���
Calls�� void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ
Input:	u8 x��u8 y����ʼ�㡣 
		u8 *text��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_Show5X7String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[5];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*5*7��ASCII���ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = (ASCIICode �C0x20 ) * 8+BaseAdd;  BaseAdd=0x3BFC0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			FONTADDR = (text[i]- 0x20)*8+0x3BFC0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
			addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
			addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,5);//ȡ5���ֽڵ����ݣ��浽"FONTBUF[32]"
			OLED_GT20Photo( x, y, 5, 7, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			i++;
			x+=5;
		}
		else 
			i++;
	}
}

/******************************************************
Function:	void OLED_Photo( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode)
Description:������㿪ʼ��ָ���С��ͼƬ
Calls�� void OLED_DrawPoint(u8 x,u8 y,u8 t) //�������껭��
Input:	u8 x��u8 y����ʼ�㡣
        u8 len������x�᷽��
		u8 wide����y�᷽�� 
		u8 *pic��ͼƬ����
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_Picture( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode)
{
	u8 x_pos,y_pos,temp;
	u8 i,j;
	x_pos = x;
	y_pos = y;
	for(j=0; j<len; j++)
	{
		for(i=0;i<wide;i++)
		{
		    if(i%8==0)
			{
				temp = *(pic+(i/8)*len+j);
			}
			if(temp&0x01)	OLED_DrawPoint( x_pos+j,y_pos+i,mode);
			else	OLED_DrawPoint( x_pos+j,y_pos+i,!mode);		  //ĳ���㡱д0����д1
			temp >>=1;			
		}
	}
}

//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//��ʾ��������
/******************************************************
Function:	 void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode)
Description: ������λ����ʾ��������
Calls��   void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //��ȡ������ģ
          void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //������㿪ʼ��ָ���С��ͼƬ
Calls by��void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);  //������λ����ʾ����
Input:	u8 x��u8 y����ʼ�㡣 
		u8 num������ 0��1��2��3��4��5��6��7��8��9
		u8 mode���ַ���ʾ����Ϊ0��1
******************************************************/
void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode)
{
	u8 addrH, addrM, addrL,temp;
	u8 FONTBUF[16];
	temp = num+'0';//	
	if((temp>=0x20)&&(temp<=0x7E))
	{						
		/*8*16��ASCII��GT20�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = (ASCIICode �C0x20 ) * 16+BaseAdd;BaseAdd=0x3B7C0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
		FONTADDR = (temp - 0x20)*16+0x3B7C0; 		
			
		addrH = (FONTADDR&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ
		addrM = (FONTADDR&0xff00)>>8;     //��ַ����8λ,��24λ
		addrL = FONTADDR&0xff;	         //��ַ�ĵ�8λ,��24λ
		GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//ȡ16���ֽڵ����ݣ��浽"FONTBUF[32]"
		OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //����GRAM, ��ʾΪ���
			
   }
  
}
/******************************************************
Function:	  void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
Description:  ������λ����ʾ����
Calls:		  void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //���������ʾGB2312�ַ���
Input:		  x,y :�������	 
              len :���ֵ�λ�� ���ܴ���10
			  num:��ֵ(0~4294967295);
              mode:ģʽ0,�����ģʽ;1,���ģʽ 
******************************************************/	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len-t-1))%10;	
		if(enshow==0 && t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowGB2312String(x+8*t,y," ",mode);
				continue;
			}
			else enshow=1; 		 	 
		}
		OLED_Show8X16Num(x+8*t,y,temp,mode); 
	}
}






