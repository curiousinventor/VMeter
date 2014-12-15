/*
 * \file
 *
 * \brief 
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
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
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
//#ifdef _DEBUG_INTERFACE_
/*============================ INCLUDES ======================================*/
//#include "avr_compiler.h"
#include "BitBangSPI_Master.h"
//#include "QDebugTransport.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

//#include <intrinsics.h>
#define JOIN( x, y ) x ## y
#define REG( REGISTER, SIDE ) JOIN( REGISTER, SIDE )
#define nop()  __asm__ __volatile__("nop")

/*============================ IMPLEMENTATION ================================*/

/*============================================================================
Name    :   BitBangSPI_Master_Init
------------------------------------------------------------------------------
Purpose :   Initialize BitBangSPI Interface
Input   :   n/a
Output  :   n/a
Notes   :	Called from QDebug_Init in QDebug.c
============================================================================*/
void BitBangSPI_Master_Init (void)
{
    REG( DDR, SPI_BB ) |=  ((1<<SS_BB) | ( 1 << MOSI_BB ) | ( 1 << SCK_BB ));
	
    REG( DDR, SPI_BB ) &=  ~( 1 << MISO_BB );  
    REG( PORT, SPI_BB ) &=  ~((1<<SS_BB) | ( 1 << MOSI_BB ) | ( 1 << SCK_BB ));
	//REG( PORT, SPI_BB ) |=  ((1<<SS_BB) | ( 1 << MOSI_BB ) | ( 1 << SCK_BB ));
    //REG( PORT, SPI_BB ) |=  ( 1 << MISO_BB ); 

}


/*============================================================================
Name    :   BitBangSPI_Send_Byte
------------------------------------------------------------------------------
Purpose :   Send and Read one byte using BitBangSPI Interface
Input   :   Data to send to slave
Output  :   Data read from slave
Notes   :	Called from BitBangSPI_Send_Message in this file
============================================================================*/
void BitBangSPI_Send_Byte(unsigned char c)
{
    //unsigned bit;
	// all the magic happens on the upstroke.  Normally low.
	// the SS is the latch that transfers the buffer to the output pins. It does this when
	// it goes high.
	// The data is transfered in on the upstroke of the clock.

/*
    for (bit = 0; bit < 8; bit++) {      
        if (c & 0x80)
            REG( PORT, SPI_BB ) |= ( 1 << MOSI_BB );
        else
            REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );	
        
        REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit
   		c <<= 1;   
        REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
    }
*/	
	// Loop unrolled:
	// MOSI_BB = 5
	
	if (c & 0x80)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again

	if (c & 0b01000000)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	if (c & 0b00100000)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	if (c & 0b00010000)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	// 5/8
	if (c & 0b00001000)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	if (c & 0b00000100)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	if (c & 0b00000010)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again
	
	if (c & 0b00000001)
        REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
    else
        REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );      
    REG( PORT, SPI_BB ) |= ( 1 << SCK_BB ); // clock high to sample the bit	
    REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB ); // send the clock low again	
 
     

}


// the following worked on the original LED driver chip... normally high, samples on the down stroke
/*
 for (bit = 0; bit < 8; bit++) {
        // write MOSI on trailing edge of previous clock /
        if (c & 0x80)
            REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );
        else
            REG( PORT, SPI_BB ) &=  ~( 1 << MOSI_BB );
        
        c <<= 1;
 
        // half a clock cycle before leading/rising edge /
        //DELAYUS(1);
		nop();
        
        REG( PORT, SPI_BB ) &= ~( 1 << SCK_BB );
 
        // half a clock cycle before trailing/falling edge /
        //DELAYUS(1);
		nop();
 
        // read MISO on trailing edge 
       // c |= ((REG( PIN, SPI_BB ) >> MISO_BB) & 0x01);
        REG( PORT, SPI_BB ) |= ( 1 << SCK_BB );
    }
	REG( PORT, SPI_BB ) |=  ( 1 << MOSI_BB );

*/
