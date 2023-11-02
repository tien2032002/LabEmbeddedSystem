/*
 * task.h
 * this header use for define task function
 *  Created on: 30 thg 9, 2023
 *      Author: Administrator
 */

#ifndef MAIN_TASK_H_
#define MAIN_TASK_H_


#include "main.h"

//parameter for printID task

extern int my_id;
extern int frequency;

//parameter for pressPrint task

extern char* msg;

//print student ID function
void init_printID();
void printID();

//press button task
void init_pressPrint(char* message, gpio_num_t gpio_num);
void pressPrint();

#endif /* MAIN_TASK_H_ */
