#include "HAL_device.h"
#include "HAL_conf.h"
#include "i2c.h"

volatile unsigned char rx_flag = 0;
volatile unsigned char tx_flag = 0;
extern uint16_t I2C_DMA_DIR;
unsigned char tx_buffer0[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
unsigned char rx_buffer0[16] ;
#define FLASH_DEVICE_ADDR 0xa2
/********************************************************************************************************
**函数信息 ：I2CInitMasterMode(void) //unit is Khz                    
**功能描述 ：初始化I2C
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void I2CInitMasterMode(void) 
{	
    RCC->AHBENR|=0x7<<17;
    RCC->APB1ENR|=0x00200000;
    
    GPIOB->CRH|=0x00ff;
    GPIOB->ODR|=0x3;
    
    I2C1->IC_CON = 0x163;
    I2C1->IC_TAR = FLASH_DEVICE_ADDR>>1;
    I2C1->IC_SS_SCL_HCNT = 0x77;
    I2C1->IC_SS_SCL_LCNT = 0x55;
    I2C1->IC_RX_TL=0x0;//Rx FIFO  
    I2C1->IC_TX_TL=0x0;//Tx FIFO 
    I2C1->IC_INTR_MASK=0;//disable all
    I2C1->IC_ENABLE |= 0x01;
    
    GPIOB->AFRH&=~0xff;	
    GPIOB->AFRH|=0x11;
}

void uni_i2c_init(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //开启GPIOA,GPIOB时钟
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				
    GPIO_Init(GPIOB, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7; 	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;			
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
    I2C_InitStructure.I2C_OwnAddress = FLASH_DEVICE_ADDR;
    I2C_InitStructure.I2C_Speed = I2C_Speed_FAST;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_1);

}


/********************************************************************************************************
**函数信息 ：I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**功能描述 ：检查发送中断标志位
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
********************************************************************************************************/
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
{
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TX_EMPTY))
        {
            break;
        }
    }
}
/********************************************************************************************************
**函数信息 ：I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**功能描述 ：检查接收中断标志位
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：无
********************************************************************************************************/
void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{
    
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL))
        {
            break;
        }	
    }
}
/********************************************************************************************************
**函数信息 ：I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)              
**功能描述 ：I2C发送数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2；cmd；temp
**输出参数 ：无
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
{
    I2C_SendData(I2Cx, temp);
    I2CTXEmptyCheck(I2Cx);
    
}
/********************************************************************************************************
**函数信息 ：I2CRXByte(I2C_TypeDef *I2Cx)             
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2
**输出参数 ：temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    unsigned short temp;
    
    I2CRXFullCheck(I2Cx);
    
    temp = I2C_ReceiveData(I2Cx);
    return (unsigned char)temp;
}

/********************************************************************************************************
**函数信息 ：I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )             
**功能描述 ：I2C发送数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2；cmd；temp
**输出参数 ：无
********************************************************************************************************/
void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    unsigned short temp;
    unsigned short i;
    unsigned char *p;
    uint32_t j;
    p = tx_data;
    
    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, FLASH_DEVICE_ADDR, I2C_Direction_Transmitter);
    I2C_Cmd(I2Cx, ENABLE);
    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(I2Cx,CMD_WRITE,temp);  //tx memory addr
    
    for(i=0;i<tx_count;i++)
    {
        temp = *p;
        p++;
        if(i==(tx_count-1))
            I2Cx->IC_DATA_CMD =temp| 0x200; // //muaul set stop flag
        else
            I2CTXByte(I2Cx,CMD_WRITE,temp); //tx data
        
        
    }	
    
    for(j=0;j<0x5000;j++); 

	I2C_GenerateSTOP(I2Cx,ENABLE);   
}

/********************************************************************************************************
**函数信息 ：I2CMasterRead(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )           
**功能描述 ：I2C接收数据
**输入参数 ：I2C_TypeDef *I2Cx，选择I2C1,I2C2；device_id；mem_byte_addr；rx_count；rx_data
**输出参数 ：无
********************************************************************************************************/
void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    unsigned char temp=0;
    unsigned short i;
    
    I2C_Send7bitAddress(I2Cx, FLASH_DEVICE_ADDR, I2C_Direction_Transmitter);
    
    I2C_Cmd(I2Cx, ENABLE);
    
    
    temp = ((mem_byte_addr) & 0xff);
    
    I2CTXByte(I2Cx,CMD_WRITE,temp);  //tx memory addr
    
    
    for(i=0;i<rx_count;i++)
    {
        rx_data[i] = I2CRXByte(I2Cx);
        
//        printf("RX data%d is  : %x \r\n",i,rx_data[i]);
        
    }
	I2C_GenerateSTOP(I2Cx,ENABLE);   
    
}








/**
  *****************************************************************************
  * @Name   : RTC????????????
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD:????????
  *           dat:    ??????
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void IIC_WriteByte(uint8_t SlaveAddress,unsigned char REG_ADD, unsigned char dat)
{
	unsigned short temp;
	unsigned short i;
	uint32_t j;

	I2C_Cmd(I2C1, DISABLE);
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	I2C_Cmd(I2C1, ENABLE);
	temp = ((REG_ADD) & 0xff);
	I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr

	   
	I2C1->IC_DATA_CMD =dat| 0x200; // //muaul set stop flag

	for(j=0;j<0x5000;j++);

} 

/**
  *****************************************************************************
  * @Name   : RTC????????????
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD:????????
  *
  * @Output : none
  *
  * @Return : ??????????
  *****************************************************************************
**/
unsigned char IIC_ReadByte(uint8_t SlaveAddress,unsigned char REG_ADD)
{
	unsigned char temp=0;
	unsigned short i;
	uint8_t data;

	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);

	I2C_Cmd(I2C1, ENABLE);


	temp = REG_ADD & 0xff;

	I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr


	data = I2CRXByte(I2C1);

	I2C_GenerateSTOP(I2C1,ENABLE);   
	return data;
}



void IIC_WriteArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t *Buff, uint8_t Length)
{
		
    unsigned short temp;
    unsigned short i;
    uint32_t j;

    I2C_Cmd(I2C1, DISABLE);
    I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
    I2C_Cmd(I2C1, ENABLE);
    temp = ((SubAddr) & 0xff);
    I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr
 
//    I2C1->IC_DATA_CMD =dat| 0x200; // //muaul set stop flag

//	  for(j=0;j<0x5000;j++);

    for(i=0;i<Length;i++)
    {
        temp = *Buff;
        Buff++;
        if(i==(Length-1))
            I2C1->IC_DATA_CMD =temp| 0x200; // //muaul set stop flag
        else
            I2CTXByte(I2C1,CMD_WRITE,temp); //tx data
                
    }	
    
    for(j=0;j<0x5000;j++); 
	I2C_GenerateSTOP(I2C1,ENABLE);   
		
}

void IIC_ReadArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Lenth, uint8_t *Buffer)
{
	
	
    unsigned char temp=0;
    unsigned short i;
	  uint8_t data;
    
    I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
    
    I2C_Cmd(I2C1, ENABLE);
    
    
    temp = SubAddr & 0xff;
    
    I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr
    
	
    for(i = 0; i < Lenth; i++)
    {
    	*(Buffer+i) = I2CRXByte(I2C1);  // Get pointer value
    }

    i = 0xFF;
    while(i--);	
	I2C_GenerateSTOP(I2C1,ENABLE);   
	
	
	
	
}

void I2CMasterTest()
{  
    
    I2CInitMasterMode();
    
	tx_buffer0[0] = 0;
	I2CMasterWrite(I2C1,FLASH_DEVICE_ADDR, 0, 1, tx_buffer0 );
  I2CMasterRead(I2C1,FLASH_DEVICE_ADDR, 0, 16, rx_buffer0 );
}
