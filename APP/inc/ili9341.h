#ifndef __Lcdlib__
#define __Lcdlib__
#include "public.h"
/*--------------------------------------------------------------------------
// 文件名：Lcdlib.h
// 描述：  LCD库头文件
// 设计者：EU电子
// 创建日期：2013年10月16日
// 深圳EU电子出品-版权所有-翻版必究
// EU-热爱嵌入式开发
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
typedef unsigned char       u8;
typedef unsigned short      u16;

/*-------------------------------------------------------------------------------------------------------
*  向外声明					 
-------------------------------------------------------------------------------------------------------*/
//颜色值
#define BLACK          0x0000   //黑

#define RED            0xF800   //红  
#define ORANGE 		   	 0xFB00   //橙
#define YELLOW         0xFFE0   //黄
#define GREEN          0x07E0   //绿
#define BLUE           0x001F   //蓝
#define PURPLE		   	 0x881F   //紫
#define GRAY  		     0X8430   //灰
#define WHITE          0xFFFF   //白

#define GOLDEN         0XBC40   //金
#define LIGHTBLUE      0x051F   //浅蓝
#define MAGENTA        0xF81F   //样红
#define CYAN           0x7FFF   //青
//----- RGB565原理 ------
//xxxxxxxx xxxxxxxx
//rrrrrggg gggbbbbb
//11111000 00000000 红 0xF800
//00000111 11100000 绿 0x07E0
//00000000 00011111 蓝 0x001F

//lcd direction
#define  LCD_DIRECT        1           		   //0 横屏  1 竖屏

//lcd bus chose
#define  LCD_BUSTYPE       1                 //1:16位总线  2:8位总线

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
*  向外接口								 
-------------------------------------------------------------------------------------------------------*/
//------------------------- APP USER LIB -----------------------------
//lcd初始化
extern void LCD_ili9341Init(void);
//lcd清屏幕
extern void LCD_ClrScr(u16 BackColor);

//--------------------------------------------------------------------
//---------------- 屏幕左上角为原点,x指行, y指列 ---------------------
//--------------------------------------------------------------------
//------------------------- 点 -----------------------------
//在x0,y0位置显示一个颜色为Color点
extern void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);
extern u16 LCD_ReadAPoint(u16 x0, u16 y0);

//------------------------- 线 -----------------------------
//在x0,y0位置显示一条直线(dir:横向1/纵向2),长度为lenth,颜色为color
extern void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 lenth, u16 color);
//在x0,y0 ~ 对角x1,y1,画一个颜色为Color的矩形边框
extern void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
//在x0,y0位置画一个半径为r,颜色为Color的圆框
extern void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);

//------------------------- 面 -----------------------------
//在x0,y0位置开始,显示一个宽度为wide,颜色为Color的正方形
extern void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);   
//在x0,y0 ~ x1,y1位置处,显示一个颜色为Color的长方形
extern void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);

//--------------------- 英文字符 ---------------------------
//在x0,y0位置显示一个ASC字符,颜色为fColor,背景色为bColor
extern void LCD_DisASCString16x8(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//--------------------- 中文字符 ---------------------------
//在x0,y0位置显示一汉字字符串,颜色为fColor,背景色为bColor
extern void LCD_DisGB2312String16x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//在x0,y0位置显示一汉字字符串,颜色为fColor,背景色为bColor
extern void LCD_DisGB2312String32x32(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//------------------------ 图片 -----------------------------
//在x0,y0位置显示一副高为high,宽为wide的图片, 图片信息按RGB565组织(大端格式每个像素点占用2个字节)
//图片信息头地址指针为pData
extern void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData);
extern void LCD_DisASPIPhoto(u16 x0, u16 y0, u16 high, u16 wide, u32 SPIAddress);  /* 从SPI-FLASH */
/**************************/
void LoadTouchFaceVal(void);
u8 GetScrSite(u16 X, u16 Y, u16 *Row, u16 *Column);
void Touch_SPIWrite(u8 Val);
u16 Touch_SPIRead(void);
void Touch_GetVal(u16 *X, u16 *Y);
int Touch_GetSite(u16 *pRow, u16 *pColumn);
/************************/


#endif



