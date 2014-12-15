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


#include "touch_qm_config.h"

FILE_HEADER

FILE_SEGMENT

#if (NUM_X_PORTS>=1)
GLOBAL_FUNCTION _00110000001_
_00110000001_:
    push usr_1
    push usr_2
    in   usr_1,REG(PORT,PORT_YB)
    mov  usr_2,p_1
    com  usr_2
    and  usr_1,usr_2
    out  REG(PORT,PORT_YB),usr_1
    in   usr_1,REG(PORT,PORT_YA)
    and  usr_1,usr_2
    out  REG(PORT,PORT_YA),usr_1
    mov  usr_2,p_1
    in   usr_1,REG(DDR,PORT_YB)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YB),usr_1
    in   usr_1,REG(DDR,PORT_YA)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YA),usr_1
    mov  R25,p_2
    com  R25
    in   R19,REG(PORT,PORT_X_1)
    and  R19,R25
    out  REG(PORT,PORT_X_1),usr_1
    in   R19, REG(DDR,PORT_X_1)
    mov  R25,p_2
    or   R19, R25
    out  REG(DDR,PORT_X_1), R19    
    cbi  REG( PORT, PORT_SMP ),SMP_BIT 
    sbi  REG( DDR, PORT_SMP ),SMP_BIT
    in   reg_clya, REG( DDR, PORT_YA ) 
    in   reg_clyb, REG( DDR, PORT_YB )
    in   reg_flya, REG( DDR, PORT_YA )
    in   reg_flyb, REG( DDR, PORT_YB )
    push r18
    mov  r18, p_3
    com  r18
    and  reg_flya,r18
    and  reg_flyb,r18
    pop  r18
#if (CLAMP_TO_DISCHARGE_TIME == 0)	
#elif (CLAMP_TO_DISCHARGE_TIME == 1)
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif ((CLAMP_TO_DISCHARGE_TIME - (3 * ((CLAMP_TO_DISCHARGE_TIME)/3))) == 0)
	_11100001_
	_10100001_
	_01101001_
#elif ((CLAMP_TO_DISCHARGE_TIME - (3 * ((CLAMP_TO_DISCHARGE_TIME)/3))) == 1)
	_11100001_
	_10100001_
	_01101001_
	_00011001_
#else
	_11100001_
	_10100001_
	_01101001_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb  
    pop  usr_2
    pop  usr_1
    ret
GLOBAL_FUNCTION _00110000010_
_00110000010_:
    push usr_1
    push usr_2 
    out  REG( DDR, PORT_YA ), reg_flya  
    out  REG( PIN, PORT_X_1 ), p_1  
#if (DELAY_PRECHARGE_TIME == 0)	
#elif (DELAY_PRECHARGE_TIME == 1)
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 0)
	_11100010_
	_10100010_
	_01101010_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 1)
	_11100010_
	_10100010_
	_01101010_
	_00011001_
#else
	_11100010_
	_10100010_
	_01101010_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_clyb  
#if (DELAY_DWELL_TIME == 0)	
#elif (DELAY_DWELL_TIME == 1)
	_00011001_
#elif (DELAY_DWELL_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 0)
	_11100011_
	_10100011_
	_01101011_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 1)
	_11100011_
	_10100011_
	_01101011_
	_00011001_
#else
	_11100011_
	_10100011_
	_01101011_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb    
    out  REG( DDR, PORT_YA ), reg_clya    
    out  REG( PIN, PORT_X_1 ), p_1      
#if (DELAY_X_DISCHARGE == 0)	
#elif (DELAY_X_DISCHARGE == 1)
	_00011001_
#elif (DELAY_X_DISCHARGE == 2)
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 0)
	_11100100_
	_10100100_
	_01101100_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 1)
	_11100100_
	_10100100_
	_01101100_
	_00011001_
#else
	_11100100_
	_10100100_
	_01101100_
	_00011001_
	_00011001_
#endif
    pop  usr_2
    pop  usr_1
    ret


#endif /*NUM_X_PORTS=1*/ 
/*//////////////////////////////////////////////////////////////////////////////////    */
#if (NUM_X_PORTS>=2)
GLOBAL_FUNCTION _00110000002_
_00110000002_:
    push usr_1
    push usr_2
    in   usr_1,REG(PORT,PORT_YB)
    mov  usr_2,p_1
    com  usr_2
    and  usr_1,usr_2
    out  REG(PORT,PORT_YB),usr_1
    in   usr_1,REG(PORT,PORT_YA)
    and  usr_1,usr_2
    out  REG(PORT,PORT_YA),usr_1
    mov  usr_2,p_1
    in   usr_1,REG(DDR,PORT_YB)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YB),usr_1
    in   usr_1,REG(DDR,PORT_YA)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YA),usr_1
    mov  R25,p_2
    com  R25
    in   R19,REG(PORT,PORT_X_2)
    and  R19,R25
    out  REG(PORT,PORT_X_2),usr_1
    in   R19, REG(DDR,PORT_X_2)
    mov  R25,p_2
    or   R19, R25
    out  REG(DDR,PORT_X_2), R19    
    cbi  REG( PORT, PORT_SMP ),SMP_BIT 
    sbi  REG( DDR, PORT_SMP ),SMP_BIT
    in   reg_clya, REG( DDR, PORT_YA ) 
    in   reg_clyb, REG( DDR, PORT_YB )
    in   reg_flya, REG( DDR, PORT_YA )
    in   reg_flyb, REG( DDR, PORT_YB )
    push r18
    mov  r18, p_3
    com  r18
    and  reg_flya,r18
    and  reg_flyb,r18
    pop  r18
#if (CLAMP_TO_DISCHARGE_TIME == 0)	
#elif (CLAMP_TO_DISCHARGE_TIME == 1)
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 0)
	_1011100001_
	_1010100001_
	_1001101001_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 1)
	_1011100001_
	_1010100001_
	_1001101001_
	_00011001_
#else
	_1011100001_
	_1010100001_
	_1001101001_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb  
    pop  usr_2
    pop  usr_1
    ret
GLOBAL_FUNCTION _00110000020_
_00110000020_:
    push usr_1
    push usr_2 
    out  REG( DDR, PORT_YA ), reg_flya  
    out  REG( PIN, PORT_X_2 ), p_1  
#if (DELAY_PRECHARGE_TIME == 0)	
#elif (DELAY_PRECHARGE_TIME == 1)
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 0)
	_1011100010_
	_1010100010_
	_1001101010_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 1)
	_1011100010_
	_1010100010_
	_1001101010_
	_00011001_
#else
	_1011100010_
	_1010100010_
	_1001101010_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_clyb  
#if (DELAY_DWELL_TIME == 0)	
#elif (DELAY_DWELL_TIME == 1)
	_00011001_
#elif (DELAY_DWELL_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 0)
	_1011100011_
	_1010100011_
	_1001101011_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 1)
	_1011100011_
	_1010100011_
	_1001101011_
	_00011001_
#else
	_1011100011_
	_1010100011_
	_1001101011_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb  
    out  REG( DDR, PORT_YA ), reg_clya  
    out  REG( PIN, PORT_X_2 ), p_1      
#if (DELAY_X_DISCHARGE == 0)	
#elif (DELAY_X_DISCHARGE == 1)
	_00011001_
#elif (DELAY_X_DISCHARGE == 2)
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 0)
	_1011100100_
	_1010100100_
	_1001101100_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 1)
	_1011100100_
	_1010100100_
	_1001101100_
	_00011001_
#else
	_1011100100_
	_1010100100_
	_1001101100_
	_00011001_
	_00011001_
#endif
    pop  usr_2
    pop  usr_1
    ret



#endif /*NUM_X_PORTS ==2*/
/*//////////////////////////////////////////////////////////////////////////////////    */
#if (NUM_X_PORTS==3)
GLOBAL_FUNCTION _00110000003_
_00110000003_:
    push usr_1
    push usr_2
    in   usr_1,REG(PORT,PORT_YB)
    mov  usr_2,p_1
    com  usr_2
    and  usr_1,usr_2
    out  REG(PORT,PORT_YB),usr_1
    in   usr_1,REG(PORT,PORT_YA)
    and  usr_1,usr_2
    out  REG(PORT,PORT_YA),usr_1
    mov  usr_2,p_1
    in   usr_1,REG(DDR,PORT_YB)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YB),usr_1
    in   usr_1,REG(DDR,PORT_YA)
    or   usr_1,usr_2
    out  REG(DDR,PORT_YA),usr_1
    mov  R25,p_2
    com  R25
    in   R19,REG(PORT,PORT_X_3)
    and  R19,R25
    out  REG(PORT,PORT_X_3),usr_1
    in   R19, REG(DDR,PORT_X_3)
    mov  R25,p_2
    or   R19, R25
    out  REG(DDR,PORT_X_3), R19    
    cbi  REG( PORT, PORT_SMP ),SMP_BIT 
    sbi  REG( DDR, PORT_SMP ),SMP_BIT
    in   reg_clya, REG( DDR, PORT_YA ) 
    in   reg_clyb, REG( DDR, PORT_YB )
    in   reg_flya, REG( DDR, PORT_YA )
    in   reg_flyb, REG( DDR, PORT_YB )
    push r18
    mov  r18, p_3
    com  r18
    and  reg_flya,r18
    and  reg_flyb,r18
    pop  r18
#if (CLAMP_TO_DISCHARGE_TIME == 0)	
#elif (CLAMP_TO_DISCHARGE_TIME == 1)
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 0)
	_1111100001_
	_1110100001_
	_1101101001_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 1)
	_1111100001_
	_1110100001_
	_1101101001_
	_00011001_
#else
	_1111100001_
	_1110100001_
	_1101101001_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb  
    pop  usr_2
    pop  usr_1
    ret
GLOBAL_FUNCTION _00110000030_
_00110000030_:
    push usr_1
    push usr_2 
    out  REG( DDR, PORT_YA ), reg_flya  
    out  REG( PIN, PORT_X_3 ), p_1  
#if (DELAY_PRECHARGE_TIME == 0)	
#elif (DELAY_PRECHARGE_TIME == 1)
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 0)
	_1111100010_
	_1110100010_
	_1101101010_
#elif (DELAY_PRECHARGE_TIME - (3 * (DELAY_PRECHARGE_TIME/3)) == 1)
	_1111100010_
	_1110100010_
	_1101101010_
	_00011001_
#else
	_1111100010_
	_1110100010_
	_1101101010_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_clyb  
#if (DELAY_DWELL_TIME == 0)	
#elif (DELAY_DWELL_TIME == 1)
	_00011001_
#elif (DELAY_DWELL_TIME == 2)
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 0)
	_1111100011_
	_1110100011_
	_1101101011_
#elif (DELAY_DWELL_TIME - (3 * (DELAY_DWELL_TIME/3)) == 1)
	_1111100011_
	_1110100011_
	_1101101011_
	_00011001_
#else
	_1111100011_
	_1110100011_
	_1101101011_
	_00011001_
	_00011001_
#endif
    out  REG( DDR, PORT_YB ), reg_flyb    
    out  REG( DDR, PORT_YA ), reg_clya    
    out  REG( PIN, PORT_X_3 ), p_1      
#if (DELAY_X_DISCHARGE == 0)	
#elif (DELAY_X_DISCHARGE == 1)
	_00011001_
#elif (DELAY_X_DISCHARGE == 2)
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 0)
	_1111100100_
	_1110100100_
	_1101101100_
#elif (DELAY_X_DISCHARGE - (3 * (DELAY_X_DISCHARGE/3)) == 1)
	_1111100100_
	_1110100100_
	_1101101100_
	_00011001_
#else
	_1111100100_
	_1110100100_
	_1101101100_
	_00011001_
	_00011001_
#endif
    pop  usr_2
    pop  usr_1
    ret



#endif /*NUM_X_PORTS==3*/ 


GLOBAL_FUNCTION _00110000011_
_00110000011_:
    sbi  REG( PORT, PORT_SMP ),SMP_BIT  
    ret
   
GLOBAL_FUNCTION _00110000110_
_00110000110_:
    cbi  REG( PORT, PORT_SMP ),SMP_BIT  
    out  REG( DDR, PORT_YB ), reg_clyb  
#if (CLAMP_TO_DISCHARGE_TIME == 0)	
#elif (CLAMP_TO_DISCHARGE_TIME == 1)
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 2)
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME == 3)
	_00011001_
	_00011001_
	_00011001_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 0)
	_11100101_
	_10100101_
	_01101101_
#elif (CLAMP_TO_DISCHARGE_TIME - (3 * (CLAMP_TO_DISCHARGE_TIME/3)) == 1)
	_11100101_
	_10100101_
	_01101101_
	_00011001_
#else
	_11100101_
	_10100101_
	_01101101_
	_00011001_
	_00011001_
#endif
    ret
    
    
FILE_FOOTER
