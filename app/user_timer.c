#include "user_timer.h"

uint16_t soft_timer[TIMER_MAX] = {0};
uint16_t soft_timer_t[TIMER_MAX] = {0};


//设置中断时间
void set_soft_timer(SOFT_TIMER index, uint32_t nms)
{		
		soft_timer[index] = nms;
		soft_timer_t[index]  = nms;	 //防内在泄漏纠正
}

//获取中断时间触发中断
uint16_t check_soft_timeout(SOFT_TIMER index )
{
	if(soft_timer[index] > soft_timer_t[index])
			soft_timer[index] = soft_timer_t[index];
	return !soft_timer[index];	
}


uint16_t check_soft_timeout_PWM(SOFT_TIMER index )
{
	if(soft_timer[index] > soft_timer_t[index])
			soft_timer[index] = soft_timer_t[index];
	
	return soft_timer[index];
}


//延时ms
void timer_delay_ms(uint16_t delay)
{
	set_soft_timer(TIMER_DELAY, delay);
	while(!check_soft_timeout(TIMER_DELAY));
}


//中断计时 
#if defined C32F0
	void TIMER16_0_IRQHandler(void)
#elif defined MM32F031K6
	void TIM16_IRQHandler(void)
#endif
{

	unsigned int i = 0;
	for(i = 0; i< TIMER_MAX; i++)
	if(soft_timer[i] > 0 ) soft_timer[i]--;
	else  soft_timer[i] = 0;
	
#if defined C32F0	
	CT16B0_ClearIntFlag(TMR0);
#elif defined MM32F031K6
  
 TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
#endif
}








