/*
 * sides.h
 *
 *  Created on: Jul 11, 2017
 *      Author: silard_g
 */

#ifndef INC_SIDES_H_
#define INC_SIDES_H_

#include "odometry.h"

typedef struct
{
	struct odometry_position point;
	uint8_t					 speed;
	uint8_t					 direction;
	char					 (*callback)(uint32_t start_time);
} gotoFields;


void darkside(void);

#endif /* INC_SIDES_H_ */
