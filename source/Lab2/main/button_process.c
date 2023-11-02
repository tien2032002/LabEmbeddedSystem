/*
 * button_process.c
 *
 *  Created on: 30 thg 9, 2023
 *      Author: Administrator
 */


#include "button_process.h"

int flagForButtonPress;
int flagForButtonLongPress;
int flagForButtonDoublePress;

int debounceButtonBuffer1, debounceButtonBuffer2, debounceButtonBuffer3, buttonBuffer;
int counterForButtonLongPress;

void reset_button_buffer(){
	debounceButtonBuffer1 = RELEASE;
	debounceButtonBuffer2 = RELEASE;
	debounceButtonBuffer3 = RELEASE;
	buttonBuffer = RELEASE;

	flagForButtonPress = 0;
	flagForButtonLongPress = 0;
	flagForButtonDoublePress = 0;
	counterForButtonLongPress = LONG_PRESS_DURATION;
}


void button_reading (){
	TickType_t xFrequency = 10 / portTICK_PERIOD_MS;
	TickType_t xLastWakeTime;
	BaseType_t xWasDelayed;

	xLastWakeTime = xTaskGetTickCount ();
	xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

	for( ;; )
		{
			// Wait for the next cycle.
			xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

			debounceButtonBuffer3 = debounceButtonBuffer2;
			debounceButtonBuffer2 = debounceButtonBuffer1;
			debounceButtonBuffer1 = gpio_get_level(BUTTON);
//			printf("debounceButtonBuffer3: %d\n", debounceButtonBuffer3);
//			printf("debounceButtonBuffer2: %d\n", debounceButtonBuffer2);
//			printf("debounceButtonBuffer1: %d\n", debounceButtonBuffer1);


			if ((debounceButtonBuffer1 == debounceButtonBuffer2) &&
				(debounceButtonBuffer2 == debounceButtonBuffer3)) {
				if (debounceButtonBuffer3 != buttonBuffer) {
					buttonBuffer = debounceButtonBuffer3;
					if (buttonBuffer == PRESS) {
						flagForButtonPress = 1;
						counterForButtonLongPress = LONG_PRESS_DURATION/10;

					}
				}

				else {
					if (buttonBuffer == PRESS) {
						counterForButtonLongPress--;
						if (counterForButtonLongPress < 0){
							flagForButtonLongPress = 1;
							counterForButtonLongPress = LONG_PRESS_DURATION/10;
						}
					}
				}
			}
		}

}
