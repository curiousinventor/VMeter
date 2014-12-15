#ifndef BITBANGSPI_MASTER_H_INCLUDED
#define BITBANGSPI_MASTER_H_INCLUDED

/*============================ PROTOTYPES ====================================*/
void BitBangSPI_Master_Init (void);
void BitBangSPI_Send_Message(void);
void BitBangSPI_Send_Byte(unsigned char c);

/*============================ MACROS ========================================*/
//#define SS_BB       0
//#define SCK_BB      1
//#define MOSI_BB     2
//#define MISO_BB     3

//#define SPI_BB      B

#define SS_BB   6    // 6 .// the latch...
#define SCK_BB  0   // 0
#define MOSI_BB 5   // 5
#define MISO_BB 3   // 7 // not used?

#define SPI_BB   D  // D

#define __delay_cycles __builtin_avr_delay_cycles
#define delay_us( us )   ( __delay_cycles( ( F_CPU / 1000000UL ) * ( us ) ) )

#define CPU_SPEED   8	// MHz
#define DELAYUS(DELAY) __delay_cycles((DELAY)*CPU_SPEED);

#endif
/* EOF */
