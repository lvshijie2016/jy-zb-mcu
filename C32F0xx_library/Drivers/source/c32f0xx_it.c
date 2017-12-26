#include "c32f0xx_it.h"

void NMI_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
//    while (1)
//    {
//    } 
	NVIC_SystemReset();
}
 
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
//    while (1)
//    {
//    }
	NVIC_SystemReset();
}
 
void SVC_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }  
}
 
void PendSV_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }  
}
 


