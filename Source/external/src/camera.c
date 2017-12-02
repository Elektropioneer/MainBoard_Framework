#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "score_display.h"
#include "ax.h"
#include "usart.h"
#include "camera.h"

/*
 * 	Function:    unsigned char camera_get_combination()
 * 	Description: Get the camera combintion (1-10)
 */
unsigned char camera_get_combination() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'c';					    // camera
	buffer[1] =  0;						    // '\0'
	buffer[2] = 'c';						// combination

	display_send(buffer);

	return UART1_Read();

}


