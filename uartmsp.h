#ifndef UARTMSP_H_
#define UARTMSP_H_

#include <stdio.h>
#include <math.h>
#define ALL_ZERO     0x00

void uart_config(void);
void send_data(unsigned int* tx_vector, unsigned int size);
void receive_data(unsigned int* rx_vector, unsigned int size);
int check_sum(unsigned int *vector, unsigned int size);
#endif /* UARTMSP_H_ */
