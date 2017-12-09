#include "user_timer.h"

uint16_t soft_timer[TIMER_MAX] = {0};



//�����ж�ʱ��
void set_soft_timer(SOFT_TIMER index, uint16_t nms)
{		
		soft_timer[index] = nms;
		
}

//��ȡ�ж�ʱ�䴥���ж�
uint16_t check_soft_timeout(SOFT_TIMER index )
{
	return !soft_timer[index];	
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








