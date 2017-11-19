#include "user_timer.h"

uint16_t soft_timer[TIMER_MAX] = {0};
uint16_t soft_timer_t[TIMER_MAX] = {0};


//�����ж�ʱ��
void set_soft_timer(SOFT_TIMER index, uint16_t nms)
{		
		soft_timer[index] = nms;
		soft_timer_t[index]  = nms;	 //������й©����
}

//��ȡ�ж�ʱ�䴥���ж�
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


//��ʱms
void timer_delay_ms(uint16_t delay)
{
	set_soft_timer(TIMER_DELAY, delay);
	while(!check_soft_timeout(TIMER_DELAY));
}


//�жϼ�ʱ 
void TIMER16_0_IRQHandler(void)
{
	unsigned int i = 0;
	for(i = 0; i< TIMER_MAX; i++)
	if(soft_timer[i] > 0 ) soft_timer[i]--;
	else  soft_timer[i] = 0;
	CT16B0_ClearIntFlag(TMR0);
}








