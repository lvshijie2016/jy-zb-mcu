#ifndef _ABNORMAL_MONINTORING_H
#define _ABNORMAL_MONINTORING_H
#include "config.h"


#define TIMEOUT_RUN_TIMER         9
#define TIMEOUT_ON_TIMER       	  40
#define TIMEOUT_OFF_TIMER         10
#define TIMEOUT_OD_TIMER     	  20



typedef struct
{
	
	bool 		run_exception_OFF_ON;
		
	uint8_t 	timeout_run_timer;
	
	
}_set_com_exception;


typedef enum
{
	EXCEPTION_0,
	EXCEPTION_1,
	EXCEPTION_2,
	EXCEPTION_3,
	EXCEPTION_4,
	EXCEPTION_5,
	MAX_EXCEPTION
}_EXCEPTION_HANDLING_FLAG;



void EXCEPTION(_EXCEPTION_HANDLING_FLAG exceotion_flag_t );

void exceotion_management(void);

#endif


