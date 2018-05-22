#ifndef _MM32_CONFIG_H_
#define _MM32_CONFIG_H_

#include "HAL_conf.h"
#include "HAL_device.h"
#include "MM32F031x8.h"
#include "HAL_pwr.h"
#include "i2c.h"
extern u32 SystemCoreClock;	

#define IAP_UPDATE_FLAG_ADDR	(0x08007800)// APP flash   0x08004000 -- 0x08020000  0x1C000  APP????

/*==========================================================================
power_key
============================================================================*/
#define POWER_KEY_VALUE				GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

/*==========================================================================
5V_DRV_EN
============================================================================*/
	#define DRV_CHECK_VALUE										GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
	#define DRV_Enable   										GPIO_SetBits(GPIOB,GPIO_Pin_8)
	#define DRV_Disable    				  						GPIO_ResetBits(GPIOB,GPIO_Pin_8)
	//#define DRV_Disable											GPIO_SetBits(GPIOB,GPIO_Pin_8)
/*==========================================================================
POWER_ON _OFF
============================================================================*/
	#define POWER_ON   												GPIO_SetBits(GPIOA,GPIO_Pin_15)
	#define POWER_OFF   				  							GPIO_ResetBits(GPIOA,GPIO_Pin_15)

	#define KAR_DORMANCY_Enable   						  GPIO_SetBits(GPIOA,GPIO_Pin_14)
	#define KAR_DORMANCY_Disable   				  		GPIO_ResetBits(GPIOA,GPIO_Pin_14)

/*===============================================================================================
MOTO GPIO
==============================================================================================*/
#define MOTO1_HIGH_PH 												GPIO_SetBits(GPIOA,GPIO_Pin_13)
#define MOTO1_LOW_PH  												GPIO_ResetBits(GPIOA,GPIO_Pin_13)
#define PWM_1_HIGH 													GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define PWM_1_LOW  													GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define MOTO2_HIGH_PH 												GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define MOTO2_LOW_PH  				 								GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define PWM_2_HIGH 												GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define PWM_2_LOW  				 								GPIO_ResetBits(GPIOA,GPIO_Pin_10)
/*===============================================================================================
MOTO  TIMER
==============================================================================================*/
#define MOTO_TIM_START			TIM_Cmd(TIM2,ENABLE)
#define MOTO_TIM_STOP			TIM_Cmd(TIM2,DISABLE)

/*===============================================================================================
USB_DET GPIO
==============================================================================================*/
#define USB_DET													GPIO_Pin_11
#define USB_DET_VALUE											GPIO_ReadInputDataBit(GPIOA,USB_DET)
/*===============================================================================================
LED GPIO
==============================================================================================*/
	#define GPIO_1_HIGH  										GPIO_SetBits(GPIOB,GPIO_Pin_5)
	#define GPIO_1_LOW		  		  							GPIO_ResetBits(GPIOB,GPIO_Pin_5)

	#define GPIO_2_HIGH  										GPIO_SetBits(GPIOD,GPIO_Pin_0)
	#define GPIO_2_LOW   		  								GPIO_ResetBits(GPIOD,GPIO_Pin_0)

	#define GPIO_3_HIGH  										GPIO_SetBits(GPIOD,GPIO_Pin_1)
	#define GPIO_3_LOW   		  								GPIO_ResetBits(GPIOD,GPIO_Pin_1)

	#define GPIO_4_HIGH  										GPIO_SetBits(GPIOB,GPIO_Pin_4)
	#define GPIO_4_LOW   		  								GPIO_ResetBits(GPIOB,GPIO_Pin_4)

#endif

