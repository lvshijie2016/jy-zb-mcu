#include "moto.h" 


static unsigned char moto_flag = 0;
static bool 		 moto_R_current_state;
static bool 		 moto_L_current_state;



_MOTO_Typedef_t   MOTO_t;

/*停止*/
void moto_P(void)
{
	CT16B1_STOP;
	PWM_2_LOW;
	PWM_1_LOW;
	MOTO1_HIGH_PH;
	MOTO2_HIGH_PH;
	MOTO1_LOW_PH;
	MOTO2_LOW_PH;
	moto_flag = MOTO_Clear;
	set_soft_timer(TIMER_MOTO,MOTO_Clear);
	memset(&MOTO_t,MOTO_Clear,sizeof(MOTO_t));	
}

/*前进*/
void moto_D(void)
{
	MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	
	
	
	MOTO1_HIGH_PH;
	//MOTO1_LOW_PH;
	//MOTO_t.L_duty  = 100;
	//MOTO_t.R_duty  = 100;
	CT16B1_START;
}

/*右前转*/

static void moto_R_D(void)
{
	MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	//MOTO1_HIGH_PH;
	MOTO1_LOW_PH;
	MOTO_t.L_duty = 0; 
	CT16B1_START;
}

/*左前转*/
static void moto_L_D(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	MOTO1_LOW_PH;
	//MOTO1_HIGH_PH;
	MOTO_t.L_duty  = 0;
	CT16B1_START;

}


/*右后转*/
static void moto_R_H(void)
{
	MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	MOTO1_HIGH_PH;
	//MOTO1_LOW_PH;
	MOTO_t.L_duty = 0;
	CT16B1_START;

}

/*左后转*/
static void moto_L_H(void)
{
	MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	MOTO1_LOW_PH;
	//MOTO1_HIGH_PH;
	MOTO_t.R_duty = 0;
	CT16B1_START;

}



/*后退*/
static void moto_H(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	//MOTO1_HIGH_PH;
	MOTO1_LOW_PH;
	CT16B1_START;
	
}


/*右转圈*/

static void moto_R_T(void)
{
	MOTO1_LOW_PH;
	//MOTO1_HIGH_PH;
	//MOTO2_HIGH_PH;
	MOTO2_LOW_PH;
	CT16B1_START;

}


/*左转圈*/
static void moto_L_T(void)
{
	MOTO1_HIGH_PH;
	//MOTO1_LOW_PH;
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	CT16B1_START;
}


static void moto_set_model(void)
{
	
	switch(MOTO_t.model){
		case MOTO_P:
			moto_P();
		break;
		case MOTO_D:
			moto_D();
		break;
		case MOTO_H:
			moto_H();
		break;
		case MOTO_R_D:
			moto_R_D();
		break;
		case MOTO_L_D:
			moto_L_D();
		break;
		case MOTO_R_H:
			moto_R_H();
		break;
		case MOTO_L_H:
			moto_L_H();
		break;
		case MOTO_R_T:
			moto_R_T();
		break;
		case MOTO_L_T:
			moto_L_T();
		break;
		default:
		break;
	}
}


void GetMotoCom(unsigned char *com)
{
	if(com[MOTO_SP] ==	0xff)
	{
		MOTO_t.timer 	= com[MOTO_RUN_TIMER];
		MOTO_t.L_duty	= com[MOTO_RUN_L_DUTY];
		MOTO_t.R_duty	= com[MOTO_RUN_R_DUTY];
		MOTO_t.model 	= com[MOTO_ASP];         
		moto_flag = MOTO_RUN;
		
	}	else if(com[MOTO_ASP] == MOTO_START_TW)
	{
		MOTO_t.ADFlag = com[MOTO_SP];
		moto_flag = MOTO_TW;
	}
	else if( com[MOTO_ASP] == MOTO_Clear)  moto_P();
	
	else if(com[MOTO_SP] < 0xfe)
	{
		MOTO_t.DancingPag[com[MOTO_SP]][0] = com[MOTO_ASP];
		MOTO_t.DancingPag[com[MOTO_SP]][1] = com[MOTO_RUN_TIMER];
		MOTO_t.DancingPag[com[MOTO_SP]][2] = com[MOTO_RUN_L_DUTY];
		MOTO_t.DancingPag[com[MOTO_SP]][3] = com[MOTO_RUN_R_DUTY];
	}
}


void get_moto_current_state(uint16_t R_state, uint16_t L_state,uint8_t bat_state)
{
	double R_vref;
	double L_vref;
	
	L_vref = (MOTO_t.L_duty*(6.4+(MOTO_t.L_duty-50)*0.08) + bat_state);
	R_vref = (MOTO_t.R_duty*(23+(MOTO_t.R_duty-50)*0.08) + bat_state);

	
	if(R_state > R_vref )
	{
		moto_R_current_state = false;
		set_soft_timer(TIMER_MOTO_R,1000);
		
	}else if(check_soft_timeout(TIMER_MOTO_R)) moto_R_current_state = true;
	
	if(L_state > L_vref )
	{
		moto_L_current_state = false;
		set_soft_timer(TIMER_MOTO_L,1000);
		
	}else if(check_soft_timeout(TIMER_MOTO_L))moto_L_current_state = true;
	
	
	//moto_R_current_state = R_state > R_vref  ?  false : true ;
	//moto_L_current_state = L_state > L_vref  ?  false : true ;
	
}


void moto_run_task(void)
{
	if(moto_flag)
	{	
		if(check_soft_timeout(TIMER_MOTO))
		{
			if(moto_flag == MOTO_TW)
			{
				MOTO_t.timer 				= MOTO_t.DancingPag[MOTO_t.num][1];
				MOTO_t.L_duty  				= MOTO_t.DancingPag[MOTO_t.num][2];
				MOTO_t.R_duty				= MOTO_t.DancingPag[MOTO_t.num][3];	
				MOTO_t.model 				= MOTO_t.DancingPag[MOTO_t.num][0]; 
				MOTO_t.num 	= (MOTO_t.num+1 == MOTO_t.ADFlag) ? 0 : MOTO_t.num+1;  
			}
			else if(moto_flag == MOTO_TC) 
				MOTO_t.model = MOTO_P;
			else 
				moto_flag = MOTO_TC;
			
			moto_set_model();	
			set_soft_timer(TIMER_MOTO,(MOTO_t.timer*25));
		}
	}
}


void TIMER16_1_IRQHandler(void)
{
	float  i;
	static uint16_t pwm_flag=0;
	pwm_flag = (pwm_flag+1) == DUTY ? 0 : pwm_flag+1;
	i = pwm_flag;
	i = (i/DUTY)*100;
	if(i < MOTO_t.L_duty && moto_R_current_state) PWM_1_HIGH;	
	else PWM_1_LOW;
	if(i < MOTO_t.R_duty && moto_L_current_state) PWM_2_HIGH;	
	else PWM_2_LOW;

	CT16B0_ClearIntFlag(TMR1);
}




