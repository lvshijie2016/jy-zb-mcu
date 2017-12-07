/*
 *-----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited.
 *
 *            (C) COPYRIGHT 2010-2011 ARM Limited.
 *                ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited.
 *
 *      SVN Information
 *
 *      Checked In          : $Date: 2011/05/31 05:22:40 $
 *
 *      Revision            : $Revision: 1.1 $
 *
 *      Release Information : BP200-r0p0-00rel0
 *-----------------------------------------------------------------------------
 */
/**************************************************************************//**
 * @file     core_cm0.c
 * @brief    CMSDK CMSIS compatible Cortex-M0 Core Peripheral Access Layer Source File
 * @version  CMSDK $State: Exp $
 * @date     $Date: 2011/05/31 05:22:40 $
 *
 ******************************************************************************/

#include <stdint.h>

/* define compiler specific symbols */
#if defined ( __CC_ARM   )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm                                       /*!< asm keyword for IAR Compiler          */
  #define __INLINE        inline                                      /*!< inline keyword for IAR Compiler. Only available in High optimization mode! */

#elif defined   (  __GNUC__  )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */

#elif defined   (  __TASKING__  )
  #define __ASM            __asm                                      /*!< asm keyword for TASKING Compiler      */
  #define __INLINE         inline                                     /*!< inline keyword for TASKING Compiler   */

#endif


/* ##########################  Core Instruction Access  ######################### */

#if defined ( __CC_ARM   ) /*------------------ RealView Compiler ----------------*/

/** \brief  Reverse byte order (16 bit)

    This function reverses the byte order in two unsigned short values.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#if (__ARMCC_VERSION < 400677)
__ASM uint32_t __REV16(uint32_t value)
{
  rev16 r0, r0
  bx lr
}
#endif /* __ARMCC_VERSION  */ 


/** \brief  Reverse byte order in signed short value

    This function reverses the byte order in a signed short value with sign extension to integer.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#if (__ARMCC_VERSION < 400677)
__ASM int32_t __REVSH(int32_t value)
{
  revsh r0, r0
  bx lr
}
#endif /* __ARMCC_VERSION  */ 


/** \brief  Remove the exclusive lock

    This function removes the exclusive lock which is created by LDREX.

 */
#if (__ARMCC_VERSION < 400000)
__ASM void __CLREX(void)
{
  clrex
}
#endif /* __ARMCC_VERSION  */ 


#elif (defined (__ICCARM__)) /*---------------- ICC Compiler ---------------------*/
/* obsolete */
#elif (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
/* obsolete */
#elif (defined (__TASKING__)) /*--------------- TASKING Compiler -----------------*/
/* obsolete */
#endif


/* ###########################  Core Function Access  ########################### */

#if defined ( __CC_ARM   ) /*------------------ RealView Compiler ----------------*/

/** \brief  Get Control Register

    This function returns the content of the Control Register.

    \return               Control Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_CONTROL(void)
{
  mrs r0, control
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Set Control Register

    This function writes the given value to the Control Register.

    \param [in]    control  Control Register value to set
 */
#if       (__ARMCC_VERSION <  400000)
__ASM void __set_CONTROL(uint32_t control)
{
  msr control, r0
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get ISPR Register

    This function returns the content of the ISPR Register.

    \return               ISPR Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_IPSR(void)
{
  mrs r0, ipsr
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get APSR Register

    This function returns the content of the APSR Register.

    \return               APSR Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_APSR(void)
{
  mrs r0, apsr
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get xPSR Register

    This function returns the content of the xPSR Register.

    \return               xPSR Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_xPSR(void)
{
  mrs r0, xpsr
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get Process Stack Pointer

    This function returns the current value of the Process Stack Pointer (PSP).

    \return               PSP Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_PSP(void)
{
  mrs r0, psp
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Set Process Stack Pointer

    This function assigns the given value to the Process Stack Pointer (PSP).

    \param [in]    topOfProcStack  Process Stack Pointer value to set
 */
#if       (__ARMCC_VERSION <  400000)
__ASM void __set_PSP(uint32_t topOfProcStack)
{
  msr psp, r0
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get Main Stack Pointer

    This function returns the current value of the Main Stack Pointer (MSP).

    \return               MSP Register value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_MSP(void)
{
  mrs r0, msp
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Set Main Stack Pointer

    This function assigns the given value to the Main Stack Pointer (MSP).

    \param [in]    topOfMainStack  Main Stack Pointer value to set
 */
#if       (__ARMCC_VERSION <  400000)
__ASM void __set_MSP(uint32_t mainStackPointer)
{
  msr msp, r0
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Get Priority Mask

    This function returns the current state of the priority mask bit from the Priority Mask Register.

    \return               Priority Mask value
 */
#if       (__ARMCC_VERSION <  400000)
__ASM uint32_t __get_PRIMASK(void)
{
  mrs r0, primask
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 


/** \brief  Set Priority Mask

    This function assigns the given value to the Priority Mask Register.

    \param [in]    priMask  Priority Mask
 */
#if       (__ARMCC_VERSION <  400000)
__ASM void __set_PRIMASK(uint32_t priMask)
{
  msr primask, r0
  bx lr
}
#endif /*  __ARMCC_VERSION  */ 
 

#elif (defined (__ICCARM__)) /*---------------- ICC Compiler ---------------------*/
/* obsolete */
#elif (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
/* obsolete */
#elif (defined (__TASKING__)) /*--------------- TASKING Compiler -----------------*/
/* obsolete */
#endif

