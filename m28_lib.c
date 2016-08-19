#include <msp430g2553.h>
#include "uartmsp.h"
#include "m28_lib.h"



/*
 * Restores Dynamixel MX28-R
 * for factory config
 * INPUT: ID motor
 * RETURN: 1 if the data is correct, 0 if not
 */
void factory_reset(unsigned int id)
{
	//Reset return packet
	unsigned int reset_rx[SIZE_RXBUF_RESET];

	//check sum for the packet reset
	unsigned int sum;

	//calculation of the check sum
	sum = ~(id + LENGTH_0 + RESET) + 255 ;

	//Reset Instruction packet declaration
	unsigned int reset_packet[SIZE_TXBUF_RESET]= {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_0,RESET,sum};

	//Data sending function
	send_data(reset_packet,SIZE_TXBUF_RESET);

	//Data receiving funcion
	receive_data(reset_rx,SIZE_RXBUF_RESET);

}






/* Function used to receive the instant
 * temperature value
 * INPUT: ID motor
 * RETURN: Value of temperature if the data
 * is correct, 0 if not */
int temp_check(unsigned int id)
{
	//Temperature return packet
	unsigned int temp_rx[SIZE_RXBUF_TEMP];

	//Temperature Value
	unsigned int temp_out;

	//check sum for the packet reset
	unsigned int sum;

	//Calculation of the check sum
	sum = ~(id + LENGTH_2 + READ_DATA + PRESENT_TEMP + DLENGTH_1) + 0xFF;

	//Temperature Check Instruction packet declaration
	unsigned int temp_tx[SIZE_TXBUF_TEMP] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_TEMP,DLENGTH_1, sum};

	//Data sending function
	send_data(temp_tx , SIZE_TXBUF_TEMP);

	//Data receiving function
	receive_data(temp_rx,SIZE_RXBUF_TEMP);

	//Selecting the temperature info
	temp_out = temp_rx[5];


	//If the check sum of packet receive is correct, return temp_out
	if(check_sum(temp_rx, SIZE_RXBUF_TEMP	))
	return temp_out;

	//Case not, return 0
	else
	return 0;
}




/* Function used to receive the instant
 * voltage value
 * INPUT: ID motor
 * RETURN: Value of temperature if the data
 * is correct, 0 if not */
int voltage_check(unsigned int id)
{
	//Voltage return packet
	unsigned int voltage_rx[7];

	//Voltage Value
	unsigned int voltage_out;

	//check sum for the packet reset
	unsigned int sum;

	//Calculation of the check sum
	sum = ~(id + LENGTH_2 + READ_DATA + PRESENT_VOLTAGE + DLENGTH_1) + 0xFF;

	//Voltage Check Instruction packet declaration
	unsigned int voltage_tx[SIZE_TXBUF_VOLT] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_VOLTAGE,DLENGTH_1,sum};

	//Data sending function
	send_data(voltage_tx,SIZE_TXBUF_VOLT);

	//Data receiving function
	receive_data(voltage_rx,SIZE_RXBUF_VOLT);

	//Selecting and calculate the voltage info
	voltage_out = voltage_rx[5]/10;

	//if the check sum of packet receive is correct, return temp_out
	if(check_sum(voltage_rx, SIZE_RXBUF_VOLT))
	return voltage_out;

	//case not, return 0
	else
	return 0;
}





/* Changes MX28 ID
 * INPUT: Actual ID  e new ID
 * RETURN: nothing */
void change_id(unsigned int id, unsigned int newid)
{

	//check sum for the packet reset
	unsigned int sum;

	//Calculation of the check sum
	sum = id + LENGTH_2 + READ_DATA + newid;

	//id_check instruction packet declaration
	unsigned int id_tx[SIZE_TXBUF_ID] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,WRITE_DATA, newid, sum};

	//Data sending function
	send_data(id_tx , SIZE_TXBUF_ID);

}


/* Function used to receive the instant
 * value of axis position
 * INPUT: ID motor
 * RETURN: Axis position value, 0 if it is not.
 */

int position_check(unsigned int id)
{
	//position low return packet
	unsigned int position_l_rx[SIZE_RXBUF_POS];

	//position high return packet
	unsigned int position_h_rx[SIZE_RXBUF_POS];

	//Position low Value
	unsigned int position_l;

	//Position high Value
	unsigned int position_h;

	//Position value out
	unsigned int position_out;

	//Check sum for the packet reset
	unsigned int sum_l;

	//Check sum for the packet reset
	unsigned int sum_h;

	//Calculation of the check sum
	sum_l = ~(id + LENGTH_2 + READ_DATA + PRESENT_POSITION_L + DLENGTH_1) + 0xFF;

	//Calculation of the check sum
	sum_h = ~(id + LENGTH_2 + READ_DATA + PRESENT_POSITION_H + DLENGTH_1) + 0xFF;

	//Position low Check Instruction packet declaration
	unsigned int position_l_tx[SIZE_TXBUF_POS] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_POSITION_L,DLENGTH_1,sum_l};

	//Position high Check Instruction packet declaration
	unsigned int position_h_tx[SIZE_TXBUF_POS] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_POSITION_H,DLENGTH_1,sum_h};

	//Data sending function
	send_data(position_l_tx,SIZE_TXBUF_POS);

	//Data receiving function
	receive_data(position_l_rx,SIZE_RXBUF_POS);

	//Data sending function
	send_data(position_h_tx,SIZE_TXBUF_POS);

	//Data receiving function
	receive_data(position_h_rx,SIZE_RXBUF_POS);

	//displacement of 8 bits
	position_h = position_h_rx[5] << 8;

	//Value of position low
	position_l = position_l_rx[5];

	//Value of position complete
	position_out = position_h + position_l;

	//Conversion position in degrees
	position_out = position_out * 0.088;


	//if the check sum of packet receive is correct, return position_out
	if(check_sum(position_h_rx, SIZE_RXBUF_POS)){
		if(check_sum(position_l_rx, SIZE_RXBUF_POS))
		return position_out;

		else
		return 0;
	}
	//case not, return 0
	else
	return 0;

}


/*Checks the motor current load
Parameter: id
Return: Percentage of max torque */

int load_check(unsigned int id){

	//Checksum value
	unsigned int sum_l= (~(id +LENGTH_2+READ_DATA+LOAD_L_ADDR+DLENGTH_1))+255;
	unsigned int sum_h= (~(id +LENGTH_2+READ_DATA+LOAD_H_ADDR+DLENGTH_1))+255;

 	//TX vectors declarations
 	unsigned int load_ltx[SIZE_TXBUF_LOAD] ={BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,LOAD_L_ADDR,DLENGTH_1,sum_l};
 	unsigned int load_htx[SIZE_TXBUF_LOAD] ={BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,LOAD_H_ADDR,DLENGTH_1,sum_h};

 	//RX vectors declarations
 	unsigned int load_lrx[SIZE_TXBUF_LOAD];
 	unsigned int load_hrx[SIZE_RXBUF_LOAD];

 	// Sennd loud instruction packet
 	send_data(load_ltx,SIZE_TXBUF_LOAD);

 	// Receive loud status packet
 	receive_data(load_lrx,SIZE_RXBUF_LOAD);

 	// Sennd loud instruction packet
 	send_data(load_htx,SIZE_TXBUF_LOAD);

 	// Receive loud status  packet
 	receive_data(load_hrx,SIZE_RXBUF_LOAD);

 	//Complete receive value
	unsigned char parameter;

	//Calculation parameter
	parameter = load_hrx[5] <<8 + load_lrx[5];

	//Load direction
	unsigned char load_direction;

	//Load direction. CCW=1 , CW=0

	//Percentage of load in relation to max_torque
	unsigned int load_total;

	//Checks if the load is in CCW direction
	if( parameter <= 1023){
	//Calculates the percentage of max torque
	load_total= (parameter/1023)*PERCENTAGE;

	return load_total;

	}

	//load is in CW direction
	else if(1023 < parameter <=2047){

	//Calculates the percentage of max torque
	load_total = ((parameter-1023)/1023)*PERCENTAGE;

	return  load_total;
 }
}




/*Checks the motor current speed
Parameter: Dynamixel ID
Return: Actual speed in rpm */
int speed_check(unsigned int id)
{
 	//Speed low return packet
 	unsigned int speed_l_rx[SIZE_RXBUF_SPEED];

 	//position high return packet
 	unsigned int speed_h_rx[SIZE_RXBUF_SPEED];;

 	//Position low Value
 	unsigned int speed_l;

 	//speed high Value
 	unsigned int speed_h;

 	//Speed parameter value
 	unsigned char speed_param;

	//Speed value out
 	unsigned char speed_out;

 	//Check sum for the packet reset
 	unsigned int sum_l;

 	//Speed direction. CCW=1 ; CW=0
 	unsigned int speed_direction;

 	//Check sum for the packet reset
 	unsigned int sum_h;

 	//Calculation of the check sum
 	sum_l = ~(id + LENGTH_2 + READ_DATA + PRESENT_SPEED_L + DLENGTH_1) + 0xFF;

 	//Calculation of the check sum
 	sum_h = ~(id + LENGTH_2 + READ_DATA + PRESENT_SPEED_H + DLENGTH_1) + 0xFF;

 	//speed low Check Instruction packet declaration
 	unsigned int speed_l_tx[SIZE_TXBUF_SPEED] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_SPEED_L,DLENGTH_1,sum_l};

	 //speed high Check Instruction packet declaration
	unsigned int speed_h_tx[SIZE_TXBUF_SPEED] = {BEGIN_PACKET,BEGIN_PACKET,id,LENGTH_2,READ_DATA,PRESENT_SPEED_H,DLENGTH_1,sum_h};

	//Data sending function
	send_data(speed_l_tx,SIZE_TXBUF_SPEED);

	//Data receiving function
	receive_data(speed_l_rx,SIZE_RXBUF_SPEED);

	//Data sending function
	send_data(speed_h_tx,SIZE_TXBUF_SPEED);

	//Data receiving function
	receive_data(speed_h_rx,SIZE_RXBUF_SPEED);

	//Displacement of 8 bits
	speed_h = speed_h_rx[5] << 8;

	//Value of speed low
	speed_l = speed_l_rx[5];

	//Value of speed complete
	speed_param = speed_h + speed_l;

	//Checks if the motor rotates is in CCW direction
	if( speed_param <= 1023){

	//Calculates the percentage of max torque
	speed_out= speed_param*SPEED_CONSTANT;
	speed_direction = 1;

	}

	//Motor rotates in CW direction
	else if(1023 < speed_param <=2047){

		//Calculates the speed value in rpm
		speed_out= ((speed_param-1023)*SPEED_CONSTANT);
		speed_direction=0;
	}

 return speed_out;

}


