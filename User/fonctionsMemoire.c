#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "global.h"

void init_i2c_eeprom(){
	//activation I2C
	I2C_Init(LPC_I2C0, 1000000);//1MHz
	I2C_Cmd(LPC_I2C0, ENABLE);
}

/** Write 'nbBytes' bytes from 'dataArray' at the address 'addr' of the FRAM module
 * dataArray bytes 1 to 'nbBytes' are send (first byte need to be 0)
 */
void write_dataI2C(uint32_t addr, uint8_t* dataArray, uint32_t nbBytes){
	I2C_M_SETUP_Type setup;
	
	uint8_t data[256];
	int i;
	for(i=0; i<nbBytes; i++){
		data[i+1]=dataArray[i];
	}
	
	addr = addr & 0x7ff; //limit addr to 11 bits
 	setup.sl_addr7bit = 10<<3 | addr>>8; //01010xxx :  xxx are the 3 most significant bit
	data[0] = (uint8_t) addr&0xff; //word address
	setup.tx_data = data;					/**< Pointer to Transmit data - NULL if data transmit is not used */
  setup.tx_length = nbBytes+1;				/**< Transmit data length - 0 if data transmit is not used*/
  setup.rx_data = NULL;								/**< Pointer to Receive data - NULL if data receive is not used */
  setup.rx_length = 0;								/**< Receive data length - 0 if data receive is not used */
  setup.retransmissions_max = 10;			/**< Max Re-Transmission value */
  I2C_MasterTransferData(LPC_I2C0, &setup, I2C_TRANSFER_POLLING);
}

/** Fill 'dataArray' with 'nbBytes' bytes read at the address 'addr' of the FRAM module*/
void read_dataI2C(uint32_t addr, uint8_t* dataArray, uint32_t nbBytes){
	I2C_M_SETUP_Type setup;
	uint8_t wordAddr;
	addr = addr & 0x7ff; //limit addr to 11 bits
 	setup.sl_addr7bit = 0x50 | addr>>8; //01010xxx :  xxx are the 3 most significant bit
	wordAddr = (uint8_t) addr&0xff;
	setup.tx_data = &wordAddr;					/**< Pointer to Transmit data - NULL if data transmit is not used */
  setup.tx_length = 1;								/**< Transmit data length - 0 if data transmit is not used*/
  setup.rx_data = dataArray;					/**< Pointer to Receive data - NULL if data receive is not used */
  setup.rx_length = nbBytes;					/**< Receive data length - 0 if data receive is not used */
  setup.retransmissions_max = 10;			/**< Max Re-Transmission value */
  I2C_MasterTransferData(LPC_I2C0, &setup, I2C_TRANSFER_POLLING);
}
