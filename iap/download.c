#include "download.h"

#define DEBUG 0

uint8_t download_last_data_id = 0;
uint32_t FlashDestination = ApplicationAddress;
 

uint8_t com_rx_data(uint8_t* data)
{
	
	
}

void 	com_tx_data(uint8_t* data)
{
	
	
}




void iap_erash_flash(void)
{
	uint16_t i;
	
	for (i=0; i<FLASH_IMAGE_SIZE; i+=512)
	{
		IAP_FlashErase(ApplicationAddress+i);
	}

}


uint8_t iap_download(void)
{
	uint8_t 	ret = 1;
    uint32_t 	time;                                 	//
    uint16_t 	j;                                   	//
    uint8_t 	start_flag = 0; 
    uint32_t 	flash_offset = ApplicationAddress;      //
    uint32_t 	RamSource;                             	//
    uint8_t 	rx_buf[1032];                            //
	uint8_t  	tx_buf[10];
    uint8_t 	i;
    
	
    uint16_t 	all_packet_num = 1;       
    uint16_t 	current_packet_num = 0;        
    uint16_t 	bin_data_length;                    	//flash bin data to be programmed
    
	
	tx_buf[0] = PACKET_HEADER;
	
	while(time < 150000)
    {
        if(iap_receive_pkt(buf))
        {   
			// receive P0's starting pkt
            if (rx_buf[0]==PACKET_HEADER || rx_buf[0]==0xF1)
            {
				start_flag == 1;
				
				all_packet_num = rx_buf[4];
				
				//send P1's ack pkt
				tx_buf[2] = 2;
				tx_buf[3] = 0xF1;
				tx_buf[4] = tx_buf[2]^tx_buf[3];
				tx_buf[5] = PACKET_TAIL;
								
				com_tx_data(tx_buf,tx_buf[2]+2);
				
				return 0;
			}
			
            if (rx_buf[0]==0xFF || rx_buf[0]==0xF2)
            {
				start_flag = 2; 
				
                bin_data_length = rx_buf[2]<<8+rx_buf[4];
				
                if (bin_data_length > 1024)
                {
					
					//send P1's ack pkt
					tx_buf[2] = 4;
					tx_buf[3] = 0xF2;
					tx_buf[4] = current_packet_num;
					tx_buf[5] = 0x01;
					tx_buf[6] = tx_buf[2]^tx_buf[3]^tx_buf[4]^tx_buf[5];
					tx_buf[7] = PACKET_TAIL;
								
					
					com_tx_data(tx_buf,tx_buf[2]+2);
					return = SEND_ALL_AGAIN;
                }  
				
				//
                current_packet_num = (uint16_t)(rx_buf[5]);
			
                
                //program bin data to RamSorce
                RamSource = (uint32_t)rx_buf+6;
                for (j=0; (j<32) && (flash_offset<FLASH_SIZE); j+=4)
                {
                   
                    if(IAP_FlashProgram(flash_offset,*(uint32_t*)RamSource))
						;
						//send P1's ack pkt
					tx_buf[2] = 4;
					tx_buf[3] = 0xF2;
					tx_buf[4] = current_packet_num;
					tx_buf[5] = 0x00;
					tx_buf[6] = tx_buf[2]^tx_buf[3]^tx_buf[4]^tx_buf[5];
					tx_buf[7] = PACKET_TAIL;
					
					com_tx_data(tx_buf,tx_buf[2]+2);
					
                    flash_offset += 4;
                    RamSource += 4;
                }
                
				if(current_packet_num==all_packet_num)
					com_tx_data();
				else
					com_tx_data();
            }    
            time = 0;  
        }
        time++;//2~3S delay
    }
    return ret;
}

