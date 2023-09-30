#include "main.h"
#include "task.h"
#include "button_process.h"

void app_main(void)
{
	//you can change student id and frequency by adjust parameter of init_printID
	init_printID(2014726, 1000);
	xTaskCreate (printID , "Cyclic_Task" , 2048 , NULL , 1 , NULL ) ;

	//you can change message and frequency gpio input
	init_pressPrint("ESP32", BUTTON);
	xTaskCreate (pressPrint , "Acyclic_Task" , 2048 , NULL , 1 , NULL ) ;

	//task for reading button
	xTaskCreate (button_reading , "ButtonReading_task" , 2048 , NULL , 1 , NULL ) ;
}
