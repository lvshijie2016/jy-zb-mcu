#include  "Abnormal_Monitoring.h"


_EXCEPTION_HANDLING_FLAG  exceotion_flag = MAX_EXCEPTION;
bool  process_flag = true;



void EXCEPTION(_EXCEPTION_HANDLING_FLAG exceotion_flag_t )
{
	if(exceotion_flag_t < MAX_EXCEPTION)exceotion_flag = exceotion_flag_t;
	
}



void exceotion_0_t(void)
{
	
	
	
	
}

void exceotion_1_t(void)
{
	
	
	
	
}

void exceotion_2_t(void)
{
	
	POWER_OFF;
	led_mode_get_t(0x06,0xff,20 ); 
	exceotion_flag = MAX_EXCEPTION;
	
}


void exceotion_3_t(void)
{
	if(process_flag)
	{
		POWER_OFF;
		process_flag = false;
		set_soft_timer(TIMER_EXCEPTION,500);
	}else if(check_soft_timeout(TIMER_EXCEPTION) )
	{
		process_flag = true;
		POWER_ON;
		exceotion_flag = MAX_EXCEPTION;
	}
	
}

void exceotion_4_t(void)
{
	
	
	
	
}

void exceotion_5_t(void)
{
	
	
	
	
}



void exceotion_management(void)
{
	switch(exceotion_flag) {
			
		case EXCEPTION_0:
			exceotion_0_t();
			
		break;
		
		case EXCEPTION_1:
			exceotion_1_t();
		break;
		case EXCEPTION_2:
			exceotion_2_t();
		break;
		case EXCEPTION_3:
			exceotion_3_t();
		break;
		case EXCEPTION_4:
			exceotion_4_t();
		break;
		case EXCEPTION_5:
			exceotion_5_t();
		break;
		
		default:break;
		}
	
}





