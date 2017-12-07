;/*****************************************************************************
; * @file:    startup_G32F0xx.s
; * @description: CMSIS Cortex-M0 Core Device Startup File 
; *           for the  G32F0xx Device Series 
; * @version: V1.1
; * @date:    11 April. 2016
; *-----------------------------------------------------------------------------
;
;/*****************************************************************************/
;/* Startup.s: Startup file for ARM Cortex-M0 Device Family                   */
;/*****************************************************************************/



    MODULE  ?cstartup

    ;; Forward declaration of sections.
    SECTION CSTACK:DATA:NOROOT(3) ;; 8 bytes alignment

    SECTION .intvec:CODE:NOROOT(2);; 4 bytes alignment

    EXTERN  __iar_program_start
    ;EXTERN  HardFault_Handler
    PUBLIC  __vector_table

    DATA
__vector_table
    DCD     sfe(CSTACK)
    DCD     __iar_program_start

    DCD     NMI_Handler
    DCD     HardFault_Handler
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     0
    DCD     SVC_Handler
    DCD     0
    DCD     0
    DCD     PendSV_Handler
    DCD     SysTick_Handler

    ; External Interrupts
    DCD     WAKEUP_IRQHandler         					; 16+ 0: Wakeup 0	 Handler
    DCD     PMWFAULT_IRQHandler         				; 16+ 1: PWM Fault interrupt	Handler
    DCD     TWS_IRQHandler    									; 16+ 2: TWS	interrupt	Handler
    DCD     CT16B0_IRQHandler               		; 16+3, 16-bit Timer0 Interrupt Handler 
    DCD     CT16B1_IRQHandler               		; 16+4, 16-bit Timer1 Interrupt Handler
    DCD     CT16B2_IRQHandler               		; 16+5,  16-bit Timer2 Interrupt Handler
    DCD     CT16B3_IRQHandler               		; 16+6,  16-bit Timer3 Interrupt Handler
    DCD     UART0_IRQHandler                    ; 16+7,  UART0 Interrupt Handler   
    DCD     UART1_IRQHandler                    ; 16+8,  UART1 Interrupt Handler    
    DCD     ADC_IRQHandler                      ; 16+9,  A/D Converter Interrupt Handler
    DCD     WDT_IRQHandler                      ; 16+10,  Watchdog timer Interrupt Handler   
    DCD     BOD_IRQHandler                      ; 16+11, Brown Out Detect(BOD) Interrupt Handler
    DCD     GPIO0_IRQHandler                    ; 16+12, External Interrupt 0 Interrupt Handler
    DCD     GPIO1_IRQHandler                    ; 16+13, External Interrupt 1 Interrupt Handler
    DCD     GPIO2_IRQHandler                    ; 16+14, External Interrupt 2 Interrupt Handler
    DCD     RTC_IRQHandler                      ; 16+15, RTC Interrupt Handler           
    DCD     SPI_IRQHandler                      ; 16+16, SSP Interrupt Handler           
    DCD     PWMEVENT_IRQHandler                	; 16+17, PWM Timer Interrupt Handler 
    DCD     0        				                                        ; 16+18: Reserved                
    DCD     0          														; 16+19: : Reserved
    DCD     0           													; 16+20: Reserved              
    DCD     0           													; 16+21: Reserved
    DCD     0            													; 16+22: Reserved                
    DCD     0            													; 16+23: Reserved
    DCD     0						  												; 16+24: Reserved
    DCD     0          														; 16+25: Reserved
    DCD     0          														; 16+26: Reserved
    DCD     0          														; 16+27: Reserved
    DCD     0						  												; 16+28: Reserved
    DCD     0            													; 16+29: Reserved
    DCD     0            													; 16+30: Reserved
    DCD     0            													; 16+31: Reserved   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
    THUMB
    PUBWEAK Reset_Handler   
    SECTION .text:CODE:REORDER(2)       ; 4 bytes alignment
Reset_Handler
             
        LDR      R0, =__iar_program_start
        BX       R0
        
    PUBWEAK NMI_Handler
    PUBWEAK HardFault_Handler
    PUBWEAK SVC_Handler       
    PUBWEAK PendSV_Handler    
    PUBWEAK SysTick_Handler         
    PUBWEAK WAKEUP_IRQHandler
    PUBWEAK PMWFAULT_IRQHandler 
    PUBWEAK TWS_IRQHandler
    PUBWEAK CT16B0_IRQHandler
    PUBWEAK CT16B1_IRQHandler
    PUBWEAK CT16B2_IRQHandler
    PUBWEAK CT16B3_IRQHandler
    PUBWEAK UART0_IRQHandler
    PUBWEAK UART1_IRQHandler
    PUBWEAK ADC_IRQHandler
    PUBWEAK WDT_IRQHandler
    PUBWEAK BOD_IRQHandler
    PUBWEAK GPIO0_IRQHandler  
    PUBWEAK GPIO1_IRQHandler 
    PUBWEAK GPIO2_IRQHandler
    PUBWEAK RTC_IRQHandler
    PUBWEAK SPI_IRQHandler
    PUBWEAK PWMEVENT_IRQHandler
    PUBWEAK CAPTOUCH_IRQHandler


    SECTION .text:CODE:REORDER(2)

NMI_Handler       
SVC_Handler
HardFault_Handler
PendSV_Handler    
SysTick_Handler   
WAKEUP_IRQHandler
PMWFAULT_IRQHandler 
TWS_IRQHandler
CT16B0_IRQHandler
CT16B1_IRQHandler
CT16B2_IRQHandler
CT16B3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
ADC_IRQHandler
WDT_IRQHandler
BOD_IRQHandler
GPIO0_IRQHandler  
GPIO1_IRQHandler 
GPIO2_IRQHandler
RTC_IRQHandler
SPI_IRQHandler
PWMEVENT_IRQHandler
CAPTOUCH_IRQHandler  
Default_Handler 

    B Default_Handler         



    
    END

