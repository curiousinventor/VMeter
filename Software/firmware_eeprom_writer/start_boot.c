/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the routines to jump in ISP mode
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  AT90USB162, AT90USB82
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//#include "config.h"
#include "start_boot.h"
//#include "wdt_drv.h"
#include <avr/wdt.h>
#include "MIDI.h"

#ifdef __ICCAVR__
   #if (defined(__AT90USB162__) ) 
      void (*start_bootloader) (void)=(void (*)(void))0x1800;
   #elif (defined(__AT90USB82__) )
      void (*start_bootloader) (void)=(void (*)(void))0x800;
   #else
      #error MCU part not define in project options
   #endif
#elif defined __GNUC__
   #if (defined(__AVR_AT90USB162__) ) 
      void (*start_bootloader) (void)=(void (*)(void))0x1C00; // was 0x1800
   #elif (defined(__AVR_AT90USB82__) )
      void (*start_bootloader) (void)=(void (*)(void))0x800;
   #else
      #error MCU part not define in project options
   #endif
#else // Other compiler
   #error Compiler unknow
#endif



#ifdef __GNUC__
   unsigned long boot_key __attribute__ ((section (".noinit")));
#else
   __no_init U32 boot_key At(0x0100); 
#endif
   
//! @brief This function will start the on-chip bootloader after a watchdog timer reset.
//!
//! The function set a special key in sram (uninitialized during mcu start-up execution) and performs a mcu
//! watchdog timer reset.
//! This function should be called when a special eve,t is detected in the application that requires the bootloader activation.
//! When this function is used, be sure to implement the "start_boot_if_required()" function at the begining of main().   
//!
void start_boot(void)
{
  
	USB_Disable();

  // Disable all interrupts
  cli();

  // Wait two seconds for the USB detachment to register on the host
  for (uint8_t i = 0; i < 128; i++)
    _delay_ms(16);

   boot_key=GOTOBOOTKEY;
   
   // Enable the WDT for reset mode
   //wdtdrv_enable(WDTO_500MS);
	wdt_enable(WDTO_250MS);
   while(1);
}

   
//! @brief This function calls the on-chip bootloader.
//!
//! The function starts the on-chip bootloader if the application has posted such a request.
//! This function sould be inserted at the begining of the main function.   
//!
void start_boot_if_required(void)
{


  if(boot_key==GOTOBOOTKEY)
  {
      boot_key = 0;
      (*start_bootloader)();           //! Jumping to bootloader
  }
  
}


//! @brief This function jump to on-chip bootloader without CPU reset.
//!
void goto_boot(void)
{
   (*start_bootloader)();              //! Jumping to bootloader
}


