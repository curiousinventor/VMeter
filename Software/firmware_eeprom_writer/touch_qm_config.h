/*******************************************************************************
*   Atmel Corporation:  http://www.atmel.com
*   Support email:  touch@atmel.com
******************************************************************************/
/*  License
*   Copyright (c) 2010, Atmel Corporation All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions are met:
*
*   1. Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
*   2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   3. The name of ATMEL may not be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
*   SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
*   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _TOUCH_QM_CONFIG_H_
#define _TOUCH_QM_CONFIG_H_


#ifdef _QMATRIX_
/***************************************************************************
 *                                                                          *
 *                  Project Configuration Options                           *
 *                                                                          *
 ****************************************************************************
*/
/*
* Number of Channels(dependent on the library used).Please refer to the user guide
* more information on selecting the number of channels.
*
* Possible values: 4, 8, 12, 16. in case of QTouch
* Possible values: 8, 16, 32,64. in case of QTouch
*/
#ifndef QT_NUM_CHANNELS
#define QT_NUM_CHANNELS 56
#endif
/* Define the Number X lines to be used.
    Values allowed : 4 and 8
*/
#ifndef NUM_X_LINES
#define NUM_X_LINES 8u
#endif

/* Define the Number Y lines to be used.
    Values allowed : 2,4 and 8
*/

#ifndef NUM_Y_LINES
#define NUM_Y_LINES 7u
#endif

/*
* Enabling the _ROTOR_SLIDER_ constant will link the library need for using rotors
* and sliders.
*
* Possible values: comment/uncomment the define
*/

#ifndef _ROTOR_SLIDER_
/*#define _ROTOR_SLIDER_*/ /*Uncomment this define in case ROTOR/SLIDER is used*/ 
#endif

#ifndef QT_MAX_NUM_ROTORS_SLIDERS
#define QT_MAX_NUM_ROTORS_SLIDERS 0
#endif

/*
* Delay cycles that determine the capacitance charge pulse width. Value of 0
* will enable charge pulse width of 1 clock cycle, 1 will enable a width of 2
* clock cycles and so on...
*
* Possible values: 1,2,3,4,5,10,25
*/
#ifndef QT_DELAY_CYCLES
#define QT_DELAY_CYCLES 4
#endif

/*
* Which ports have X lines on them. These macros are used
* to conditionally compile in support for ports driving X lines.
*/

/* Specify the number of ports for X-lines that are needed.
 * Note: Support is provided only for a maximum of 3 ports for X.
 *		 ( Maximum possible value for NUM_X_PORTS is 3)
 * Also, note that code memory increases with the number of ports
 * used for X lines.
*/

#ifndef NUM_X_PORTS
#define NUM_X_PORTS 1
#endif

  	/* if X lines spread on single port*/
    #if (NUM_X_PORTS==1)
      #ifndef PORT_X_1
      #define PORT_X_1 B
      #define PORT_NUM_1 1/*please donot edit this value*/
    #endif
      /* if X lines spread on two ports*/
    #elif (NUM_X_PORTS==2)
      #ifndef PORT_X_1
        #define PORT_X_1 E
        #define PORT_NUM_1 1 /*please donot edit this value*/
      #endif
      #ifndef PORT_X_2
        #define PORT_X_2 E
        #define PORT_NUM_2 2/*please donot edit this value*/
      #endif
      /* if X lines spread on three ports*/
    #elif (NUM_X_PORTS==3)
      #ifndef PORT_X_1
        #define PORT_X_1 B
        #define PORT_NUM_1 1/*please donot edit this value*/
      #endif
      #ifndef PORT_X_2
        #define PORT_X_2 C
        #define PORT_NUM_2 2/*please donot edit this value*/
      #endif
      #ifndef PORT_X_3
        #define PORT_X_3 D
        #define PORT_NUM_3 3/*please donot edit this value*/
      #endif
      
   /* if X lines on more than three ports*/
   #else
      #error 'maximum ports supported are only 3'
   #endif

/* Specify the port for YA */

#ifndef PORT_YA
#define PORT_YA C
#endif
/* Specify the port for YB */

#ifndef PORT_YB
#define PORT_YB D
#endif
/* Specify the port for SMP */

#ifndef PORT_SMP
#define PORT_SMP D
#endif
/* Specify the pin for SMP on selected SMP port*/

#ifndef SMP_PIN
#define SMP_PIN 7
#endif

#ifndef _ATXMEGA_  
/*#define _ATXMEGA_*/  /*Uncomment this define for Xmega devices*/ 
#endif


/*--------------------------Project Configuration Options End-----------------------*/

/*============================================================================*
*                 Please donot edit below this line                           *
*=============================================================================*/

#ifdef _ATXMEGA_
    #define SMP_BIT (1 <<SMP_PIN)
#else
    #define SMP_BIT SMP_PIN
#endif

/*
* Set dwell time in cycles, and an empirical discharge delay to give c.50%
* duty cycle x pulses for the longer pulses. For the shorter ones, the spread
* spectrum calculations mean that we can't get the pulse low time down short
* enough for 50%.
*/

#if (QT_DELAY_CYCLES == 1u)

    #define DWELL_TIME_CYCLES 1
    #define DELAY_X_DISCHARGE 0

#elif (QT_DELAY_CYCLES == 2u)

    #define DWELL_TIME_CYCLES 2
    #define DELAY_X_DISCHARGE 0

#elif (QT_DELAY_CYCLES == 3u)

    #define DWELL_TIME_CYCLES 3
    #define DELAY_X_DISCHARGE 0

#elif (QT_DELAY_CYCLES == 4u)

    #define DWELL_TIME_CYCLES 4
    #define DELAY_X_DISCHARGE 0

#elif (QT_DELAY_CYCLES == 5u)

    #define DWELL_TIME_CYCLES 5
    #define DELAY_X_DISCHARGE 0


#elif (QT_DELAY_CYCLES == 10u)

    #define DWELL_TIME_CYCLES 10
    #define DELAY_X_DISCHARGE 10

#elif (QT_DELAY_CYCLES == 25u)

    #define DWELL_TIME_CYCLES 25
    #define DELAY_X_DISCHARGE 39

#elif (QT_DELAY_CYCLES == 50u)

    #define DWELL_TIME_CYCLES 50
    #define DELAY_X_DISCHARGE 103

#else
    #error No burst dwell time specified or dwell time specified is wrong
#endif


#if (DWELL_TIME_CYCLES < 1u )
    #error Dwell time must be at least 1 cycle.
#endif


/*
* Set actual delay time in burst loop based on dwell time. The -1 is because
* the following instruction in the burst routine takes one cycle.
*/
#define DELAY_DWELL_TIME        (DWELL_TIME_CYCLES -1)
#define DELAY_PRECHARGE_TIME    (DWELL_TIME_CYCLES -1)
#define CLAMP_TO_DISCHARGE_TIME (DWELL_TIME_CYCLES -1)

#if (defined(__IAR_SYSTEMS_ASM__) || defined(__ASSEMBLER__))

#if defined(__IAR_SYSTEMS_ASM__)

#include <ioavr.h>

#define p_1 r16
#define p_2 r17
#define p_3 r18
#define r_v r16
#define usr_1 r19
#define usr_2 r25
#define reg_clya r3
#define reg_clyb r0
#define reg_flya r1
#define reg_flyb r2

#define FILE_HEADER NAME qm_asm_iar
#define FILE_FOOTER END
#define FILE_SEGMENT RSEG CODE
#define GLOBAL_FUNCTION PUBLIC

#elif defined(__ASSEMBLER__)
#define __SFR_OFFSET 0
#include <avr/io.h>

#define p_1 r24
#define p_2 r22
#define p_3 r20
#define r_v r24
#define usr_1 r19
#define usr_2 r25
#define reg_clya r2
#define reg_clyb r3
#define reg_flya r4
#define reg_flyb r5

#define FILE_HEADER
#define FILE_FOOTER
#define FILE_SEGMENT
#define GLOBAL_FUNCTION .global

#endif /*IAR or GCC assembler*/
/* Macro to build register writes for controlling ports. The intermediate
*  JOIN macro is required for correct expansion of the args.   */
#define JOIN( x, y ) x ## y
#define JOIN1( A, B, C ) A ## B ## C
/* Macro to build register writes for controlling ports. The intermediate
*  JOIN macro is required for correct expansion of the args.   */
#define REG( REGISTER, SIDE ) JOIN( REGISTER, SIDE )
#define CONCAT( A,B, C ) JOIN1( A,B,C )
#define sreg_save r2
#define _00011001_	nop

#if (NUM_X_PORTS>=1)
    #define _01101001_	brne _111_
    #define _01101010_	brne _222_
    #define _01101011_	brne _333_
    #define _01101100_	brne _444_
    #define _01101101_	brne _555_
    #define _10001110_	rjmp .
    #define _10100001_	_111_: dec r19
    #define _10100010_	_222_: dec r19
    #define _10100011_	_333_: dec r19
    #define _10100100_	_444_: dec r19
    #define _10100101_	_555_: dec r19
    #define _11100001_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
    #define _11100010_	ldi r19, (DELAY_PRECHARGE_TIME/3)
    #define _11100011_	ldi r19, (DELAY_DWELL_TIME/3)
    #define _11100100_	ldi r19, (DELAY_X_DISCHARGE/3)
    #define _11100101_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
#endif/*NUM_X_PORTS==1*/
#if (NUM_X_PORTS>=2)
    #define _1001101001_	brne _666_
    #define _1001101010_	brne _777_
    #define _1001101011_	brne _888_
    #define _1001101100_	brne _999_
    #define _1001101101_	brne _AAA_
    #define _1010001110_	rjmp .
    #define _1010100001_	_666_: dec r19
    #define _1010100010_	_777_: dec r19
    #define _1010100011_	_888_: dec r19
    #define _1010100100_	_999_: dec r19
    #define _1010100101_	_AAA_: dec r19
    #define _1011100001_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
    #define _1011100010_	ldi r19, (DELAY_PRECHARGE_TIME/3)
    #define _1011100011_	ldi r19, (DELAY_DWELL_TIME/3)
    #define _1011100100_	ldi r19, (DELAY_X_DISCHARGE/3)
    #define _1011100101_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
#endif/*NUM_X_PORTS==2*/
#if (NUM_X_PORTS==3)
    #define _1101101001_	brne _BBB_
    #define _1101101010_	brne _CCC_
    #define _1101101011_	brne _DDD_
    #define _1101101100_	brne _EEE_
    #define _1101101101_	brne _FFF_
    #define _1110001110_	rjmp .
    #define _1110100001_	_BBB_: dec r19
    #define _1110100010_	_CCC_: dec r19
    #define _1110100011_	_DDD_: dec r19
    #define _1110100100_	_EEE_: dec r19
    #define _1110100101_	_FFF_: dec r19
    #define _1111100001_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
    #define _1111100010_	ldi r19, (DELAY_PRECHARGE_TIME/3)
    #define _1111100011_	ldi r19, (DELAY_DWELL_TIME/3)
    #define _1111100100_	ldi r19, (DELAY_X_DISCHARGE/3)
    #define _1111100101_	ldi r19, (CLAMP_TO_DISCHARGE_TIME/3)
#endif /*NUM_X_PORTS==3*/
#endif /*IAR and GCC*/
#endif /* Acquisition method: QMATRIX or QTOUCH*/
#endif /*_TOUCH_CONFIG_H_*/
