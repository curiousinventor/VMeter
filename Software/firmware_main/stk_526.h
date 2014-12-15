/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief Library for the board STK526
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

#ifndef STK_526_H
#define STK_526_H

//! @defgroup STK_526_module stk526
//! This module contains low level hardware abstraction layer for stk526 board
//! @image html stk_526.gif
//! @{

      //! @defgroup STK_526_usb USB Hardware connection
      //! Macros to define the USB Hardware connection
      //! @{
// Used if VBUS_SENSING_IO is defined as "ENABLED"
#define   VBUS_SENSE_PORT        PORTC   // PORTx
#define   VBUS_SENSE_PIN         PINC    // PINx
#define   VBUS_SENSE_DDR         DDRC    // DDRx
#define   VBUS_SENSE_IO          4       // 0...7
      //! @}

      //! @defgroup STK_526_leds Leds Management
      //! Macros to manage Leds on stk526
      //! @{
#define  LED_PORT  PORTD
#define  LED_DDR   DDRD
#define  LED_PIN   PIND

#define  LED0_BIT  PIND0
#define  LED1_BIT  PIND1
#define  LED2_BIT  PIND4
#define  LED3_BIT  PIND5

#define  Leds_init()             (LED_DDR  |=  (1<<LED0_BIT)|(1<<LED1_BIT)|(1<<LED2_BIT)|(1<<LED3_BIT), Leds_off())
#define  Leds_on()               (LED_PORT |=  (1<<LED0_BIT)|(1<<LED1_BIT)|(1<<LED2_BIT)|(1<<LED3_BIT))
#define  Leds_off()              (LED_PORT &= ~((1<<LED0_BIT)|(1<<LED1_BIT)|(1<<LED2_BIT)|(1<<LED3_BIT)))
#define  Leds_set_val(c)         (Leds_off(),LED_PORT |= (c<<4)&((1<<LED0_BIT) | (1<<LED1_BIT) | (1<<LED2_BIT) | (1<<LED3_BIT)))
#define  Leds_get_val()          (LED_PORT>>4)
                                
#define  Led0_on()               (LED_PORT |=  (1<<LED0_BIT))
#define  Led1_on()               (LED_PORT |=  (1<<LED1_BIT))
#define  Led2_on()               (LED_PORT |=  (1<<LED2_BIT))
#define  Led3_on()               (LED_PORT |=  (1<<LED3_BIT))
#define  Led0_off()              (LED_PORT &= ~(1<<LED0_BIT))
#define  Led1_off()              (LED_PORT &= ~(1<<LED1_BIT))
#define  Led2_off()              (LED_PORT &= ~(1<<LED2_BIT))
#define  Led3_off()              (LED_PORT &= ~(1<<LED3_BIT))
#define  Led0_toggle()           (LED_PIN  |=  (1<<LED0_BIT))
#define  Led1_toggle()           (LED_PIN  |=  (1<<LED1_BIT))
#define  Led2_toggle()           (LED_PIN  |=  (1<<LED2_BIT))
#define  Led3_toggle()           (LED_PIN  |=  (1<<LED3_BIT))
#define  Is_led0_on()            (LED_PIN  &   (1<<LED0_BIT) ? TRUE : FALSE)
#define  Is_led1_on()            (LED_PIN  &   (1<<LED1_BIT) ? TRUE : FALSE)
#define  Is_led2_on()            (LED_PIN  &   (1<<LED2_BIT) ? TRUE : FALSE)
#define  Is_led3_on()            (LED_PIN  &   (1<<LED3_BIT) ? TRUE : FALSE)
      //! @}

      //! @defgroup STK_5226_joy Joystick Management
      //! Macros to manage Joystick on stk526
      //! @{
#define  Joy_init()              (DDRB &= ~((1<<PINB7)|(1<<PINB6)|(1<<PINB5)|(1<<PINB4)|(1<<PINB0)), PORTB |= ((1<<PINB7)|(1<<PINB6)|(1<<PINB5)|(1<<PINB4)|(1<<PINB0)))
#define  Push_button_init()      Joy_init()
                                
#define  Is_btn_left()           Is_hwb()
#define  Is_btn_middle()         ((PINB & (1<<PINB0)) ?  FALSE : TRUE)
#define  Is_btn_right()          FALSE

#define  Is_joy_down()           ((PINB & (1<<PINB7)) ?  FALSE : TRUE)
#define  Is_joy_right()          ((PINB & (1<<PINB6)) ?  FALSE : TRUE)
#define  Is_joy_up()             ((PINB & (1<<PINB5)) ?  FALSE : TRUE)
#define  Is_joy_left()           ((PINB & (1<<PINB4)) ?  FALSE : TRUE)
                                
#define  Is_btn_not_left()       Is_not_hwb()
#define  Is_btn_not_middle()     ((PINB & (1<<PINB0)) ?  TRUE : FALSE)
#define  Is_btn_not_right()      TRUE

#define  Is_joy_not_down()       ((PINB & (1<<PINB7)) ?  TRUE : FALSE)
#define  Is_joy_not_right()      ((PINB & (1<<PINB6)) ?  TRUE : FALSE)
#define  Is_joy_not_up()         ((PINB & (1<<PINB5)) ?  TRUE : FALSE)
#define  Is_joy_not_left()       ((PINB & (1<<PINB4)) ?  TRUE : FALSE)


//! Enable interrupts for switches (PINB0 and PINB4 to PINB7)
//! - Set up interrupts for switches
//! - clear flag
//! - enable interrupt
#define  Switches_enable_it() {  \
      PCMSK0 |= (1<<PCINT0)|(1<<PCINT4)|(1<<PCINT5)|(1<<PCINT6)|(1<<PCINT7);\
      PCIFR |= (1<<PCIF0);       \
      PCICR |= (1<<PCIE0);}
 
//! Disable interrupts for switches (PINB0 and PINB4 to PINB7)
//! - disable interrupt
//! - clear flag
//! - Clear mask
#define  Switches_disable_it() { \
      PCICR  &= ~(1<<PCIE0);     \
      PCIFR  |= (1<<PCIF0);      \
      PCMSK0 &= ~((1<<PCINT0)|(1<<PCINT4)|(1<<PCINT5)|(1<<PCINT6)|(1<<PCINT7));}
      //! @}


      //! @defgroup STK526_HWB HWB button management
      //! HWB button is connected to PD7 and can also
      //! be used as generic push button
      //! @{
#define  Hwb_button_init()       (DDRD &= ~(1<<PIND7))
#define  Is_hwb()                ((PIND & (1<<PIND7)) ? FALSE : TRUE)
#define  Is_not_hwb()            ((PIND & (1<<PIND7)) ? TRUE : FALSE)
      //! @}


      //! @defgroup STK526 data flash configuration
      //! Macros to init the environnement for DF on STK526 board
      //! @{

// One AT45DB642D
#define DF_8_MB            
#define DF_NB_MEM          1

// PINB0 : SPI Slave Select pin, this pin must be disable (disable input mode) because it is used by joystick
//         Note: the joystick can work fine in Output mode, because the pull-up resistor is activated and the level pin can be read.
// PINB1 : the SCK signal direction is OUTPUT
// PINB2 : the SI signal direction is OUTPUT
// PINB3 : the SO signal direction is INPUT
// PINC2 : the CS signal direction is OUTPUT
#define  Df_init_spi()     (DDRB  &= ~((1<<PINB3)), \
                            DDRB  |=  ((1<<PINB2)|(1<<PINB1)|(1<<PINB0)), \
                            PORTB |=  ((1<<PINB3)|(1<<PINB2)|(1<<PINB1)|(1<<PINB0)),\
                            DDRC  |=  ((1<<PINC2)), \
                            PORTC |=  ((1<<PINC2)) )
#define  Df_select_0()     (PORTC &= ~(1<<PINC2))
#define  Df_desel_all()    (PORTC |=  (1<<PINC2))
      //! @}

//! @}

#endif  // STK_526


