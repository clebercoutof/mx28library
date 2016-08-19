#ifndef M28_LIB_H_
#define M28_LIB_H_

#define BEGIN_PACKET				0xFF
#define ID01						0x01
#define ID02						0x02
#define ID03						0x03
#define ID04						0x04
#define ID05						0x05

#define SIZE_TXBUF_RESET			0x06
#define SIZE_RXBUF_RESET			0x06

#define SIZE_TXBUF_TEMP				0x08
#define SIZE_RXBUF_TEMP				0x07

#define SIZE_TXBUF_VOLT				0x08
#define SIZE_RXBUF_VOLT				0x07

#define SIZE_TXBUF_ID				0x08
#define SIZE_RXBUF_ID				0x06

#define SIZE_TXBUF_POS				0x08
#define SIZE_RXBUF_POS				0x07

#define LOAD_L_ADDR  				 0x28
#define LOAD_H_ADDR   				0x29
#define SIZE_TXBUF_LOAD  			0x08
#define SIZE_RXBUF_LOAD  			0x07

#define PRESENT_SPEED_L				0x26
#define PRESENT_SPEED_H				0x27
#define SIZE_TXBUF_SPEED			0x08
#define SIZE_RXBUF_SPEED			0x07
#define SPEED_CONSTANT				0.11

#define RESET_ID					0x00
#define LENGTH_0 					0x02
#define LENGTH_2 					0x04
#define LENGTH_3 					0x05
#define LENGTH_4 					0x06
#define LENGTH_5 					0x07
#define LENGTH_6 					0x08
#define PING 						0x01
#define READ_DATA 					0x02
#define WRITE_DATA					0x03
#define REG_WRITE 					0x04
#define ACTION 						0x05
#define RESET						0x06
#define SYNC_WRITE					0x83
#define CHECKSUM_RESET				0xF7
#define PRESENT_TEMP				0x2B
#define DLENGTH_1					0x01
#define DLENGTH_2					0x02
#define CHECKSUM_TC					0xCC
#define CHECKSUM_VOLT				0xCB
#define PRESENT_VOLTAGE				0X2A
#define PRESENT_POSITION_L			0x24
#define PRESENT_POSITION_H			0x25
#define PERCENTAGE					100

void factory_reset(unsigned int id);
int temp_check(unsigned int id);
int voltage_check(unsigned int id);
void change_id(unsigned int id, unsigned int newid);
int load_check(unsigned int id);
int position_check(unsigned int id);
int speed_check(unsigned int id);

#endif /* M28_LIB_H_ */
