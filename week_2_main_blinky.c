/*
    FreeRTOS V9.0.0rc2 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define SIZE 10 
#define ROW SIZE 
#define COL SIZE 

typedef struct _TASK {
    int taskNum;
    const char *taskText; 
} TASK;

/* External functions */
static void matrix_task() 
{ 
	int i, j, k, l; 
	long simulationdelay; 
	double sum = 0.0; 
	double **a = (double **)pvPortMalloc( ROW * sizeof(double*) ); 
	double **b = (double **)pvPortMalloc( ROW * sizeof(double*) ); 
	double **c = (double **)pvPortMalloc( ROW * sizeof(double*) ); 
	
	for( i = 0; i < ROW; i++ ) 
		a[i] = (double *)pvPortMalloc( COL * sizeof(double) ); 
	for( i = 0; i < ROW; i++ ) 
		b[i] = (double *)pvPortMalloc( COL * sizeof(double) ); 
	for( i = 0; i < ROW; i++ ) 
		c[i] = (double *)pvPortMalloc( COL * sizeof(double) ); 
	
	for( i = 0; i < SIZE; i++ )
	{
		for( j = 0; j < SIZE; j++ ) 
		{
			a[ i ][ j ] = 1.5; 
			b[ i ][ j ] = 2.6; 
		} 
	} 
	
	while( 1 ) 
	{ 
	/* 
	* In an embedded systems, matrix multiplication would block the CPU for 
	* a long time 
	* but since this is a PC simulator we must add one additional dummy 
	* delay. 
	*/ 
		for( simulationdelay = 0; simulationdelay<1000000000; simulationdelay++ ); 
		for( i = 0; i < SIZE; i++ ) 
		{
			for( j = 0; j < SIZE; j++ )  
			{
				c[ i ][ j ] = 0.0; 
			}
		} 
	} 
	
	for( i = 0; i < SIZE; i++ ) 
	{ 
		for( j = 0; j < SIZE; j++ ) 
		{ 
			sum = 0.0; 
			for( k = 0; k < SIZE; k++ ) 
			{ 
				for( l = 0; l<10; l++ ) 
				{ 
					sum = sum + a[ i ][ k ] * b[ k ][ j ]; 
				} 
			} 
			c[i][j] = sum; 
		} 
	} 
	
	vTaskDelay(100); 
} 

static void communication_task() 
{ 
	while( 1 ) 
	{ 
		printf( "Sending data...\n" ); 
		fflush( stdout ); 
		vTaskDelay( 100 ); 
		printf( "Data sent!\n" ); 
		fflush( stdout ); 
		vTaskDelay( 100 ); 
	} 
} 

xTaskHandle matrix_handle;
xTaskHandle communication_handle;

int main_blinky(void) {
	xTaskCreate( (pdTASK_CODE)matrix_task, (const char *)"Matrix", 1000, NULL, 3, &matrix_handle );
	xTaskCreate( (pdTASK_CODE)communication_task, (const char *)"Communication",configMINIMAL_STACK_SIZE, NULL, 4, &communication_handle );
    
    /* Start the scheduler so our tasks start executing. */
    vTaskStartScheduler();

    /* If all is well then main() will never reach here as the scheduler will
    now be running the tasks. If main() does reach here then it is likely that
    there was insufficient heap memory available for the idle task to be created.
    CHAPTER 5 provides more information on memory management. */
    for( ;; );
}
