;//*****************************************************************************
;//
;//! \file startup_rvmdk.c
;//! \brief Cortex-M0/M3 Devices Startup code for MDK.
;//!        This module performs:
;//!           - Set the initial SP
;//!           - Set the vector table entries with the exceptions ISR address
;//!           - Initialize data and bss 			
;//!           - Call the application's entry point.
;//!           .
;//! \version V2.1.1.0
;//! \date 11/14/2011
;//! \author CooCox
;//! \copy
;//!
;//! Copyright (c)  2011, CooCox 
;//! All rights reserved.
;//! 
;//! Redistribution and use in source and binary forms, with or without 
;//! modification, are permitted provided that the following conditions 
;//! are met: 
;//! 
;//!     * Redistributions of source code must retain the above copyright 
;//! notice, this list of conditions and the following disclaimer. 
;//!     * Redistributions in binary form must reproduce the above copyright
;//! notice, this list of conditions and the following disclaimer in the
;//! documentation and/or other materials provided with the distribution. 
;//!     * Neither the name of the <ORGANIZATION> nor the names of its 
;//! contributors may be used to endorse or promote products derived 
;//! from this software without specific prior written permission. 
;//! 
;//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
;//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
;//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
;//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
;//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
;//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
;//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
;//! THE POSSIBILITY OF SUCH DAMAGE.
;//
;//*****************************************************************************

Stack_Size      EQU     0x0000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                ; Top of Stack
                DCD     ResetHandler                ; Reset Handler
                DCD     NMIIntHandler               ; NMI Handler
                DCD     HardFaultIntHandler         ; Hard Fault Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     SVCIntHandler               ; SVCall Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     PendSVIntHandler            ; PendSV Handler
                DCD     SysTickIntHandler           ; SysTick Handler

                ; External Interrupts
                DCD     WAKEUP_IRQHandler         ; 16+ 0: Wakeup PIO0.0
                DCD     WAKEUP_IRQHandler         ; 16+ 1: Wakeup PIO0.1
                DCD     WAKEUP_IRQHandler         ; 16+ 2: Wakeup PIO0.2
                DCD     WAKEUP_IRQHandler         ; 16+ 3: Wakeup PIO0.3
                DCD     WAKEUP_IRQHandler         ; 16+ 4: Wakeup PIO0.4
                DCD     WAKEUP_IRQHandler         ; 16+ 5: Wakeup PIO0.5
                DCD     WAKEUP_IRQHandler         ; 16+ 6: Wakeup PIO0.6
                DCD     WAKEUP_IRQHandler         ; 16+ 7: Wakeup PIO0.7
                DCD     WAKEUP_IRQHandler         ; 16+ 8: Wakeup PIO0.8
                DCD     WAKEUP_IRQHandler         ; 16+ 9: Wakeup PIO0.9
                DCD     WAKEUP_IRQHandler         ; 16+ 10: Wakeup PIO0.10
                DCD     WAKEUP_IRQHandler         ; 16+ 11: Wakeup PIO0.11
                DCD     TWS_IRQHandler            ; 16+12: TWS
                DCD     TIMER16_0_IRQHandler      ; 16+13: 16-bit Timer0
                DCD     TIMER16_1_IRQHandler      ; 16+14: 16-bit Timer1
                DCD     TIMER32_0_IRQHandler      ; 16+15: 32-bit Timer0
                DCD     TIMER32_1_IRQHandler      ; 16+16: 32-bit Timer1
                DCD     SPI_IRQHandler            ; 16+17: SSP
                DCD     UART0_IRQHandler          ; 16+18: UART0                
                DCD     UART1_IRQHandler          ; 16+19: UART1
                DCD     COMP_IRQHandler           ; 16+20: Comparator                
                DCD     ADC_IRQHandler            ; 16+21: A/D Converter
                DCD     WDT_IRQHandler            ; 16+22: Watchdog timer                
                DCD     BOD_IRQHandler            ; 16+23: Brown Out Detect
                DCD     PWM0_IRQHandler           ; 16+24: Wakeup PIO0.10
                DCD     EINT0_IRQHandler        ; 16+25: PIO INT0
                DCD     EINT1_IRQHandler        ; 16+26: PIO INT1
                DCD     EINT2_IRQHandler        ; 16+27: PIO INT2
                DCD     PWM1_IRQHandler           ; 16+25: Wakeup PIO0.11
                DCD     DMA_IRQHandler            ; 16+29: DMA
                DCD     RTC_IRQHandler            ; 16+30: RTC
                DCD     DAC_IRQHandler            ; 16+31: D/A Converter            
__Vectors_End
                
__Vectors_Size         EQU     __Vectors_End - __Vectors                
                
                
                AREA    |.text|, CODE, READONLY
                
                
                
; Reset Handler 
                
                ENTRY
                
ResetHandler   PROC
                EXPORT  ResetHandler             [WEAK]
                IMPORT  __main              

                LDR     R0, =__main
                BX      R0
                ENDP
                
                
; Dummy Exception Handlers (infinite loops which can be modified)                
                
NMIIntHandler     PROC
                EXPORT  NMIIntHandler               [WEAK]
                B       .
                ENDP
HardFaultIntHandler\
                PROC
                EXPORT  HardFaultIntHandler         [WEAK]
                B       .
                ENDP
SVCIntHandler     PROC
                EXPORT  SVCIntHandler               [WEAK]
                B       .
                ENDP
PendSVIntHandler  PROC
                EXPORT  PendSVIntHandler            [WEAK]
                B       .
                ENDP
SysTickIntHandler PROC
                EXPORT  SysTickIntHandler           [WEAK]
                B       .
                ENDP

DefaultIntHandler PROC
                
                EXPORT  WAKEUP_IRQHandler         [WEAK]
                EXPORT  TWS_IRQHandler            [WEAK]
                EXPORT  TIMER16_0_IRQHandler      [WEAK]
                EXPORT  TIMER16_1_IRQHandler      [WEAK]
                EXPORT  TIMER32_0_IRQHandler      [WEAK]
                EXPORT  TIMER32_1_IRQHandler      [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  COMP_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  PWM0_IRQHandler           [WEAK]
                EXPORT	EINT0_IRQHandler          [WEAK]
                EXPORT	EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  DAC_IRQHandler            [WEAK]


WAKEUP_IRQHandler
TWS_IRQHandler
TIMER16_0_IRQHandler
TIMER16_1_IRQHandler
TIMER32_0_IRQHandler
TIMER32_1_IRQHandler
SPI_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
COMP_IRQHandler
ADC_IRQHandler
WDT_IRQHandler
BOD_IRQHandler
PWM0_IRQHandler
EINT0_IRQHandler  
EINT1_IRQHandler 
EINT2_IRQHandler
PWM1_IRQHandler
DMA_IRQHandler
RTC_IRQHandler
DAC_IRQHandler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
