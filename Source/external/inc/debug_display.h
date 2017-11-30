/*
 * debug_display.h
 *
 *  Created on: Nov 30, 2017
 *      Author: silardg
 */

#ifndef SOURCE_EXTERNAL_INC_DEBUG_DISPLAY_H_
#define SOURCE_EXTERNAL_INC_DEBUG_DISPLAY_H_

unsigned char debug_ready();
unsigned char debug_setup_wait();
unsigned char debug_position(int16_t x, int16_t y);

#endif /* SOURCE_EXTERNAL_INC_DEBUG_DISPLAY_H_ */
