#ifndef SPI_HARDWARE_H
#define SPI_HARDWARE_H

#define HARDWARE_SS 0 // port B

void hardware_spi_init_as_master (void);
void hardware_spi_init_as_slave (void);
void hardware_spi_send_byte(unsigned char);
// receive will happen in the interrupt
#endif


