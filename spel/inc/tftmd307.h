/*
*	tftmd307.h
*	Header for building the tft-library
*	Roger Johansson
*	Version 2025-10-28
*/
#ifndef _TFT_MD307_H_
#define _TFT_MD307_H_

#include	<stdint.h>


#define FONT_7DIGIT_11x25	1
#define FONT_ARIAL_NARROW_BOLD_24		2
#define	FONT_MAX			FONT_ARIAL_NARROW_BOLD_24
#define	FONT_DEFAULT		FONT_ARIAL_NARROW_BOLD_24	

#define GEO_LINE			1
#define GEO_RECT			2
#define GEO_FILLED_RECT		3
#define GEO_ELLIPSE			4
#define GEO_FILLED_ELLIPSE	5

#define TFT_INIT_ILI9488	1
#define	TFT_INIT_ST7796S	2
#define	TFT_INIT_ILI9486	4
#define	TFT_INIT_TOUCH		0x10

/* Some RGB-colours to play around with... */
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

/* Public functions */
void	delay_ms(unsigned int ms);
void	delay_us(unsigned int us);

/* tft library functions */
int 	tft_tp_scan(int tp);
int 	tft_tp_getpos( int *x, int *y );
int		tft_init(int);
void 	tft_lcd_pixel(int x,int y, int colour);
void 	tft_lcd_crosshair(int x,int y,int colour);
void 	tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill);
void 	tft_lcd_rect(int x1, int y1, int x2, int y2, int colour, int fill);
void 	tft_lcd_line(int  x1, int y1, int x2, int y2, int colour);
void 	tft_printf (int font, uint16_t x, uint16_t y, uint16_t bg, uint16_t fg, char * formatstr, ...);

#ifndef DBGCALL
/* rest is when building from code ... */
static int 	_tft_tp_scan(int tp);
static int 	_tft_tp_getpos( int *x, int *y );
static int		_tft_init(int);
static void 	_tft_lcd_pixel(int x,int y, int colour);
static void 	_tft_lcd_crosshair(int x,int y,int colour);
static void 	_tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill);
static void 	_tft_lcd_rect(int x1, int y1, int x2, int y2, uint16_t colour, int fill);
static void 	_tft_lcd_line(int  x1, int y1, int x2, int y2, int colour);
static void	_delay_ms(unsigned int ms);

#ifndef CUSTOM
/* Trap call interface */
typedef struct tft_call{
	int	function;
	unsigned int *caller_frame;
} TFT_CALL, *PTFT_CALL;
#endif

#define USE_HORIZONTAL  	 1
#define LCD_W 480
#define LCD_H 320

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t x;
	uint16_t y;						   	    
	uint8_t  sta;
}_m_tp_dev;

typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t  dir;				
	uint16_t wramcmd;		
	uint16_t setxcmd;		
	uint16_t setycmd;		
}_lcd_dev; 	

 typedef struct {
     const uint8_t *data;
     uint8_t dataSize;
     } tImage;
 typedef struct {
     long int code;
     const tImage *image;
     const int char_width;
     const int char_height;
     } tChar;
 typedef struct {
     int length;
     const tChar *chars;
     } tFont;

extern const tFont font_arial_narrow_bold_24;

/* Interface wiring default board */

#define	LCD_CS_CLR  (*GPIO_OUTDR(GPIO_B) &= ~(1<<15))	/* Clear PB15 */
#define	LCD_CS_SET  (*GPIO_OUTDR(GPIO_B) |= (1<<15))   	/* Set PB15 */

#define	LCD_RS_CLR  (*GPIO_OUTDR(GPIO_B) &= ~(1<<14))	/* Clear PB14 */
#define	LCD_RS_SET  (*GPIO_OUTDR(GPIO_B) |= (1<<14))   	/* Set PB14 */

#define	LCD_LED_CLR  (*GPIO_OUTDR(GPIO_B) &= ~(1<<13))	/* Clear PB13 */
#define	LCD_LED_SET  (*GPIO_OUTDR(GPIO_B) |= (1<<13))   	/* Set PB13 */

#define	LCD_RST_CLR  (*GPIO_OUTDR(GPIO_B) &= ~(1<<12))	/* Clear PB12 */
#define	LCD_RST_SET  (*GPIO_OUTDR(GPIO_B) |= (1<<12))   	/* Set PB12 */

#define	PEN  	( (*GPIO_INDR(GPIO_C)) &(1<<0))	/* T_IRQ, PC0 */
#define	DOUT  	( (*GPIO_INDR(GPIO_C)) &(1<<1))	/* T_DO (MISO), PC1 */

#define TDIN_SET 	(*GPIO_OUTDR(GPIO_C) |= (1<<2))	/* T_DI (MOSI), PC2 */
#define TDIN_CLR 	(*GPIO_OUTDR(GPIO_C) &= ~(1<<2)) 	

#define TCS_SET 	(*GPIO_OUTDR(GPIO_C) |= (1<<3))	/* T_CS, PC3 */
#define TCS_CLR 	(*GPIO_OUTDR(GPIO_C) &= ~(1<<3))	

#define TCLK_SET 	((*GPIO_OUTDR(GPIO_C)) |= (1<<4))	/* T_CLK, PC4 */
#define TCLK_CLR 	((*GPIO_OUTDR(GPIO_C)) &= ~(1<<4))	

#define TP_PRES_DOWN 0x80 
#define TP_CATH_PRES 0x40   

static void _tft_lcd_direction(int direction);
//int		tft_clock_init_144(void);

#endif	/* not DBGCALL */

#endif	/* _TFT_MD307_H_ */