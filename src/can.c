#include "can.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define RX_BUFFER_SIZE		5


// unutrasnje promenljive koje sluze za komunikaciju
// svaka poruka sadrzi 8 bajtova podataka i identifikator poruke

static volatile unsigned char *rxBuffers[10];
static volatile unsigned char rxWrIndex[10];
static volatile unsigned char rxRdIndex[10];
static volatile unsigned char rxCounter[10];
//volatile static long canTime;

// Interrupt Service Routine
ISR(CANIT_vect)
{
	unsigned char tempPage = CANPAGE;
	unsigned char ch, ide;
	unsigned char i;

	// prolazim redom kroz MOb- ove i gledam da li se nesto dogadja
	for(ch = 0; ch < 15; ch++)
	{
		CANPAGE = ch << 4;

		// receive interrupt
		if( (CANSTMOB >> RXOK) & 0x01)
		{
			// PRIJEMNI MOb- ovi uvek ostaju prijemni!
			CANCDMOB = (1 << CONMOB1); // podesavam MOb kao receiver
			CANSTMOB &= ~(1 << RXOK); // resetujem flag

			ide = (CANIDT2 >> 5) | (CANIDT1 << 3);

			for(i = 0; i < 8; ++i)
				(*(rxBuffers + ide - 1) + sizeof(unsigned char) * (*(rxWrIndex + ide - 1)))[i] = CANMSG;

			if(++(*(rxWrIndex + ide - 1)) == RX_BUFFER_SIZE)
				*(rxWrIndex + ide - 1) = 0;

			(*(rxCounter + ide - 1))++;

			break;
		}

		//transmit interrupt
		if( (CANSTMOB >> TXOK) & 0x01)
		{
			CANCDMOB = 0; // disable mob
			CANSTMOB &= ~(1 << TXOK); // resetujem flag

			break;
		}
	}

	CANPAGE = tempPage;
	//CANGIT = CANGIT; // zato sto tako treba.
}


static void CAN_InitModule(void)
{
	unsigned char ch;
	unsigned char data;

	// Reset the CAN controller
	CANGCON = 0x01;

	// idem od 0 do 14 MOba i resetujem ih, zato sto posle reseta MObovi imaju nedefinisano stanje
	for(ch = 0; ch < 15; ch++)
	{
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

		// CANMSG je organizovan kao 8- bajtni FIFO red
		// koristi se auto-increment pa ce proci kroz svih 8 bajtova poruke-> organizovan kao FIFO
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


static char CAN_InitRxMob(unsigned int ide)
{
	unsigned char ch;
	unsigned char tempPage = CANPAGE;

	// sve dok je magistrala zauzeta
	while(((CANGSTA >> TXBSY) & 0x01) || ((CANGSTA >> RXBSY) & 0x01));

	// prolazim redom kroz MOb- ove, cim naidjem na slobodan MOb
	for(ch = 0; ch < 15; ch++)
	{
		CANPAGE = ch << 4;

		// ako je MOb vec zauzet nastavi dalje
		if((CANCDMOB >> 6))
			continue;

		//uvek primam 8 bajtova
		CANCDMOB = (1 << DLC0) | (1 << DLC1) | (1 << DLC2) | (1 << DLC3);

		// podesavam identifikator
		CANIDT1 = (0x00FF & ide) >> 3;
		CANIDT2 = (ide & 0x00FF) << 5;

		// podesavam masku- gledam sve bitove
		CANIDM2 = 0x07 << 5;
		CANIDM1 = 0xFF;

		// podesavam MOb kao receiver
		CANCDMOB |= (1 << CONMOB1);

		CANPAGE = tempPage;

		return 1;
	}

	CANPAGE = tempPage;

	return 0;
}


void CAN_Init(unsigned char numOfNodes)
{
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


unsigned char CAN_CheckRX(unsigned char nodeID)
{
	return rxCounter[nodeID - 1];
}


char CAN_Read(unsigned char *buffer, unsigned char sendingNodeID)
{
	unsigned char i;
	PORTG = 0xff;
	// sve dok ne stigne neki podatak ostajem u funkciji
	while(!rxCounter[sendingNodeID - 1]);

	for(i = 0; i < 8; ++i)
	*(buffer + i) = (*(rxBuffers + sendingNodeID - 1) + sizeof(unsigned char) * (*(rxRdIndex + sendingNodeID - 1)))[i];

	if(++(*(rxRdIndex + sendingNodeID - 1)) == RX_BUFFER_SIZE)
	*(rxRdIndex + sendingNodeID - 1) = 0;

	rxCounter[sendingNodeID - 1]--;

	return 0;
}

char CAN_Write(unsigned char *data, unsigned char receivingNodeAddress)
{
    unsigned char i;
	// Cekam sve dok je magistrala zauzeta-> da li je neophodno?
	while(((CANGSTA >> TXBSY) & 0x01) || ((CANGSTA >> RXBSY) & 0x01));

	unsigned char ch;
	unsigned char tempPage = CANPAGE;

	// trazim slobodan MOb jel ovo sranje mene zajebava ili? sta ti treba? find all
	for(ch = 0; ch < 15; ch++)
	{
		CANPAGE = ch << 4;

		if(!(CANCDMOB >> 6))
			break;
	}

	// nema slobodnih MOb- ova
	if(ch == 15)
		return -1;


	//ova dva registra sadrze identifikator poruke koja ce se slati! efektivno to je adresa cvora koji ce primiti poruku
	CANIDT2 = (receivingNodeAddress & 0x0F) << 5;
	CANIDT1 = (receivingNodeAddress) >> 3;

	// DLC = 8
	CANCDMOB = 8;

	// upisujem 8 bajtova koje ce se slati u predajni bafer
	for(i = 0; i < 8; i++)
		CANMSG = data[i];

	// podesavam ga kao predajnik
	CANCDMOB |= (1 << CONMOB0);

	CANPAGE = tempPage;

	return 0;
}
