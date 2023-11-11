
#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "chrono_display.h"


//Exercises 2 and 3
int min, sec, msec;
int color;


int main(void) {

	consoleDemoInit();
/*
	//Exercise 1

	u16 start, end;
	int result;

	// Initialize the timer with the correct configuration
	TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_1 ;
	TIMER_CR(1) = TIMER_ENABLE | TIMER_DIV_64 ;
	// Set the TIMER_DATA to 0 (the timer starts to count in 0)
	TIMER_DATA(0) = 0;
	TIMER_DATA(1) = 0;
	//Read the value of the timer
	start = TIMER_DATA(0);
	u16 start2 = TIMER_DATA(1);
	result = floor(sqrt((double)start));
	//Read the value of the timer again
	end = TIMER_DATA(0);
	u16 end2 = TIMER_DATA(1);
	//Print the solution
	//double elapsed_us = 1*((1./33.514) * (end - start));
	printf("est duration: %f, %f\n" , (1./33.514) *(end - start), (1./33.514) *64*(end2-start2));

	/*Do the same fot the hand-written square root
	start = ...
	result = iSqrt(start);
	end = ...
	printf(...
*/
/*
	//Exercise 2

	//Initialization of the display
	initChronoDisp_Main();

	irqInit();
	//Initialize the interrupts correctly
	void timer_second_IRQ(){
		sec += 1;
		if(sec >59) {sec = 0; min += 1;}
		updateChronoDisp_Main(min,sec,msec);
	}
	void timer_ms_IRQ(){
		msec += 1;
		if (msec > 999) msec = 0;
		updateChronoDisp_Main(min,sec,msec);
	}
	//Set the seconds and mseconds to 0
	sec = msec = 0;

	//Configure two timers (i.e.: TIMER0 and TIMER1) correcly
	TIMER_DATA(0) = TIMER_FREQ_1024(1);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(1) = TIMER_FREQ(1000);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1 | TIMER_IRQ_REQ;
	
	//Associate the interrupt handlers
	irqSet(IRQ_TIMER0, &timer_second_IRQ);
	irqSet(IRQ_TIMER1, &timer_ms_IRQ);
	//Enable the interrupt lines of the timers
	irqEnable(IRQ_TIMER0);
	irqEnable(IRQ_TIMER1);
*/


	//Exercise 3 (Comment exercise 1 and 2)

	//Initialize the display and the color of the display
	initChronoDisp_Main();
	changeColorDisp_Main(RED);

	//Initialize the interrupts correctly
	irqInit();

	void timer_irq(){
		msec+= 1;
		if(msec > 999) {msec = 0; sec += 1;}
		if(sec > 59) {sec = 0; min += 1;}
	}

	void refresh_irq(){
		updateChronoDisp_Main(min,sec,msec);
	}

	void color_irq(){
		color = (1+color)%4;
		switch(color){
		case 0:
			changeColorDisp_Main(RED); break;
		case 1:
			changeColorDisp_Main(GREEN); break;
		case 2:
			changeColorDisp_Main(BLUE); break;
		default:
			changeColorDisp_Main(YELLOW);
		}
	}

	void keys_irq(){
		printf("lap is %d:%d.%d\n", min, sec, msec);
	}

	//Set the minutes, the seconds and the mseconds
	min = sec = msec = 0;
	color = 0;
	//Configure the timer (i.e. TIMER0) correctly
	TIMER_DATA(0) = TIMER_FREQ_64(1000);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
	
	TIMER_DATA(1) = TIMER_FREQ_1024(1);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;

	//Associate the interrupt handler and enable the interrupt line
	irqSet(IRQ_TIMER0, &timer_irq);
	irqSet(IRQ_VBLANK, &refresh_irq);
	irqSet(IRQ_TIMER1, &color_irq);
	irqSet(IRQ_KEYS, &keys_irq);
	REG_KEYCNT = (1<<14)| KEY_A;
	irqEnable(IRQ_TIMER0);
	irqEnable(IRQ_VBLANK);
	irqEnable(IRQ_TIMER1);
	irqEnable(IRQ_KEYS);


/*
	//Exercise 6

	//Configure the interruption to be fired when button A is pressed

	//Associate the handler and enable the line of interrupt
	...
	...
*/


	while(1) {
		swiWaitForVBlank();
	}
}
