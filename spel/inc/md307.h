/*
*	md307.h
*	macros and types for MD307 RISC-V lab system
*	Roger Johansson
*	Version 2025-10-28
*/
#ifndef _MD307_H_
#define _MD307_H_

#include <stdint.h>

/* Constants required by c runtime */
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* ********* RCC definitions ********* */
#define RCC_CTLR 		((volatile unsigned int *) 0x40021000) /* Clock control register */
#define RCC_CFGR0 		((volatile unsigned int *) 0x40021004) /* Clock configuration register */
#define RCC_INTR 		((volatile unsigned int *) 0x40021008) /* Clock interrupt register */
#define RCC_APB2PRSTR 	((volatile unsigned int *) 0x4002100C) /* APB2 peripheral reset register */
#define RCC_APB1PRSTR 	((volatile unsigned int *) 0x40021010) /* APB1 peripheral reset register */
#define RCC_AHBPCENR 	((volatile unsigned int *) 0x40021014) /* AHB peripheral clock enable register */
#define RCC_APB2PCENR 	((volatile unsigned int *) 0x40021018) /* APB2 peripheral clock enable register */
#define RCC_APB1PCENR 	((volatile unsigned int *) 0x4002101C) /* APB1 peripheral clock enable register */
#define RCC_BDCTLR 		((volatile unsigned int *) 0x40021020) /* Backup domain control register */
#define RCC_RSTSCKR 	((volatile unsigned int *) 0x40021024) /* Control/status register */
#define RCC_AHBRSTR 	((volatile unsigned int *) 0x40021028) /* AHB peripheral reset register */
#define RCC_CFGR2 		((volatile unsigned int *) 0x4002102C) /* Clock configuration register 2 */

typedef	struct	tagRccWchdevice {
	volatile uint32_t ctlr;			/* 0x00 */
	volatile uint32_t cfgr0;		/* 0x04 */
	volatile uint32_t intr;			/* 0x08 */
	volatile uint32_t apb2prstr;	/* 0x0C */
	volatile uint32_t apb1prstr;	/* 0x10 */
	volatile uint32_t ahbpcenr;		/* 0x14 */
	volatile uint32_t apb2cenr;		/* 0x18 */
	volatile uint32_t apb1cenr; 	/* 0x1C */
	volatile uint32_t bdctlr;		/* 0x20 */
	volatile uint32_t rstsckr;		/* 0x24 */
	volatile uint32_t ahbrstr; 		/* 0x28 */
	volatile uint32_t cfgr2; 		/* 0x2C */	
} RCC_WCH_DEVICE, *PRCC_WCH_DEVICE;
#define RCC ((volatile PRCC_WCH_DEVICE) 0x40021000)

/* RCC_APB1CENR, RCC_APB1PRSTR bit definitions */
#define TIM2EN (1<<0)
#define TIM3EN (1<<1)
#define TIM4EN (1<<2)
#define TIM5EN (1<<3)
#define TIM6EN (1<<4)
#define TIM7EN (1<<5)
#define UART6EN (1<<6)
#define UART7EN (1<<7)
#define UART8EN (1<<8)
#define WWDGEN (1<<11)
#define SPI2EN (1<<14)
#define SPI3EN (1<<15)
#define USART2EN (1<<17)
#define USART3EN (1<<18)
#define USART4EN (1<<19)
#define USART5EN (1<<20)
#define I2C1EN (1<<21)
#define I2C2EN (1<<22)
#define USBDEN (1<<23)
#define CAN1EN (1<<25)
#define CAN2EN (1<<26)
#define BKPEN (1<<27)
#define PWREN (1<<28)
#define DACEN (1<<29)

/* RCC_APB2CENR, RCC_APB2PRSTR bit definitions*/
#define AFIOEN (1<<0)
#define IOPAEN (1<<2)
#define IOPBEN (1<<3)
#define IOPCEN (1<<4)
#define IOPDEN (1<<5)
#define IOPEEN (1<<6)
#define ADC1EN (1<<9)
#define ADC2EN (1<<10)
#define TIM1EN (1<<11)
#define TIM1EN (1<<11)
#define SPI1EN (1<<12)
#define TIM8EN (1<<13)
#define USART1EN (1<<14)
#define TIM9EN (1<<19)
#define TIM10EN (1<<20)

/* Constants for PLL clock init (144MHz) in MD307 */
#define RCC_HSEON 	(1<<16)
#define RCC_HSERDY 	(1<<17)
#define	HSE_STARTUP_TIMEOUT	500
#define RCC_HPRE_DIV1 0
#define RCC_PPRE2_DIV1 0
#define RCC_PPRE1_DIV2 0x400 
#define RCC_PLLSRC (1<<16)
#define RCC_PLLXTPRE (1<<17)
#define RCC_PLLMULL 0x3C0000   
#define RCC_PLLSRC_HSE (1<<16)
#define RCC_PLLXTPRE_HSE 0   
#define RCC_PLLMULL12 (0x9<<18)
#define RCC_PLLMULL18_EXTEN 0
#define RCC_PLLON (1<<24)
#define RCC_PLLRDY (1<<25)
#define	RCC_SWS 0xC
#define	RCC_SW 0x3

/* ********* GPIO/AFIO definitions ********* */
#define	GPIO_CFGLR(x) 	((volatile unsigned int *)		(x))  
#define	GPIO_CFGHR(x) 	((volatile unsigned int *)		(x+0x4))  
#define	GPIO_INDR(x) 	((volatile unsigned short *)	(x+0x8))  
#define	GPIO_OUTDR(x) 	((volatile unsigned short *)	(x+0xC))  
#define	GPIO_BSHR(x) 	((volatile unsigned long *)		(x+0x10))  
#define	GPIO_BSR(x) 	((volatile unsigned short *)	(x+0x10))  
#define	GPIO_BCR(x) 	((volatile unsigned short *)	(x+0x14))  
#define	GPIO_LCKR(x) 	((volatile unsigned int *)		(x+0x18))  

typedef	struct	tagGpiodevice{
	volatile uint32_t	cfglr;		/* 0x00 */
	volatile uint32_t	cfghr;		/* 0x04 */
	volatile uint16_t	indr;		/* 0x08 */
	volatile uint16_t	Unused0;	/* 0x0A */
	volatile uint16_t	outdr;		/* 0x0C */
	volatile uint16_t	Unused1;	/* 0x0E */
	union{
		unsigned int  bshr;       	/* +0x10 */
		struct{
			unsigned short bsr;  	/* +0x10 */
			unsigned short Unused2;	/* +0x12 */
		};
	};
	union{
		unsigned int  bcr;			/* +0x10 */
		struct{
			unsigned short brr;  	/* +0x10 */
			unsigned short Unused3;  /* +0x12 */
		};
	};
	volatile uint32_t	lckr;	/* 0x18 */
} GPIO_WCH_DEVICE, *PGPIO_WCH_DEVICE;

#define	GPIO_A		0x40010800
#define	GPIO_B		0x40010C00
#define	GPIO_C		0x40011000
#define	GPIO_D		0x40011400
#define	GPIO_E		0x40011800

#define	GPIOA		((PGPIO_WCH_DEVICE)	(GPIO_A))
#define	GPIOB		((PGPIO_WCH_DEVICE)	(GPIO_B))
#define	GPIOC		((PGPIO_WCH_DEVICE)	(GPIO_C))
#define	GPIOD		((PGPIO_WCH_DEVICE)	(GPIO_D))
#define	GPIOE		((PGPIO_WCH_DEVICE)	(GPIO_E))

/*
 * GPIO in and out regs can only be accessed as 16 bits,
 * we provide macros for 8-bit read and write for convience
 */
#define GPIO_READ_HIGH(x) ( ((unsigned short)*GPIO_INDR(x))>>8)
#define GPIO_READ_LOW(x)  ( ((unsigned short)*GPIO_INDR(x))&0xFF)

#define GPIO_WRITE_LOW(x,c) (*GPIO_OUTDR(x) = (unsigned short) ((unsigned short)c | (*GPIO_OUTDR(x) & 0xFF00)) )

#define GPIO_WRITE_HIGH(x,c) \
	*GPIO_BCR(x)=  (unsigned short) (~(c<<8) & 0xFF00);\
	*GPIO_BSHR(x)= ( (unsigned short) ((c<<8) & 0xFF00) );	


#define WRITE_BSHR(port,ushort)   *GPIO_BSHR(port) = \
	 (((( (unsigned long)ushort << 16)  | (unsigned long) ushort)) ^ 0xFFFF0000)

#define AFIO_PCFR1 ((volatile unsigned int *)	(0x40010004))	/* Remap register 1 */  
#define AFIO_PCFR2 ((volatile unsigned int *)	(0x4001001C))	/* Remap register 2 */  
#define AFIO_EXTICR1 ((volatile unsigned int *) (0x40010008))	/* External interrupt configuration register1 */ 
#define AFIO_EXTICR2 ((volatile unsigned int *) (0x4001000C))	/* External interrupt configuration register2 */ 
#define AFIO_EXTICR3 ((volatile unsigned int *) (0x40010010))	/* External interrupt configuration register3 */ 
#define AFIO_EXTICR4 ((volatile unsigned int *) (0x40010014))	/* External interrupt configuration register4 */ 


/* ********* SysTick ********* */
#define STK_CTLR  ((volatile unsigned int *)		(0xE000F000))  /* System count control register */ 
#define STK_SR 	  ((volatile unsigned int *)		(0xE000F004))  /* System count status register */
#define STK_CNT   ((volatile unsigned long long *)	(0xE000F008))  /* System counter register */
#define STK_CNTL  ((volatile unsigned int *)		(0xE000F008))  /* System counter low register */
#define STK_CNTH  ((volatile unsigned int *)		(0xE000F00C))  /* System counter high register */
#define STK_CMP   ((volatile unsigned long long *)	(0xE000F010))  /* System count comparison value */
#define STK_CMPLR ((volatile unsigned int *)		(0xE000F010))  /* System count comparison value low register */
#define STK_CMPHR ((volatile unsigned int *)		(0xE000F014))  /* System count comparison value high register */

typedef	struct	tagSystickWchdevice{
	volatile unsigned int ctlr;
	volatile unsigned int sr;
	volatile unsigned int cntl;
	volatile unsigned int cnth;
	volatile unsigned int cmplr;
	volatile unsigned int cmphr;
} SYSTICK_WCH_DEVICE, *PSYSTICK_WCH_DEVICE;

/* SysTick bit definitions */
#define STK_B_EN	1<<0
#define STK_B_IE	1<<1
#define STK_B_CLK	1<<2
#define STK_B_RE	1<<3
#define STK_B_CM	1<<4
#define STK_B_INIT	1<<5
#define STK_B_SWIE	1<<31

/* ********* TIMER/COUNTERS ********* */

/* Timer 6 */
#define		TIM6_CTLR1 		((volatile unsigned short *)	0x40001000)  
#define		TIM6_CTLR2 		((volatile unsigned short *)	0x40001004) 
#define		TIM6_DMAINTENR 	((volatile unsigned short *)	0x4000100C)  
#define		TIM6_INTFR 		((volatile unsigned short *)	0x40001010)  
#define		TIM6_SWEVGR 	((volatile unsigned short *)	0x40001014) 
#define		TIM6_CNT 		((volatile unsigned short *)	0x40001024)  
#define		TIM6_PSC		((volatile unsigned short *)	0x40001028)  
#define		TIM6_ATRLR		((volatile unsigned short *)	0x4000102C)  

/* Timer 7 */
#define		TIM7_CTLR1 		((volatile unsigned short *)	0x40001400)  
#define		TIM7_CTLR2 		((volatile unsigned short *)	0x40001404) 
#define		TIM7_DMAINTENR 	((volatile unsigned short *)	0x4000140C)  
#define		TIM7_INTFR 		((volatile unsigned short *)	0x40001410)  
#define		TIM7_CNT 		((volatile unsigned short *)	0x40001424)  
#define		TIM7_PSC		((volatile unsigned short *)	0x40001428)  
#define		TIM7_ATRLR		((volatile unsigned short *)	0x4000142C)  

typedef struct tagTimDevice
{
  volatile uint16_t ctlr1;		/* TIM control register 1,              0x00 */
  uint16_t res0;
  volatile uint16_t ctlr2;		/* TIM control register 2,              0x04 */
  uint16_t res1;
  volatile uint16_t smcfgr;		/* TIM slave mode control register,     0x08 */
  uint16_t res2;
  volatile uint16_t dmaintenr;	/* TIM DMA/interrupt enable register,   0x0C */
  uint16_t res3;
  volatile uint16_t intfr;		/* TIM status register,                 0x10 */
  uint16_t res4;
  volatile uint16_t swevgr;		/* TIM event generation register,       0x14 */
  uint16_t res5;
  volatile uint16_t chctlr1;	/* TIM capture/compare mode register 1, 0x18 */
  uint16_t res6;
  volatile uint16_t chctlr2;	/* TIM capture/compare mode register 2, 0x1C */
  uint16_t res7;
  volatile uint16_t ccer;		/* TIM capture/compare enable register, 0x20 */
  uint16_t res8;
  volatile uint16_t cnt;		/* TIM counter register,                0x24 */
  uint16_t res9;
  volatile uint16_t psc;		/* TIM prescaler,                       0x28 */
  uint16_t res10;
  volatile uint16_t atrlr;		/* TIM auto-reload register,            0x2C */
  uint16_t res11;
  uint32_t res12;
  volatile uint16_t ch1cvr;		/* TIM capture/compare register 1,      0x34 */
  uint16_t res13;
  volatile uint16_t ch2cvr;		/* TIM capture/compare register 2,      0x38 */
  uint16_t res14;
  volatile uint16_t ch3cvr;		/* TIM capture/compare register 3,      0x3C */
  uint16_t res15;
  volatile uint16_t ch4cvr;		/* TIM capture/compare register 4,      0x40 */
  uint16_t res16;
  uint16_t res17;
  volatile uint16_t dmacfgr;	/* TIM DMA configuration register,      0x48 */
  uint16_t res18;
  volatile uint16_t dmaadr;		/* TIM DMA address for full transfer,   0x4C */
} TIM_WCH_DEVICE, *PTIM_WCH_DEVICE;

#define TIM2 ((PTIM_WCH_DEVICE) 0x40000000)
#define TIM3 ((PTIM_WCH_DEVICE) 0x40000400)
#define TIM4 ((PTIM_WCH_DEVICE) 0x40000800)
#define TIM5 ((PTIM_WCH_DEVICE) 0x40000C00)
#define TIM6 ((PTIM_WCH_DEVICE) 0x40001000)
#define TIM7 ((PTIM_WCH_DEVICE) 0x40001400)

/* Timers (6 and 7) bit definitions */

#define TIM_UIF		(1<<0)
#define TIM_UIE		(1<<0)
#define TIM_UG		(1<<0)
#define TIM_ARPE	(1<<7)
#define TIM_OPM		(1<<3)
#define TIM_URS		(1<<2)
#define TIM_UDIS	(1<<1)
#define	TIM_CEN		(1<<0)


/* ******** Interrupt handling ******** */

/* PFIC, parameter 'x' is the interrupt number */
#define	PFIC_ISR(x) 		((volatile unsigned int *)	(0xE000E000+4*(x/32)) )  
#define	PFIC_IPR(x) 		((volatile unsigned int *)	(0xE000E020+4*(x/32)) )  
#define	PFIC_IENR(x) 		((volatile unsigned int *)	(0xE000E100+4*(x/32)) )  
#define	PFIC_IRER(x) 		((volatile unsigned int *)	(0xE000E180+4*(x/32)) )  
#define	PFIC_IPSR(x) 		((volatile unsigned int *)	(0xE000E200+4*(x/32)) )  
#define	PFIC_IPRR(x) 		((volatile unsigned int *)	(0xE000E280+4*(x/32)) )  
#define PFIC_IRQ_BPOS(x) 	(1<<(x%32))

/* Trap/Interrupt numbers */
#define NMI_IRQ			2
#define HARD_FAULT_IRQ	3
#define ECALL_M_IRQ		5
#define ECALL_U_IRQ		8
#define BREAKPOINT_IRQ	9
#define SYSTICK_IRQ		12
#define SW_IRQ			14

#define WWDG_IRQ		16
#define PVD_IRQ			17
#define TAMPER_IRQ		18
#define RTC_IRQ			19
#define FLASH_IRQ		20
#define RCC_IRQ			21
#define EXTI0_IRQ		22
#define EXTI1_IRQ		23
#define EXTI2_IRQ		24
#define EXTI3_IRQ		25
#define EXTI4_IRQ		26
#define DMA1CH1_IRQ		27
#define DMA1CH2_IRQ		28
#define DMA1CH3_IRQ		29
#define DMA1CH4_IRQ		30
#define DMA1CH5_IRQ		31
#define DMA1CH6_IRQ		32
#define DMA1CH7_IRQ		33
#define ADC1_2_IRQ		34
#define USB_HP_IRQ		35
#define CAN1_TX_IRQ		35
#define USB_LP_IRQ		36
#define CAN1_RX0_IRQ	36
#define CAN1_IR1		37
#define CAN1_SCE_IRQ	38
#define EXTI9_5_IRQ		39
#define TIM1_BRK_IRQ	40
#define TIM1_UP_IRQ		41
#define TIM1_TRG_COM_IRQ 42
#define TIM1_CC_IRQ		43
#define TIM2_IRQ		44
#define TIMER2_IRQ		44
#define TIM3_IRQ		45
#define TIMER3_IRQ		45
#define TIM4_IRQ		46
#define TIMER4_IRQ		46
#define I2C1_EV_IRQ		47
#define I2C1_ER_IRQ		48
#define I2C2_EV_IRQ		49
#define I2C2_ER_IRQ		50
#define SPI1_IRQ		51
#define SPI2_IRQ		52
#define USART1_IRQ		53
#define USART2_IRQ		54
#define USART3_IRQ		55
#define EXTI15_10_IRQ	56
#define RTCA_IRQ		57
#define USBW_IRQ		58
#define TIM8_BRK_IRQ	59
#define TIM8_UP_IRQ		60
#define TIM8_TRG_COM_IRQ 61
#define TIM8_CC_IRQ		62
#define RNG_IRQ			63
#define SDIO_IRQ		65
#define TIM5_IRQ		66
#define TIMER5_IRQ		66
#define SPI3_IRQ		67
#define UART4_IRQ		68
#define UART5_IRQ		69
#define TIM6_IRQ		70
#define TIMER6_IRQ		70
#define TIM7_IRQ		71
#define TIMER7_IRQ		71
#define DMA2_CH1_IRQ	72
#define DMA2_CH2_IRQ	73
#define DMA2_CH3_IRQ	74
#define DMA2_CH4_IRQ	75
#define DMA2_CH5_IRQ	76
#define ETH_IRQ			77
#define ETH_WKUP_IRQ	78
#define CAN2_TX_IRQ		79
#define CAN2_RX0_IRQ	80
#define CAN2_RX1_IRQ	81
#define CAN2_SCE_IRQ	82
#define OTG_FS_IRQ		83
#define USBHSWU_IRQ		84
#define USBHS_IRQ		85
#define DVP_IRQ			86
#define UART6_IRQ		87
#define UART7_IRQ		88
#define UART8_IRQ		89
#define TIM9_BRK_IRQ	90
#define TIM9_UP_IRQ		91
#define TIM9_TRG_COM_IRQ 92
#define TIM9_CC_IRQ		93
#define TIM10_BRK_IRQ	94
#define TIM10_UP_IRQ	95
#define TIM10_TRG_COM_IRQ 96
#define TIM10_CC_IRQ	97
#define DMA2_CH6_IRQ	98
#define DMA2_CH7_IRQ	99
#define DMA2_CH8_IRQ	100
#define DMA2_CH9_IRQ	101
#define DMA2_CH10_IRQ	102
#define DMA2_CH11_IRQ	103

/*
   EXAMPLE, PFIC access
	*PFIC_IENR(Interrupt number) |= PFIC_IRQ_BPOS( Interrupt number );	 	enable interrupt
	*PFIC_IRER(Interrupt number) |= PFIC_IRQ_BPOS( Interrupt number );	 	enable clear (disable) interrupt
	etc.
*/

#define IRQVEC(x)		((void (**)(void) )  (0x2001C000+(x*4)) )
/*
    EXAMPLE, set exception (trap/interrupt) vector mode 3
    (Assume: mtvec = 0x2001C000 | 3;)
	 __attribute__((interrupt )) void xxx_interrupt( void );
	 *IRQVEC( Interrupt number )= xxx_interrupt; 				set interrupt routine
*/

/*
   EXAMPLE, set exception (trap/interrupt) vector mode 1
    (Assume: mtvec = vtable | 1;)
	 .org	vtable+(Interrupt Number*4)
	 j      xxx_interrupt
*/

/* EXTI definitions */
#define EXTI_INTENR	((volatile unsigned int *) (0x40010400))  /* Interrupt enable register */ 
#define EXTI_EVENR	((volatile unsigned int *) (0x40010404))  /* Event enable register */ 
#define EXTI_RTENR	((volatile unsigned int *) (0x40010408))  /* Rising edge trigger enable register */ 
#define EXTI_FTENR	((volatile unsigned int *) (0x4001040C))  /* Falling edge trigger enable register */ 
#define EXTI_SWIEVR	((volatile unsigned int *) (0x40010410))  /* Software interrupt event register */ 
#define EXTI_INTFR	((volatile unsigned int *) (0x40010414))  /* Interrupt flag register */ 

/*  */
__attribute__( ( naked ) ) unsigned int getMstatus( void );
__attribute__( ( naked ) ) void setMstatus( unsigned int ); 
__attribute__( ( naked ) ) unsigned int getMtvec ( void );
__attribute__( ( naked ) ) void setMtvec( unsigned int ); 
__attribute__( ( naked ) ) unsigned int getMepc ( void );
__attribute__( ( naked ) ) void setMepc( unsigned int ); 
__attribute__( ( naked ) ) unsigned int getMcause ( void ); 
__attribute__( ( naked ) ) void setMcause( unsigned int ); 
__attribute__( ( naked ) ) unsigned int getMtval( void );
__attribute__( ( naked ) ) void setMtval( unsigned int ); 
__attribute__( ( naked ) ) unsigned int getGintenr( void ); 
__attribute__( ( naked ) ) void setGintenr( unsigned int ); 

/*  ******** USART ******** */
typedef	struct	tagUsartWchdevice {
	volatile unsigned short statr;		/* 0x00 */
	unsigned short unused0;				/* 0x02 */
	volatile unsigned short datar;		/* 0x04 */
	unsigned short unused1;				/* 0x06 */
	volatile unsigned short brr;		/* 0x08 */
	unsigned short unused2;				/* 0x0A */
	volatile unsigned short ctlr1;		/* 0x0C */
	unsigned short unused3;				/* 0x0E */
	volatile unsigned short ctlr2;		/* 0x10 */
	unsigned short unused4;				/* 0x12 */
	volatile unsigned short ctlr3;		/* 0x14 */
	unsigned short unused5;				/* 0x16 */
	volatile unsigned short gpr;		/* 0x18 */
} USART_WCH_DEVICE, * PUSART_WCH_DEVICE;

#define		USART1 	(( PUSART_WCH_DEVICE) 0x40013800)
#define		USART2 	(( PUSART_WCH_DEVICE) 0x40004400)
#define		USART3 	(( PUSART_WCH_DEVICE) 0x40004800)
#define		UART4 	(( PUSART_WCH_DEVICE) 0x40004C00)
#define		UART5 	(( PUSART_WCH_DEVICE) 0x40005000)
#define		UART6 	(( PUSART_WCH_DEVICE) 0x40001800)
#define		UART7 	(( PUSART_WCH_DEVICE) 0x40001C00)
#define		UART8 	(( PUSART_WCH_DEVICE) 0x40002000)

/* USART bit definitions */
#define UST_CTS 	(1<<9)
#define UST_LBD 	(1<<8)
#define UST_TXE 	(1<<7)
#define UST_TC 		(1<<6)
#define UST_RXNE 	(1<<5)
#define UST_IDLE 	(1<<4)
#define UST_ORE 	(1<<3)
#define UST_NE	 	(1<<2)
#define UST_FE 		(1<<1)
#define UST_PE 		(1<<0)

/* USART Control register 1 */
#define UST_UE 		(1<<13)
#define UST_M 		(1<<12)
#define UST_WAKE 	(1<<11)
#define UST_PCE 	(1<<10)
#define UST_PS 		(1<<9)
#define UST_PEIE 	(1<<8)
#define UST_TXEIE 	(1<<7)
#define UST_TC 		(1<<6)
#define UST_RXNEIE 	(1<<5)
#define UST_IDLEIE 	(1<<4)
#define UST_TE 		(1<<3)
#define UST_RE	 	(1<<2)
#define UST_RWU 	(1<<1)
#define UST_SBK 	(1<<0)

/* USART Control register 2 */
#define UST_LINEN 	(1<<14)
#define UST_STOP 	(3<<12)
#define UST_CLKEN 	(1<<11)
#define UST_CPOL 	(1<<10)
#define UST_CPHA 	(1<<9)
#define UST_LBCL 	(1<<8)
#define UST_LBDIE 	(1<<6)
#define UST_LBDL 	(1<<5)
#define UST_ADD	 	(0xF)

/* USART Control register 3 */
#define UST_ONEBIT 	(1<<11)
#define UST_CTSIE 	(1<<10)
#define UST_CTSE 	(1<<9)
#define UST_RTSE 	(1<<8)
#define UST_DMAT 	(1<<7)
#define UST_DMAR 	(1<<6)
#define UST_SCEN 	(1<<5)
#define UST_NACK 	(1<<4)
#define UST_HDSEL	(1<<3)
#define UST_IRLP	(1<<2)
#define UST_IREN 	(1<<1)
#define UST_EIE 	(1<<0)


/*  ******** SPI ******** */

typedef struct tagSpiDevice
{
  volatile unsigned short cr1;    /* SPI control register 1 (not used in I2S mode),      0x00 */
  unsigned short      reserved0;  
  volatile unsigned short cr2;    /* SPI control register 2,                             0x04 */
  unsigned short      reserved1;  
  volatile unsigned short sr;     /* SPI status register,                                0x08 */
  unsigned short      reserved2;  
  volatile unsigned short dr;     /* SPI data register,                                  0x0C */
  unsigned short      reserved3;  
  volatile unsigned short crcpr;  /* SPI CRC polynomial register (not used in I2S mode), 0x10 */
  unsigned short      reserved4;  
  volatile unsigned short rxcrcr; /* SPI RX CRC register (not used in I2S mode),         0x14 */
  unsigned short      reserved5;  
  volatile unsigned short txcrcr; /* SPI TX CRC register (not used in I2S mode),         0x18 */
  unsigned short      reserved6;  
  volatile unsigned short i2scfgr; /* SPI_I2S configuration register,                    0x1C */
  unsigned short      reserved7;  
  volatile unsigned short i2spr;   /* SPI_I2S prescaler register,                        0x20 */
  unsigned short      reserved8;  
} SPI_WCH_DEVICE, *PSPI_WCH_DEVICE;

#define SPI1_BASE 0x40013000 
#define SPI2_BASE 0x40003800 
#define SPI3_BASE 0x40003C00 

#define SPI1 ((PSPI_WCH_DEVICE) SPI1_BASE)
#define SPI2 ((PSPI_WCH_DEVICE) SPI2_BASE)
#define SPI3 ((PSPI_WCH_DEVICE) SPI3_BASE)


#endif


