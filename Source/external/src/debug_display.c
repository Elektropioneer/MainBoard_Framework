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

