#include <msp430g2553.h>
#include "uartmsp.h"


/*
 * Função que configura os registradores do microcontrolador para
 * possibilitar a comunicação via uart dos dois.
 * Porta 1.1 utilizada como RX, e Porta 1.2 utilizada como TX.
 */
void uart_config(void)
{
	// UCA logic held in reset state to be configured
	UCA0CTL1 |= UCSWRST ;

	// Clock Source: SMCLK(1,1MHz)
	UCA0CTL1 |= UCSSEL_2 ;

	//Global interruption Enable
	__bis_SR_register(GIE);

	// 1.1MHz/ 57600
	UCA0BR0 = 19;
	UCA0BR1 = ALL_ZERO;

	//USCI Second Stage Modulation: 1
	UCA0MCTL |= UCBRS_1;

	// P1.1 RX, P1.2 TX
	P1SEL |= BIT2 + BIT1;

	// P1.1 RX, P1.2 TX
	P1SEL2 |= BIT2 + BIT1;

	//UCA0 out of reset
	UCA0CTL1 &= ~UCSWRST;
}


/*
 * Função para mandar os pacotes
 * possibilitar a comunicação via uart dos dois.
 * Porta 1.1 utilizada como RX, e Porta 1.2 utilizada como TX.
 */
void send_data(unsigned int* tx_vector, unsigned int size)
{
		unsigned int i = 0;
		//receive enable
		P1DIR |= BIT4 + BIT5;
		P1OUT |= BIT4 + BIT5;

		//Loop for data sending
		for( i=0; i < size; i++)
			{

			UCA0TXBUF = tx_vector[i];

				//UCATX	buffer check
				while(!(UC0IFG & UCA0TXIFG) ) ;
			}
}
/*
 * Função para receber os pacotes
 * da UART
 * INPUT= vetor que sera inscrito e tamanho do vetor
 * RETURN= nada
 */
void receive_data(unsigned int* rx_vector, unsigned int size)
{

			unsigned int i;
			//Loop for data receiving
			for(i=0 ; i < size; i++)
				{

					//UCARX buffer check
					while(!(UC0IFG & UCA0RXIFG)) ;

					//Received data vector
					rx_vector[i]= UCA0RXBUF;

					//UCA0RX interruption flag reset
					UC0IFG &= ~UCA0RXIFG;;
		}
}



/*
 * Verify the packet checksum
 * INPUT= Vector and its size
 * RETURN= 1 case correct, 0 case not.
 */
 int check_sum(unsigned int *vector, unsigned int size)
 {
	 //Checksum error declaration. 1= No error; 0= Error
	 int check;
	 //Checksum value declaratin
	 int sum = 0;
	 //Vector Size
	 int i = size - 1;

	 //Loop for received vector checksum calculus
	 while(i > 2){

		 sum = sum + vector[i];
		 i = size - 1;

 }

	 //Checks if calculated value is the same value of checksum byte of the received vector
	 if (sum == vector[size])
		 check = 1;
		 else
		check = 0;

	 //Returns checksum error
	 return check;

 }



/*Sets MSP430 baud rate
Parameter: Baud rate chosen by user
Return: None */

void uart_baudrate(float baudrate){


	// UCA logic held in reset state to be configured
	UCA0CTL1 |= UCSWRST ;

	// Clock Source: SMCLK(1,1MHz)
	UCA0CTL1 |= UCSSEL_2 ;

	// Division factor(SMCLK/BAUDRATE)
	float div_factor;
	div_factor = 1100000/baudrate;

	//Integer portion of the division factor
	unsigned int integer_portion;
	integer_portion = div_factor;

	//Value of the modulation needed
	unsigned int modulation;
	modulation=round((div_factor)-integer_portion)*8;

	//Setting the baud rate control registers
	UCA0BR0 = integer_portion;
	UCA0BR1 = 0x00;

	//Setting the modulation control register
	UCA0MCTL = modulation;

}

