/*
*	dbg307.h
*	Declaration of library functions, constants etc...
*	Roger Johansson
*   Version 2026-03-11
*/

/* Library functions */

#ifndef	_DBG307_H
#define _DBG307_H

#define GEO_LINE			1
#define GEO_RECT			2
#define GEO_FILLED_RECT		3
#define GEO_ELLIPSE			4
#define GEO_FILLED_ELLIPSE	5

#define TFT_INIT_ILI9488	1
#define	TFT_INIT_ST7796S	2
#define	TFT_INIT_ILI9486	4
#define	TFT_INIT_TOUCH		0x10

/* 16 bit RGB */
#define TFT_WHITE       0xFFFF
#define TFT_BLACK      	0x0000	  
#define TFT_BLUE       	0x001F  
#define TFT_BRED        0XF81F
#define TFT_GRED		0XFFE0
#define TFT_GBLUE		0X07FF
#define TFT_RED         0xF800
#define TFT_MAGENTA     0xF81F
#define TFT_GREEN       0x07E0
#define TFT_CYAN        0x7FFF
#define TFT_YELLOW		0xFFE0
#define TFT_BROWN		0xBC40 
#define TFT_BRRED		0xFC07 
#define TFT_GRAY		0x8430 
#define TFT_DARKBLUE	0x01CF	
#define TFT_LIGHTBLUE	0x7D7C	
#define TFT_GRAYBLUE	0x5458 
#define TFT_LIGHTGREEN	0x841F 
#define TFT_LIGHTGRAY	0xEF5B 
#define TFT_LGRAY 		0xC618 
#define TFT_LGRAYBLUE	0xA651 
#define TFT_LBBLUE		0x2B12 

void dbg_delay_ms(unsigned int ms);

void dbg_restart(void);
void dbg_clockinit(void);
void dbg_portinit(void);
void dbg_outchar(unsigned char c);
unsigned char dbg_tstchar(void);

int dbg_tft_init(int);
void dbg_tft_lcd_pixel(int x,int y, int colour);
void dbg_tft_lcd_line(int  x1, int y1, int x2, int y2, int colour);
void dbg_tft_lcd_rect(int x1, int y1, int x2, int y2, int, int fill);
void dbg_tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill);
void dbg_tft_lcd_crosshair(int x,int y,int colour);
void dbg_delay_ms(unsigned int ms);
int dbg_tft_tp_scan(int tp);
int dbg_tft_tp_getpos( int *x, int *y );

#endif



