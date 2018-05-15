#ifndef __Lcdlib__
#define __Lcdlib__
#include "public.h"
/*--------------------------------------------------------------------------
// �ļ�����Lcdlib.h
// ������  LCD��ͷ�ļ�
// ����ߣ�EU����
// �������ڣ�2013��10��16��
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
typedef unsigned char       u8;
typedef unsigned short      u16;

/*-------------------------------------------------------------------------------------------------------
*  ��������					 
-------------------------------------------------------------------------------------------------------*/
//��ɫֵ
#define BLACK          0x0000   //��

#define RED            0xF800   //��  
#define ORANGE 		   	 0xFB00   //��
#define YELLOW         0xFFE0   //��
#define GREEN          0x07E0   //��
#define BLUE           0x001F   //��
#define PURPLE		   	 0x881F   //��
#define GRAY  		     0X8430   //��
#define WHITE          0xFFFF   //��

#define GOLDEN         0XBC40   //��
#define LIGHTBLUE      0x051F   //ǳ��
#define MAGENTA        0xF81F   //����
#define CYAN           0x7FFF   //��
//----- RGB565ԭ�� ------
//xxxxxxxx xxxxxxxx
//rrrrrggg gggbbbbb
//11111000 00000000 �� 0xF800
//00000111 11100000 �� 0x07E0
//00000000 00011111 �� 0x001F

//lcd direction
#define  LCD_DIRECT        1           		   //0 ����  1 ����

//lcd bus chose
#define  LCD_BUSTYPE       1                 //1:16λ����  2:8λ����

#include <string.h>
#include <stdio.h>
#include <stddef.h> 

//+++++++++++++++++++++++++++ Numbers Define  ++++++++++++++++++++++++++++++++++//
#define  True  1
#define  False 0
#define  ZERO 		   	0
#define  ONE         	1
#define  TWO         	2
#define  THREE       	3
#define  FOUR        	4
#define  FIVE        	5
#define  SIX         	6
#define  SEVEN       	7
#define  EIGHT       	8
#define  NINE        	9
#define  TEN         	10
#define  HUNDRED     	100
#define  THOUSAND    	1000
#define  HALFBYTEMAX 	15
#define  ONEBYTEMAX  	255

//+++++++++++++++++++++++++++ Type Declaration ++++++++++++++++++++++++++++++++++//
//!You should modify it for different c compiler.


//+++++++++++++++++++++++++++ Bit Computing ++++++++++++++++++++++++++++++++++//
//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

//+++++++++++++++++++++++++++++ Others ++++++++++++++++++++++++++++++++++++//
//TAST RUN STEPS.
enum
{
	STEP0 = 0,         STEP1 = 1,         STEP2 = 2,         STEP3 = 3,
	STEP4 = 4,         STEP5 = 5,         STEP6 = 6,         STEP7 = 7,
	STEP8 = 8,         STEP9 = 9,         STEP10 = 10,       STEP11 = 11,
	STEP12 = 12,       STEP13 = 13,       STEP14 = 14,       STEP15 = 15,
	STEP16 = 16,       STEP17 = 17,       STEP18 = 18,       STEP19 = 19,
	STEP20 = 20,       STEP21 = 21,       STEP22 = 22,       STEP23 = 23,
	STEP24 = 24,       STEP25 = 25,       STEP26 = 26,       STEP27 = 27,
	STEP28 = 28,       STEP29 = 29,       STEP30 = 30,       STEP31 = 31,
	STEP32 = 32,       STEP33 = 33,       STEP34 = 34,       STEP35 = 35
};
/*-------------------------------------------------------------------------------------------------------
*  ����ӿ�								 
-------------------------------------------------------------------------------------------------------*/
//------------------------- APP USER LIB -----------------------------
//lcd��ʼ��
extern void LCD_ili9341Init(void);
//lcd����Ļ
extern void LCD_ClrScr(u16 BackColor);

//--------------------------------------------------------------------
//---------------- ��Ļ���Ͻ�Ϊԭ��,xָ��, yָ�� ---------------------
//--------------------------------------------------------------------
//------------------------- �� -----------------------------
//��x0,y0λ����ʾһ����ɫΪColor��
extern void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);
extern u16 LCD_ReadAPoint(u16 x0, u16 y0);

//------------------------- �� -----------------------------
//��x0,y0λ����ʾһ��ֱ��(dir:����1/����2),����Ϊlenth,��ɫΪcolor
extern void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 lenth, u16 color);
//��x0,y0 ~ �Խ�x1,y1,��һ����ɫΪColor�ľ��α߿�
extern void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
//��x0,y0λ�û�һ���뾶Ϊr,��ɫΪColor��Բ��
extern void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);

//------------------------- �� -----------------------------
//��x0,y0λ�ÿ�ʼ,��ʾһ�����Ϊwide,��ɫΪColor��������
extern void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);   
//��x0,y0 ~ x1,y1λ�ô�,��ʾһ����ɫΪColor�ĳ�����
extern void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);

//--------------------- Ӣ���ַ� ---------------------------
//��x0,y0λ����ʾһ��ASC�ַ�,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisASCString16x8(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//--------------------- �����ַ� ---------------------------
//��x0,y0λ����ʾһ�����ַ���,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisGB2312String16x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//��x0,y0λ����ʾһ�����ַ���,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisGB2312String32x32(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//------------------------ ͼƬ -----------------------------
//��x0,y0λ����ʾһ����Ϊhigh,��Ϊwide��ͼƬ, ͼƬ��Ϣ��RGB565��֯(��˸�ʽÿ�����ص�ռ��2���ֽ�)
//ͼƬ��Ϣͷ��ַָ��ΪpData
extern void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData);
extern void LCD_DisASPIPhoto(u16 x0, u16 y0, u16 high, u16 wide, u32 SPIAddress);  /* ��SPI-FLASH */
/**************************/
void LoadTouchFaceVal(void);
u8 GetScrSite(u16 X, u16 Y, u16 *Row, u16 *Column);
void Touch_SPIWrite(u8 Val);
u16 Touch_SPIRead(void);
void Touch_GetVal(u16 *X, u16 *Y);
int Touch_GetSite(u16 *pRow, u16 *pColumn);
/************************/


#endif



