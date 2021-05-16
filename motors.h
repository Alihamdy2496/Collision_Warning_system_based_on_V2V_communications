#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"


/*
 * motors.h
 *
 *  Created on: Feb 23, 2019
 *      Author: hp
 */

#ifndef MOTORS_H_
#define MOTORS_H_

//extern bool forward;

unsigned short Speed(char char_s);
void Direction(char input_char);
void Motors_init(void);

#endif /* MOTORS_H_ */
