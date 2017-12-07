#ifndef _USER_IT_H
#define _USER_IT_H
#include "config.h"



#define DRV_EVENTS                  			0x01
#define MOTO_DET1_EVENTS                  0x02
#define MOTO_DET2_EVENTS                  0x04
#define POWER_KEY_EVENTS                  0x08
#define RTC_INT_EVENTS                 	  0x10
#define USB_DET_EVENTS                 	  0x20
#define ADC_BAT_EVENTS                 	  0x40

typedef struct
{
	
	bool DRV;

	
	
}_all_event_flag;




extern uint16_t Information_events; 
extern _all_event_flag 												all_event_flag;
void clear_all_event(void);






#endif



