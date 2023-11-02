/*
 * task.c
 *
 *  Created on: 30 thg 9, 2023
 *      Author: Administrator
 */

#include "task.h"
#include "main.h"
#include "button_process.h"

int my_id;
int frequency;
char* msg;

void init_printID(int id, unsigned int freq){
	my_id = id;
	frequency = freq;
}

void printID() {
	TickType_t xFrequency = frequency / portTICK_PERIOD_MS;
	TickType_t xLastWakeTime;
	BaseType_t xWasDelayed;

	xLastWakeTime = xTaskGetTickCount ();
	xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

	for( ;; )
	    {
	        // Wait for the next cycle.
	        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

	        printf("%d\n", my_id);
	    }
}

void init_pressPrint(char* message, gpio_num_t gpio_num){
	msg = message;
	gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
	gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);

}
void pressPrint(){
	TickType_t xFrequency = 10 / portTICK_PERIOD_MS;
	TickType_t xLastWakeTime;
	BaseType_t xWasDelayed;

	xLastWakeTime = xTaskGetTickCount ();
	xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

	for( ;; )
	    {
	        // Wait for the next cycle.
	        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

	        if (flagForButtonPress){
	        	printf("%s\n", msg);
	        	flagForButtonPress = 0;
	        }

	        if (flagForButtonLongPress) {
	        	printf("long press\n");
	        	flagForButtonLongPress = 0;
	        }
	    }
}

