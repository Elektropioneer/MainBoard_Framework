#ifndef CAN_H
#define CAN_H


//#define XBEE_TX_IDENTIFICATOR			0xA1 // bikon
//#define KINEZ_TX_IDENTIFICATOR			0XB2 // aktuatorska
#define DRIVER_TX_IDENTIFICATOR			0xC3 // odometrija
#define DRIVER_LIFT_TX_IDENTIFICATOR	0xD4 // vojina
//#define ARDUINO_TX_INDENTIFICATOR		0xA1 // arduino actuator

#define DRIVER_RX_IDENTIFICATOR 		0x01 //8
//#define XBEE_RX_IDENTIFICATOR			0x02 //11
//#define KINEZ_RX_IDENTIFICATOR			0x03 //20
#define DRIVER_LIFT_RX_IDENTIFICATOR	0x04
//#define ARDUINO_RX_INDENTIFICATOR		0x02

unsigned char CAN_CheckRX(unsigned char nodeID);
char CAN_Read(unsigned char *buffer, unsigned char sendingNodeID);
char CAN_Write(unsigned char *data, unsigned char receiveingNodeAddress);
void CAN_Init(unsigned char numOfNodes);


#endif /* CAN_H_ */
