#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "score_display.h"
#include "debug_display.h"
#include "ax.h"

unsigned char debug_ready() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';
	buffer[1] = 'd';
	buffer[2] = 'r';

	return display_send(buffer);
}

unsigned char debug_setup_wait() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';
	buffer[1] = 'd';
	buffer[2] = 'w';

	return display_send(buffer);
}

unsigned char debug_position(int16_t x, int16_t y) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';
	buffer[1] = 'd';
	buffer[2] = 'p';
	buffer[3] = x >> 8 ;
	buffer[4] = x & 0x00FF;
	buffer[5] = y >> 8 ;
	buffer[6] = y & 0x00FF;

	return display_send(buffer);
}


