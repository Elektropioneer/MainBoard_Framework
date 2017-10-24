#include "can.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define RX_BUFFER_SIZE		5

// every message contains 8 bytes and the indentificator of the message
static volatile unsigned char *rxBuffers[10];
static volatile unsigned char rxWrIndex[10];
static volatile unsigned char rxRdIndex[10];
static volatile unsigned char rxCounter[10];
//volatile static long canTime;

/*
 * 	Function: ISR(CANIT_vect)
 * 	Description: Interrupt Service Routine on CAN
 */
ISR(CANIT_vect) {
	unsigned char tempPage = CANPAGE;
	unsigned char ch, ide;
	unsigned char i;

	// Checking all MoB's
	for(ch = 0; ch < 15; ch++) {
		CANPAGE = ch << 4;

		// receive interrupt
		if( (CANSTMOB >> RXOK) & 0x01) {
			CANCDMOB = (1 << CONMOB1); // settings MoB as receiver
			CANSTMOB &= ~(1 << RXOK); // reseting the  flag

			ide = (CANIDT2 >> 5) | (CANIDT1 << 3);

			for(i = 0; i < 8; ++i)
				(*(rxBuffers + ide - 1) + sizeof(unsigned char) * (*(rxWrIndex + ide - 1)))[i] = CANMSG;

			if(++(*(rxWrIndex + ide - 1)) == RX_BUFFER_SIZE)
				*(rxWrIndex + ide - 1) = 0;

			(*(rxCounter + ide - 1))++;

			break;
		}

		//transmit interrupt
		if( (CANSTMOB >> TXOK) & 0x01) {
			CANCDMOB = 0; // disable mob
			CANSTMOB &= ~(1 << TXOK); // reset flag

			break;
		}
	}

	CANPAGE = tempPage;
}

/*
 * 	Function:    static void CAN_InitModule(void)
 * 	Description: Init the CAN module
 */
static void CAN_InitModule(void) {
	unsigned char ch;
	unsigned char data;

	// Reset the CAN controller
	CANGCON = 0x01;

	// reseting all MOb's because after reset they dont have a defined state
	for(ch = 0; ch < 15; ch++) {
		CANPAGE = ch << 4;

		CANSTMOB = 0;
		CANCDMOB = 0;

		CANIDT1 = 0;
		CANIDT2 = 0;
		CANIDT3 = 0;
		CANIDT4 = 0;
		CANIDM1 = 0;
		CANIDM2 = 0;
		CANIDM3 = 0;
		CANIDM4 = 0;

		// CANMSG -> 8 byte FIFO row
		for(data = 0; data < 8; data++)
			CANMSG = 0;
	}


	// Enable the CAN controller
	CANGCON = 0x02;

	/************ BIT TAJMING PODESAVANJA -> BAUD RATE = 250kbps  **********************************/

	//CANBT1 = 0x08; // Fcan = 2Mhz-> Tq = 0.5us
	CANBT1 = 0x01;
	CANBT2 = (1 << PRS2); // Propagation Time Segment-> 5xTq
	CANBT3 = (1 << PHS22) | (1 << PHS20) | (1 << PHS10) | (1 << PHS11) | (1 << PHS12); // Phase Segment 2-> 6xTq, Phase Segment 1-> 8xTq

	/// Sjw je podesen na 1... 1 + 5 + 6 + 8 = 22
	/************ BIT TAJMING PODESAVANJA -> BAUD RATE = 250kbps  **********************************/

	CANTCON = 0; // preskaler za CAN timer-> ne koristimo ga pa ova vrednost nije vazna

	// ukljucujem interrupte- ENIT-> Enable All Interrupts
	// ENRX-> Enable RX interrupt
	// ENTX-> Enable TX interrupt
	CANGIE = (1 << ENIT) | (1 << ENRX) | (1 << ENTX);
	// interrupti svih MOb- ova su ukljuceni
	CANIE1 = 0xFF >> 1; // glupo govno trazi da MSB mora biti nula prilikom upisivanja u registar
	CANIE2 = 0xFF;
	sei();
}

/*
 * 	Function:    static char CAN_InitRxMob(unsigned int ide)
 * 	Description: Create an Rx Mob based on the ide
 */
static char CAN_InitRxMob(unsigned int ide) {
	unsigned char ch;
	unsigned char tempPage = CANPAGE;

	// until busy
	while(((CANGSTA >> TXBSY) & 0x01) || ((CANGSTA >> RXBSY) & 0x01));

	// checking for free MObs
	for(ch = 0; ch < 15; ch++) {
		CANPAGE = ch << 4;

		// if not free continue
		if((CANCDMOB >> 6))
			continue;

		// receiving 8 bytes
		CANCDMOB = (1 << DLC0) | (1 << DLC1) | (1 << DLC2) | (1 << DLC3);

		// set indentificator
		CANIDT1 = (0x00FF & ide) >> 3;
		CANIDT2 = (ide & 0x00FF) << 5;

		// set mask - look at all the bits
		CANIDM2 = 0x07 << 5;
		CANIDM1 = 0xFF;

		// settings MOb as receiver
		CANCDMOB |= (1 << CONMOB1);

		CANPAGE = tempPage;

		return 1;
	}

	CANPAGE = tempPage;

	return 0;
}

/*
 * 	Function:    void CAN_Init(unsigned char numOfNodes)
 * 	Description: Init can with specified number of nodes
 */
void CAN_Init(unsigned char numOfNodes) {
    unsigned char i;
	CAN_InitModule();

	//CAN_InitRxMob(DRIVER_RX_IDENTIFICATOR);
	//CAN_InitRxMob(XBEE_RX_IDENTIFICATOR);
	//CAN_InitRxMob(KINEZ_RX_IDENTIFICATOR);

	for(i = 0; i < numOfNodes; ++i)
	{
		CAN_InitRxMob(i + 1);
		*(rxBuffers + i) = (unsigned char *) calloc(RX_BUFFER_SIZE, sizeof(unsigned char));
		//*(rxBuffers + i) = (unsigned char *) malloc(RX_BUFFER_SIZE * sizeof(unsigned char));
		rxWrIndex[i] = rxRdIndex[i] = rxCounter[i] = 0;
	}

	//rxWrIndex = (unsigned char *) calloc(RX_BUFFER_SIZE, sizeof(unsigned char));
	//rxRdIndex = (unsigned char *) calloc(RX_BUFFER_SIZE, sizeof(unsigned char));
}

/*
 * 	Function: 	 unsigned char CAN_CheckRX(unsigned char nodeID)
 * 	Description: num of not read data from the node
 */
unsigned char CAN_CheckRX(unsigned char nodeID) {
	return rxCounter[nodeID - 1];
}

/*
 * 	Function:    char CAN_Read(unsigned char *buffer, unsigned char sendingNodeID)
 * 	Description: read the buffer from the node id
 */
char CAN_Read(unsigned char *buffer, unsigned char sendingNodeID) {
	unsigned char i;

	// until no data
	while(!rxCounter[sendingNodeID - 1]);

	for(i = 0; i < 8; ++i)
	*(buffer + i) = (*(rxBuffers + sendingNodeID - 1) + sizeof(unsigned char) * (*(rxRdIndex + sendingNodeID - 1)))[i];

	if(++(*(rxRdIndex + sendingNodeID - 1)) == RX_BUFFER_SIZE)
	*(rxRdIndex + sendingNodeID - 1) = 0;

	rxCounter[sendingNodeID - 1]--;

	return 0;
}

/*
 * 	Function:    char CAN_Write(unsigned char *data, unsigned char receivingNodeAddress)
 * 	Description: write array of data to the receiving node address
 */
char CAN_Write(unsigned char *data, unsigned char receivingNodeAddress) {
    unsigned char i;

    // waiting for not busy
	while(((CANGSTA >> TXBSY) & 0x01) || ((CANGSTA >> RXBSY) & 0x01));

	unsigned char ch;
	unsigned char tempPage = CANPAGE;

	// waiting for a free MOb
	for(ch = 0; ch < 15; ch++) {
		CANPAGE = ch << 4;

		if(!(CANCDMOB >> 6))
			break;
	}

	// no free MObs found
	if(ch == 15)
		return -1;


	// this is the indentificator
	CANIDT2 = (receivingNodeAddress & 0x0F) << 5;
	CANIDT1 = (receivingNodeAddress) >> 3;

	// DLC = 8
	CANCDMOB = 8;

	// writing the 8byte
	for(i = 0; i < 8; i++)
		CANMSG = data[i];

	// as a sender
	CANCDMOB |= (1 << CONMOB0);

	CANPAGE = tempPage;

	return 0;
}
