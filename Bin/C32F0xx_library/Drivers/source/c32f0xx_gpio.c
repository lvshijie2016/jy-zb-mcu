/*****************************************************************************
 *   _GPIO:  Sourse file for MCU GPIO application
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2015.08.14  ver 1.00
 *
******************************************************************************/
#include "c32f0xx_gpio.h"

/*****************************************************************************
Function Name	GPIO_SetPinsAsInput
Function Definition	void GPIO_SetPinsAsInput(GPIO_TypeDef *port, uint16_t pins)
Function Description	Setup GPIO pins as digital input
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pins: combination of IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_ConfigPinsAsInput(GPIO_TypeDef *port, uint16_t pins)
{
	port->DIR.all &= ~pins;
	return;
}
/*****************************************************************************
Function Name	GPIO_SetPinsAsOutput
Function Definition	void GPIO_SetPinsAsOutput(GPIO_TypeDef *port, uint16_t pins)
Function Description	Setup GPIO pins as digital Output
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pins: combination of IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_ConfigPinsAsOutput(GPIO_TypeDef *port, uint16_t pins)
{
	port->DIR.all |= pins;
	return;
}

/*****************************************************************************
Function Name	GPIO_SetPin
Function Definition	void GPIO_SetPin (GPIO_TypeDef *port, uint16_t pin)
Function Description	Set GPIO pin Output as 1
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pin: IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_SetPin(GPIO_TypeDef *port, uint16_t pin)
{
	port->SET.all |= pin;
	return;
}

/*****************************************************************************
Function Name	GPIO_ResetPin
Function Definition	void GPIO_ResetPin (GPIO_TypeDef *port, uint16_t pin)
Function Description	Reset GPIO pin Output as 0
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pin: IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_ResetPin (GPIO_TypeDef *port, uint16_t pin)
	{
	port->CLR.all |= pin;
	return;
}
	
/*****************************************************************************
Function Name	GPIO_InvertOutPin
Function Definition	void GPIO_InvertOutPin (GPIO_TypeDef *port, uint16_t pin)
Function Description	Invert GPIO pin Output 
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pin: IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_InvertOutPin (GPIO_TypeDef *port, uint16_t pin)
{
	port->NOT.all |= pin;
	return;
}
	
/*****************************************************************************
Function Name	GPIO_GetPinState
Function Definition	uint8_t GPIO_GetPinState(GPIO_TypeDef *port, uint16_t pin)
Function Description	get GPIO pin state
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pin: IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	GPIO pin state, 0 is low and 1is high
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint8_t GPIO_GetPinState(GPIO_TypeDef *port, uint16_t pin)
{
	if (port->PIN.all & pin)
		return 1;
	else
		return 0;
}

/*****************************************************************************
Function Name	GPIO_GetPortState
Function Definition	uint16_t GPIO_GetPortState(GPIO_TypeDef *port)
Function Description	get GPIO port state
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Return Value	GPIO port state value
Condition	-
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t GPIO_GetPortState(GPIO_TypeDef *port)
{
	return (uint16_t)port->PIN.all;
}


/*****************************************************************************
Function Name	GPIO_SetPortMask
Function Definition	void GPIO_SetPortMask(GPIO_TypeDef *port, uint16_t pins)
Function Description	Set GPIO port IO mask
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pins: combination of IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_SetPortMask(GPIO_TypeDef *port, uint16_t pins)
{
	port->MASK.all |= pins;
	return;
}
/*****************************************************************************
Function Name	GPIO_ResetPortMask
Function Definition	void GPIO_ResetPortMask(GPIO_TypeDef *port, uint16_t pins)
Function Description	Remove GPIO port IO mask
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Pins: combination of IO_PIN0~IO_PIN15, predefine in sys.h
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_ResetPortMask(GPIO_TypeDef *port, uint16_t pins)
{
	port->MASK.all &= ~pins;
	return;
}

/*****************************************************************************
Function Name	GPIO_ReadPort
Function Definition	uint16_t GPIO_ReadPort(GPIO_TypeDef *port)
Function Description	Read GPIO port value
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Return Value	Return GPIO port value
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t GPIO_ReadPort(GPIO_TypeDef *port)
{
	return (uint16_t) port->PIN.all;
}

/*****************************************************************************
Function Name	GPIO_WritePort
Function Definition	void GPIO_WritePort(GPIO_TypeDef *port, uint16_t data)
Function Description	Write value to GPIO port
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
Data: data to be write to GPIO port
Return Value	No
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_WritePort(GPIO_TypeDef *port, uint16_t data)
{
	port->OUT.all = data;
	return;
}

/*****************************************************************************
Function Name	GPIO_EnableInt
Function Definition	void GPIO_EnableInt(GPIO_TypeDef *port, uint16_t pin, uint8_t triggeredge)
Function Description	Setup GPIO pin interrupt
Input Parameters	* port : pointer to GPIO device, GPIO0/GPIO1/GPIO2
pin: IO_PIN0~IO_PIN15, predefine in sys.h
triggeredge: RISE_EDGE/FALL_EDGE/BOTH_EDGE
Return Value	Return GPIO port value
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void GPIO_EnableInt(GPIO_TypeDef *port, uint16_t pin, uint8_t triggeredge)
{
	port->IS.all &= ~pin;
	port->IE.all |= pin;
	switch(triggeredge)
	{
		case RISE_EDGE:
			port->IBE.all &= ~pin;
			port->IEV.all |= pin;
			break;
		case FALL_EDGE:
			port->IBE.all &= ~pin;
			port->IEV.all &= ~pin;
			break;
		case BOTH_EDGE:
			port->IBE.all |= pin;
			break;
		default:
			break;
	}
	return;
}






