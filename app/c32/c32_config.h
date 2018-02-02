#ifndef _C32_CONFIG_H_
#define _C32_CONFIG_H_

/*==========================================================
  gpio 
  ===========================================================*/
#if true

#include "c32f0xx_gpio.h"

#endif
/*==========================================================
  IIC 
  ===========================================================*/
#if true

#include "c32f0xx_iic.h"

#endif
/*==========================================================
  PWM
  ===========================================================*/
#if true

#include "c32f0xx_pwm.h"

#endif
/*==========================================================
  UART
  ===========================================================*/
#if true

#include "c32f0xx_uart.h"

#endif
/*==========================================================
  IAP
  ===========================================================*/

#if true

#include "c32f0xx_iap.h"

#endif
/*==========================================================
  timer
  ===========================================================*/
#if true

#include "c32f0xx_timer.h"

#endif
/*==========================================================
  adc
  ===========================================================*/
#if true

#include "c32f0xx_adc.h"

#endif
/*==========================================================
  sys
  ===========================================================*/
#if true

#include "c32f0xx_sys.h"

#endif

#if true

#include "c32f0xx_wdt.h"

#endif


/*==========================================================================
power_key
============================================================================*/
#define POWER_KEY_VALUE			GPIO_GetPinState(GPIOA,PIN0)

/*==========================================================================
5V_DRV_EN
============================================================================*/
	#define DRV_Enable   												GPIO_SetPin(GPIOC,PIN3)
	#define DRV_Disable    				  								GPIO_ResetPin(GPIOC,PIN3)
/*==========================================================================
POWER_ON _OFF
============================================================================*/
	#define POWER_ON   													GPIO_SetPin(GPIOA,PIN15)
	#define POWER_OFF   				  								GPIO_ResetPin(GPIOA,PIN15)

	#define KAR_DORMANCY_Enable   										GPIO_SetPin(GPIOA,PIN14)
	#define KAR_DORMANCY_Disable   				  						GPIO_ResetPin(GPIOA,PIN14)

/*===============================================================================================
MOTO GPIO
==============================================================================================*/

#define MOTO1_HIGH_PH 												GPIO_SetPin(GPIOA,PIN13)
#define MOTO1_LOW_PH  												GPIO_ResetPin(GPIOA,PIN13)


#ifndef USING_V50

	#define PWM_1_HIGH 													GPIO_SetPin(GPIOB,PIN3)
	#define PWM_1_LOW  													GPIO_ResetPin(GPIOB,PIN3)

#else

	#define PWM_1_HIGH 													GPIO_SetPin(GPIOB,PIN1)
	#define PWM_1_LOW  													GPIO_ResetPin(GPIOB,PIN1)

#endif 


#define PWM_2_HIGH 													GPIO_SetPin(GPIOA,PIN10)
#define PWM_2_LOW  													GPIO_ResetPin(GPIOA,PIN10)

#ifndef USING_V50

	#define MOTO2_HIGH_PH 												GPIO_SetPin(GPIOB,PIN1)
	#define MOTO2_LOW_PH  				 								GPIO_ResetPin(GPIOB,PIN1)

#else

	#define MOTO2_HIGH_PH 												GPIO_SetPin(GPIOB,PIN3)
	#define MOTO2_LOW_PH  				 								GPIO_ResetPin(GPIOB,PIN3)

#endif
/*===============================================================================================
MOTO  TIMER
==============================================================================================*/
#define MOTO_TIM_START			CT16B1_START
#define MOTO_TIM_STOP			CT16B1_STOP


/*===============================================================================================
USB_DET GPIO
==============================================================================================*/
#define USB_DET													GPIO_Pin_11
#define USB_DET_VALUE											GPIO_GetPinState(GPIOA,USB_DET)
/*===============================================================================================
LED GPIO
==============================================================================================*/
	#define GPIO_1_HIGH  													GPIO_SetPin(GPIOB,PIN5)
	#define GPIO_1_LOW   				  									GPIO_ResetPin(GPIOB,PIN5)

	#define GPIO_2_HIGH  													GPIO_SetPin(GPIOC,PIN0)
	#define GPIO_2_LOW   				  									GPIO_ResetPin(GPIOC,PIN0)

	#define GPIO_3_HIGH  													GPIO_SetPin(GPIOC,PIN1)
	#define GPIO_3_LOW   				  									GPIO_ResetPin(GPIOC,PIN1)

	#define GPIO_4_HIGH  													GPIO_SetPin(GPIOB,PIN4)
	#define GPIO_4_LOW   				  									GPIO_ResetPin(GPIOB,PIN4)

#endif

