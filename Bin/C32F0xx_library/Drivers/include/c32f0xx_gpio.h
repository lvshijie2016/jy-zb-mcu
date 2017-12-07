/*****************************************************************************
 *   c32f0xx_gpio.h:  Header file for C32F0xx Family Microprocessors
 *
 *   Copyright(C) 2014, Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __GPIO_H 
#define __GPIO_H

#include "c32f0xx.h"
#include "c32f0xx_sys.h"


//--GPIO Registers BITS Field---------------------------------------------------------------------------------------
struct GPIO_BITS  // bit description: 0=not masked/LOW; 1=masked/HIGH
{								 
    uint32_t IO0:1;							// 0 PIO2_0 access control
    uint32_t IO1:1;							// 1 PIO2_1 access control
    uint32_t IO2:1;							// 2 PIO2_2 access control
    uint32_t IO3:1;							// 3 PIO2_3 access control
    uint32_t IO4:1;							// 4 PIO2_4 access control
    uint32_t IO5:1;							// 5 PIO2_5 access control
    uint32_t IO6:1;							// 6 PIO2_6 access control
    uint32_t IO7:1;							// 7 PIO2_7 access control
    uint32_t IO8:1;							// 8 PIO2_8 access control
    uint32_t IO9:1;							// 9 PIO2_9 access control
    uint32_t IO10:1;						// 10 PIO2_10 access control
    uint32_t IO11:1;						// 11 PIO2_11 access control
    uint32_t IO12:1;						// 12 PIO2_12 access control
    uint32_t IO13:1;						// 13 PIO2_13 access control
    uint32_t IO14:1;						// 14 PIO2_14 access control
    uint32_t IO15:1;						// 15 PIO2_15 access control
    uint32_t RSVD:16;						// 31:16 Reserved
};

union GPIO_MASK_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_PIN_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_OUT_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_SET_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_CLR_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_NOT_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

union GPIO_DIR_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Interrupt Sense Register: 0=edge, 1=level, Reset=0
union GPIO_IS_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Interrupt Both Edge Sense Register: 0=depend on IEV, 1=Both edge, Reset=0
union GPIO_IBE_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Interrupt Event Register(depend on IS): 
//0=falling edge/LOW level, 1=rising edge/HIGH level, Reset=0
union GPIO_IEV_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Interrupt Mask Register: 0=Masked, 1=Not masked
union GPIO_IM_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Raw Interrupt Mask Status Register: 0=No, 1=Yes(RO)
union GPIO_RIS_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Masked Interrupt Status Register: 0=No, 1=Yes(RO)
union GPIO_MIS_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

// GPIO Interrupt Clear Register: 0=No effect, 1=Clear Flag(WO)
union GPIO_IC_REG 
{
    uint32_t					all;
    struct GPIO_BITS			bit;
};

	 
/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @addtogroup GPIO C32F0xx General Purpose Input/Output 
  @{
*/
typedef struct
{    
    __IO	union GPIO_MASK_REG		MASK;		/*!< Offset:0x000 GPIO mask register:PIN,OUT,SET,CLR,NOT (R/W) */
    __I		union GPIO_PIN_REG		PIN;		/*!< Offset:0x004 GPIO value register (RO)                     */
    __IO	union GPIO_OUT_REG		OUT;		/*!< Offset:0x008 GPIO output value register (R/W)             */
    __O		union GPIO_SET_REG		SET;		/*!< Offset:0x00C GPIO output value set register (WO)          */
    __O		union GPIO_CLR_REG		CLR;		/*!< Offset:0x010 GPIO output value clear register (WO)        */
    __O		union GPIO_NOT_REG		NOT;		/*!< Offset:0x014 GPIO output value invert register (WO)       */
    __IO	uint32_t                RESERVE[2];
    __IO	union GPIO_DIR_REG		DIR;        /*!< Offset:0x020 Data direction register (R/W)                */
    __IO	union GPIO_IS_REG		IS;			/*!< Offset:0x024 Interrupt sense register (R/W)               */
    __IO	union GPIO_IBE_REG		IBE;		/*!< Offset:0x028 Interrupt both edges register (R/W)          */
    __IO	union GPIO_IEV_REG		IEV;		/*!< Offset:0x02C Interrupt event register (R/W)               */
    __IO	union GPIO_IM_REG		IE;			/*!< Offset:0x030 Interrupt mask register (R/W)                */
    __I		union GPIO_RIS_REG		RIS;		/*!< Offset:0x034 Raw Interrupt status register (RO)           */
    __I		union GPIO_MIS_REG		MIS;		/*!< Offset:0x038 Masked Interrupt status register (RO)        */
    __O		union GPIO_IC_REG		IC;			/*!< Offset:0x03C Interrupt clear register (WO)                */
} GPIO_TypeDef;



//Define GPIO
#define		GPIOA			                ((GPIO_TypeDef   *) GPIOA_BASE )
#define		GPIOB			                ((GPIO_TypeDef   *) GPIOB_BASE )
#define		GPIOC			                ((GPIO_TypeDef   *) GPIOC_BASE )

#define PIN_SET 					        0x1
#define PIN_RESET 				            0x0
#define PIN_EDGE_SENSE 		                0x0
#define PIN_LEVEL_SENSE 	                0x1
#define PIN_IEV_CTL 			            0x0
#define PIN_BOTH_EDGE_CTL                   0x1
#define PIN_FALLEDGE_OR_LOWLEVEL_TRIGGER 	0x0
#define PIN_RISEEDGE_OR_HIGHLEVEL_TRIGGER   0x1


#if 0
#define ENABLEGPIO0CLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<29))
#define ENABLEGPIO1CLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<30))
#define ENABLEGPIO2CLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<31))
#define DISABLEGPIO0CLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<29))
#define DISABLEGPIO1CLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<30))
#define DISABLEGPIO2CLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<31))
#define SETGPIO0MASK(mask)                  (GPIO0->MASK.all = mask)
#endif



#define ENABLEGPIOACLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<29))
#define ENABLEGPIOBCLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<30))
#define ENABLEGPIOCCLK                      (SYSCON->SYSAHBCLKCTRL.all |= (1<<31))
#define DISABLEGPIOACLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<29))
#define DISABLEGPIOBCLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<30))
#define DISABLEGPIOCCLK                     (SYSCON->SYSAHBCLKCTRL.all &= ~(1<<31))
#define SETGPIOAMASK(mask)                  (GPIOA->MASK.all = mask)

#define PIN0                                IO_PIN0 
#define PIN1                                IO_PIN1 
#define PIN2                                IO_PIN2 
#define PIN3                                IO_PIN3 
#define PIN4                                IO_PIN4 
#define PIN5                                IO_PIN5 
#define PIN6                                IO_PIN6 
#define PIN7                                IO_PIN7 
#define PIN8                                IO_PIN8 
#define PIN9                                IO_PIN9 
#define PIN10                               IO_PIN10 	
#define PIN11                               IO_PIN11 	
#define PIN12                               IO_PIN12 	
#define PIN13                               IO_PIN13 	
#define PIN14                               IO_PIN14 	
#define PIN15                               IO_PIN15


//GPIO function select
#define PA0_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA0_FUNC_ADC_IN0        2   //ADC input channel 0

#define PA1_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA1_FUNC_ADC_IN1        2   //ADC input channel 1

#define PA2_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA2_FUNC_TIM3_CAP0      1   //Capture input, channel 0 for 16-bit timer 3
#define PA2_FUNC_TIM3_MAT0      2   //Match output, channel 0 for 16-bit timer 3
#define PA2_FUNC_TXD0           3   //Transmitter output for UART0
#define PA2_FUNC_ADC_IN2        5   //ADC input channel 2

#define PA3_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA3_FUNC_TIM3_CAP1      1   //Capture input, channel 1 for 16-bit timer 3
#define PA3_FUNC_TIM3_MAT1      2   //Match output, channel 1 for 16-bit timer 3
#define PA3_FUNC_RXD0           3   //Receiver input for UART0
#define PA3_FUNC_ADC_IN3        5   //ADC input channel 3

#define PA4_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA4_FUNC_TIM3_CAP2      1   //Capture input, channel 2 for 16-bit timer 3
#define PA4_FUNC_TIM3_MAT2      2   //Match output, channel 2 for 16-bit timer 3
#define PA4_FUNC_SPI_SSEL       3   //SPI chip select
#define PA4_FUNC_ADC_IN4        5   //ADC input channel 4

#define PA5_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA5_FUNC_SPI_SCK        1   //SPI serial clock
#define PA5_FUNC_ADC_IN5        3   //ADC input channel 5

#define PA6_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA6_FUNC_PWM_FAULT      1   //PWM Fault in
#define PA6_FUNC_TIM2_CAP0      2   //Capture input, channel 0 for 16-bit timer 2
#define PA6_FUNC_TIM2_MAT0      3   //Match output, channel 0 for 16-bit timer 2
#define PA6_FUNC_SPI_MISO       4   //SPI master in slave out
#define PA6_FUNC_ADC_IN6        6   //ADC_IN6 ！ ADC input channel 6

#define PA7_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA7_FUNC_PWM_OUT1       1   //PWM output channel 1
#define PA7_FUNC_TIM2_CAP1      2   //Capture input, channel 1 for 16-bit timer 2
#define PA7_FUNC_TIM2_MAT1      3   //Match output, channel 1 for 16-bit timer 2
#define PA7_FUNC_SPI_MOSI       4   //SPI master out slave in
#define PA7_FUNC_ADC_IN7        6   //ADC_IN7 ！ ADC input channel 7

#define PA8_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA8_FUNC_PWM_OUT0       1   //PWM output channel 0
#define PA8_FUNC_TIM3_CAP3      2   //Capture input, channel 3 for 16-bit timer 3
#define PA8_FUNC_TIM3_MAT3      3   //Match output, channel 3 for 16-bit timer 3
#ifndef ENABLE_C32F030_LQFP48
#define PA8_FUNC_CKLOUT         5   //clock output pin
#endif 

#define PA9_FUNC_GPIO           0   //General purpose digital input/output pin
#define PA9_FUNC_PWM_OUT2       1   //PWM output channel 2
#define PA9_FUNC_TXD0           2   //UART0 TXD
#define PA9_FUNC_I2C_SCL        4   //I2C serial clock

#define PA10_FUNC_GPIO          0   //General purpose digital input/output pin
#define PA10_FUNC_PWM_OUT4      1   //PWM output channel 4
#define PA10_FUNC_RXD0          2   //UART0 RXD
#define PA10_FUNC_I2C_SDA       4   //serial data

#define PA11_FUNC_GPIO          0   //General purpose digital input/output pin
#define PA11_FUNC_TIM2_CAP2 	2   //Capture input, channel 2 for 16-bit timer 2
#define PA11_FUNC_TIM2_MAT2		3   //Match output, channel 2 for 16-bit timer 2

#define PA12_FUNC_GPIO     		0   //General purpose digital input/output pin

#define PA13_FUNC_SWDIO 		0   //SWD debug port data
#define PA13_FUNC_GPIO     		1   //General purpose digital input/output pin

#define PA14_FUNC_SWDCLK  		0   //SWD debug port clock
#define PA14_FUNC_GPIO     		1   //General purpose digital input/output pin
#define PA14_FUNC_TXD1     		2   //TXD1 ！ Transmitter output for UART1

#define PA15_FUNC_GPIO          0   //General purpose digital input/output pin
#define PA15_FUNC_SPI_SSEL      1   //SPI chip select
#define PA15_FUNC_RXD1          2   //Receiver input for UART1

#define PB0_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB0_FUNC_PWM_OUT3       1   //PWM output channel 3
#define PB0_FUNC_TIM2_CAP2      2   //Capture input, channel 2 for 16-bit timer 2
#define PB0_FUNC_TIM2_MAT2      3   //Match output, channel 2 for 16-bit timer 2

#define PB1_FUNC_GPIO           0   //General purpose digital input/output pin.
#define PB1_FUNC_PWM_OUT5       1   //PWM output channel 5
#define PB1_FUNC_TIM2_CAP3      2   //Capture input, channel 3 for 16-bit timer 2
#define PB1_FUNC_TIM2_MAT3      3   //Match output, channel 3 for 16-bit timer 2

#ifdef ENABLE_C32F030_LQFP48
#define PB2_FUNC_GPIO           0   //General purpose digital input/output pin.
#endif

#define PB3_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB3_FUNC_SPI_SCK        1   //SPI serial clock

#define PB4_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB4_FUNC_SPI_MISO       1   //SPI master in slave out
#define PB4_FUNC_TIM2_CAP0      2   //Capture input, channel 0 for 16-bit timer 2
#define PB4_FUNC_TIM2_MAT0      3   //Match output, channel 0 for 16-bit timer 2

#define PB5_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB5_FUNC_SPI_MOSI       1   //SPI master out slave in
#define PB5_FUNC_TIM2_CAP1      2   //Capture input, channel 1 for 16-bit timer 2
#define PB5_FUNC_TIM2_MAT1      3   //Match output, channel 1 for 16-bit timer 2

#define PB6_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB6_FUNC_I2C_SCL        1   //I2C_SCL ！ I2C serial clock
#define PB6_FUNC_TXD1           2   //Transmitter output for UART1
#define PB6_FUNC_TIM3_CAP0      4   //Capture input, channel 0 for 16-bit timer 3
#define PB6_FUNC_TIM3_MAT0      5   //Match output, channel 0 for 16-bit timer 3

#define PB7_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB7_FUNC_I2C_SDA        1   //I2C serial data
#define PB7_FUNC_RXD1           2   //RXD1 ！ Receiver input for UART1
#define PB7_FUNC_TIM3_CAP1      4   //Capture input, channel 1 for 16-bit timer 3
#define PB7_FUNC_TIM3_MAT1      5   //Match output, channel 1 for 16-bit timer 3

#ifdef ENABLE_C32F030_LQFP48

#define PB8_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB8_FUNC_I2C_SCL        1   //
#define PB8_FUNC_TIM3_CAP2      2   //Capture input, channel 3 for 16-bit timer 2
#define PB8_FUNC_TIM3_MAT2      3   //Match output, channel 3 for 16-bit timer 2

#define PB9_FUNC_GPIO           0   //General purpose digital input/output pin
#define PB9_FUNC_I2C_SDA        1   //

#define PB10_FUNC_GPIO          0   //General purpose digital input/output pin
#define PB10_FUNC_I2C_SCL       1   //

#define PB11_FUNC_GPIO          0   //General purpose digital input/output pin
#define PB11_FUNC_I2C_SDA    	2   //

#define PB12_FUNC_GPIO     		0   //General purpose digital input/output pin
#define PB12_FUNC_SPI_SSEL      1   //
#define PB12_FUNC_PWM_FAULT     2   //

#define PB13_FUNC_GPIO  		0   //General purpose digital input/output pin
#define PB13_FUNC_SPI_SCK 		1   //
#define PB13_FUNC_PWM_OUT1 		2   //

#define PB14_FUNC_GPIO     		0   //General purpose digital input/output pin
#define PB14_FUNC_SPI_MISO      1   //
#define PB14_FUNC_TIM2_CAP3     2   //
#define PB14_FUNC_TIM2_MAT3     3   //
#define PB14_FUNC_PWM_OUT3      4

#define PB15_FUNC_GPIO          0   //General purpose digital input/output pin
#define PB15_FUNC_SPI_MISO      1   //SPI chip select
#define PB15_FUNC_TIM3_CAP3     2   //Receiver input for UART1
#define PB15_FUNC_TIM3_MAT3     3
#define PB14_FUNC_PWM_OUT5      4
#endif

#ifndef ENABLE_C32F030_LQFP48
#define PC0_FUNC_GPIO           0   //General purpose digital input/output pin

#define PC1_FUNC_GPIO           0   //General purpose digital input/output pin

#define PC2_FUNC_NRST           0   //Device reset input (active low)
#define PC2_FUNC_GPIO           1   //General purpose digital input/output pin

#define PC3_FUNC_BOOT           0   //Boot memory selection
#define PC3_FUNC_GPIO           1   //General purpose digital input/output pin

#else 
#define PC7_FUNC_GPIO           0   //General purpose digital input/output pin

#define PC8_FUNC_GPIO           0   //General purpose digital input/output pin
#define PC8_FUNC_CKLOUT         1   //clock output pin

#define PC9_FUNC_NRST           0   //General purpose digital input/output pin
#define PC9_FUNC_GPIO           1   //General purpose digital input/output pin

#define PC10_FUNC_GPIO          0   //General purpose digital input/output pin
#define PC10_FUNC_I2C_SCL       1   //

#define PC11_FUNC_GPIO          0   //General purpose digital input/output pin
#define PC11_FUNC_I2C_SDA       1   //

#define PC12_FUNC_RESERVED     	0   //General purpose digital input/output pin
#define PC12_FUNC_GPIO     		1   //General purpose digital input/output pin

#define PC13_FUNC_GPIO  	    0   //SWD debug port data

#define PC14_FUNC_GPIO  		0   //SWD debug port clock

#define PC15_FUNC_GPIO          0   //General purpose digital input/output pin

#endif

#define PULL_DOWN_DISABLE        0
#define PULL_DOWN_ENABLE         1

#define PULL_UP_DISABLE          0
#define PULL_UP_ENABLE           1

#define SCHMITT_DISABLE          1
#define SCHMITT_ENABLE           0

#define INPUT_INVERT_DISABLE     0
#define INPUT_INVERT_ENABLE      1

#define FAST_SLEW_RATE_MODE      0
#define SLOW_SLWE_RATE_MODE      1

#define DIGIT_ENABLE             0 
#define ANALOG_ENABLE            1

#define HIGH_DRIVE               0
#define LOW_DRIVE                1

#define OPEN_DRAIN_DISABLE       0 
#define OPEN_DRAIN_ENABLE        1

#define INPUT_FILTER_DISABLE     0
#define INPUT_FILTER_ENABLE      1

#define INPUT_ENABLE             0
#define INPUT_DISABLE            1


void GPIO_ConfigPinsAsInput(GPIO_TypeDef*port, uint16_t pins);
void GPIO_ConfigPinsAsOutput(GPIO_TypeDef*port, uint16_t pins);
void GPIO_SetPin (GPIO_TypeDef*port, uint16_t pin);
void GPIO_ResetPin (GPIO_TypeDef*port, uint16_t pin);
void GPIO_InvertOutPin(GPIO_TypeDef*port, uint16_t pin);
uint8_t GPIO_GetPinState(GPIO_TypeDef*port, uint16_t pin);
uint16_t GPIO_GetPortState(GPIO_TypeDef*port);
void GPIO_SetPortMask(GPIO_TypeDef*port, uint16_t pins);
void GPIO_ResetPortMask(GPIO_TypeDef*port, uint16_t pins);
uint16_t GPIO_ReadPort(GPIO_TypeDef*port);
void GPIO_WritePort(GPIO_TypeDef*port, uint16_t data);
void GPIO_EnableInt(GPIO_TypeDef*port, uint16_t pin, uint8_t triggeredge);




#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
