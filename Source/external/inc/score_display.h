#ifndef SOURCE_EXTERNAL_INC_SCORE_DISPLAY_H_
#define SOURCE_EXTERNAL_INC_SCORE_DISPLAY_H_

#define DISPLAY_BOARD_SUCCESS		1
#define DISPLAY_BOARD_ERROR			0

void score_display_init();
unsigned char update_score(int update_by);
unsigned char clear_score();
unsigned char flash_score();

unsigned char display_send(unsigned char buffer[]);

#endif /* SOURCE_EXTERNAL_INC_SCORE_DISPLAY_H_ */
