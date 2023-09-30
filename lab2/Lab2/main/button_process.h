/*
 * button_process.h
 *
 *  Created on: 30 thg 9, 2023
 *      Author: Administrator
 */

#ifndef MAIN_BUTTON_PROCESS_H_
#define MAIN_BUTTON_PROCESS_H_

#include "main.h"

void button_reading ( void );
void reset_button_buffer();

extern int flagForButtonPress;
extern int flagForButtonLongPress;
extern int flagForButtonDoublePress;

#define PRESS 0
#define RELEASE 1

#define LONG_PRESS_DURATION 3000

#endif /* MAIN_BUTTON_PROCESS_H_ */
