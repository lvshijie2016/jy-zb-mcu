#include "download.h"

#define DEBUG 0

uint8_t download_last_data_id = 0;
uint32_t FlashDestination = ApplicationAddress;
 

uint8_t com_rx_data();
void 	com_tx_data();

void iap_send_pkt(uint8_t* data)
{
	
	
}

void iap_receive_pkt(uint8_t* data)
{
	
	
	
}


void erash_flash(void)
{
	uint16_t i;
	
	for (i=0; i<FLASH_IMAGE_SIZE; i+=512)
	{
		IAP_FlashErase(ApplicationAddress+i);
	}

}


uint8_t Download(void)
{
	uint8_t 	ret = 1;
    uint32_t 	time;                                 	//
    uint16_t 	j;                                   	//
    uint8_t 	start_flag = 0; 
    uint32_t 	flash_offset = ApplicationAddress;      //
    uint32_t 	RamSource;                             	//
    uint16_t 	buf[1032];                              //
    uint8_t 	i;
    uint8_t 	check_sum = 0;
    uint16_t 	all_packet_num = 1;       
    uint16_t 	current_packet_num = 0;        
    uint16_t 	bin_data_length;                    	//flash bin data to be programmed
    
	while(time < 150000)
    {
        if(iap_receive_pkt(buf))
        {   
			// receive P0's starting pkt
            if (buf[0]==0xFF || buf[0]==0xF1)
            {
				start_flag == 1;
				
				all_packet_num = buf[4];
				
				//send P1's ack pkt
				iap_send_pkt();
				
				return 0;
			}
			
            if (buf[0]==0xFF || buf[0]==0xF2)
            {
				start_flag = 2; 
				
                bin_data_length = buf[2]<<8+buf[4];
				
                if (bin_data_length > 1024)
                {
					iap_send_pkt();
					return = SEND_ALL_AGAIN;
                }  
				
				//
                current_packet_num = (uint16_t)(buf[5]);
			
                
                //program bin data to RamSorce
                RamSource = (uint32_t)buf+6;
                for (j=0; (j<32) && (flash_offset<FLASH_SIZE); j+=4)
                {
                   
                    if(IAP_FlashProgram(flash_offset,*(uint32_t*)RamSource))
						;
					
                    flash_offset += 4;
                    RamSource += 4;
                }
                
				if(current_packet_num==all_packet_num)
					iap_send_pkt();
				else
					iap_send_pkt();
            }    
            time = 0;  
        }
        time++;//2~3S delay
    }
    return ret;
}

