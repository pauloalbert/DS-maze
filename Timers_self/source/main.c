
#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "chrono_display.h"


//Exercises 2 and 3
int min, sec, msec;
int reset_msec;
int color;
int state = 0; //0 - playing, 1 - hit, 2 - lost
void reset(){
	printf("reset\n");
	changeColorDisp_Main(GREEN);
	min = 0, sec = 10, msec = 0;
	reset_msec = 0;
	state = 0;
}

void fail(){
	printf("fails\n");
	state = 2;
	changeColorDisp_Main(RED);
	sec = 58;
	msec = 888;
	min = 58;
}

int main(void) {

	consoleDemoInit();

	//Initialize the display and the color of the display
	initChronoDisp_Main();
	changeColorDisp_Main(GREEN);

	//Initialize the interrupts correctly
	irqInit();

	void timer_irq_tick(){
		switch (state){
		case 0:
			msec-= 1;
			if(msec > 999) {msec = 0; sec += 1;}
			if(sec > 59) {sec = 0; min += 1;}
			if(msec < 0) {msec = 999; sec -= 1;}
			if(sec < 0) {fail();}
			break;
		case 1:
		case 2:
			reset_msec += 1;
			if(reset_msec > 999){
				reset();
			}
		}
	}

	void vblank_irq_refresh(){
		updateChronoDisp_Main(min,sec,msec);
	}

	void keys_a_irq(){
		if(state != 0) return;
		state = 1;
		printf("\nstopped at %d:%d.%d\n", min, sec, msec);
	}

	//Set the minutes, the seconds and the mseconds
	min = sec = msec = 0;
	reset_msec = 0;
	sec = 10;
	color = 0;
	//Configure the timer (i.e. TIMER0) correctly
	TIMER_DATA(0) = TIMER_FREQ_64(1000);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
	
	TIMER_DATA(1) = TIMER_FREQ_1024(1);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;

	//Associate the interrupt handler and enable the interrupt line
	irqSet(IRQ_TIMER0, &timer_irq_tick);
	irqSet(IRQ_VBLANK, &vblank_irq_refresh);
	irqSet(IRQ_KEYS, &keys_a_irq);

	irqEnable(IRQ_TIMER0);
	irqEnable(IRQ_VBLANK);
	irqEnable(IRQ_KEYS);

	//Configure the interruption to be fired when button A is pressed
	REG_KEYCNT = (1<<14)| KEY_A;
	reset();
	while(1) {
		swiWaitForVBlank();
	}
}
