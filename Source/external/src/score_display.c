#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "i2c.h"
#include "score_display.h"

#define SCORE_DISPLAY_IDDR 0x00

int current_score = 0;

void update_score(int increment_score_by) {

	int new_score = current_score + increment_score_by;

	uint8_t _CC = new_score / 10;

	uint8_t S__ = new_score / 100;
	uint8_t _S_ = (new_score - (S__ * 100)) / 10;
	uint8_t __S = new_score - (_CC * 10);

	uint8_t data[3] = {S__, _S_, __S};

	i2c_transmit(SCORE_DISPLAY_IDDR, data, 3);

	current_score = new_score;
}
