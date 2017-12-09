#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H 	

#include "c32f0xx.h"
#include "c32f0xx_iap.h"
#include "stdio.h"

#define ApplicationAddress    (0x800)    //2KB
#define FLASH_SIZE            (0x8000)   //32 KBytes
#define FLASH_IMAGE_SIZE      (uint32_t) (FLASH_SIZE - ApplicationAddress)

#define DOWNLOAD       	(0x31)  
#define DOWNLOADING    	(0x32)  
#define ACK            	(0x06)  
#define SEND_AGAIN     	(0x07)  
#define SEND_ALL_AGAIN 	(0x08)  
#define EOT            	(0x04)  

#define BYTE_TIME_OVER     (5000)
#define DOWNLOAD_TIME_OVER (100) 


#define PACKET_HEADER           (0xFF)
#define PACKET_NUM              (1)
#define PACKET_TOTAL_NUM        (1)
#define PACKET_DATA_TOTAL_SIZE  (2)
#define PACKET_DATA_SIZE        (1)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_NUM + PACKET_TOTAL_NUM + PACKET_DATA_TOTAL_SIZE + PACKET_DATA_SIZE)
#define PACKET_DATA             (128)
#define PACKET_CHECK_SUM        (1)  
#define PACKET_TAIL             (0xFE)  
#define PACKET_SIZE             (PACKET_OVERHEAD + PACKET_DATA + PACKET_CHECK_SUM + PACKET_TAIL)

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

uint8_t com_rx_data();
void 	com_tx_data();

void 	iap_send_pkt(uint8_t* data);
void 	iap_receive_pkt(uint8_t* data);

void 	iap_erash_flash(void);
uint8_t iap_ownload(void);

	
#endif
