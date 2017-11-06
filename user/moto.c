#include "moto.h" 


static unsigned char moto_flag = 0;
_MOTO_Typedef_t   MOTO_t;

/*停止*/

void moto_P(void)
{
	MOTO1_LOW_PH;
	MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4 | PWM5, MOTO_Clear);
	moto_flag = MOTO_Clear;
	set_soft_timer(TIMER_MOTO,MOTO_Clear);
	memset(&MOTO_t,MOTO_Clear,sizeof(MOTO_t));	
}

/*前进*/
static void moto_D(void)
{
	//MOTO1_LOW_PH;
	MOTO1_HIGH_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );
	//MOTO2_HIGH_PH;
	MOTO2_LOW_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );
}

/*右前转*/

static void moto_R_D(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, 0 );
	MOTO1_HIGH_PH;
	//MOTO1_LOW_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );



}

/*左前转*/
static void moto_L_D(void)
{
		MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );
	//MOTO1_LOW_PH;
	MOTO1_HIGH_PH;
	PWM_SetDuty(PWM5, 0 );

}


/*右后转*/

static void moto_R_H(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, 0 );
	//MOTO1_HIGH_PH;
	MOTO1_LOW_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );


}

/*左后转*/
static void moto_L_H(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );

	//MOTO1_LOW_PH;
	MOTO1_HIGH_PH;
	PWM_SetDuty(PWM5, 0);

}



/*后退*/
static void moto_H(void)
{
	//MOTO2_LOW_PH;
	MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );

	//MOTO1_HIGH_PH;
	MOTO1_LOW_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );
	
}


/*右转圈*/

static void moto_R_T(void)
{
	//MOTO1_LOW_PH;
	MOTO1_HIGH_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );

	MOTO2_HIGH_PH;
	//MOTO2_LOW_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );

	
}


/*左转圈*/
static void moto_L_T(void)
{
	//MOTO1_HIGH_PH;
	MOTO1_LOW_PH;
	PWM_SetDuty(PWM5, MOTO_t.R_duty );
	MOTO2_LOW_PH;
	//MOTO2_HIGH_PH;
	PWM_SetDuty(PWM4, MOTO_t.L_duty );
		
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
		MOTO_t.timer = com[MOTO_RUN_TIMER];
		MOTO_t.L_duty	 = com[MOTO_RUN_L_DUTY];
		MOTO_t.R_duty	 = com[MOTO_RUN_R_DUTY];
		MOTO_t.model = com[MOTO_ASP];         
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







