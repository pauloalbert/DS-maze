#include <nds.h>
#include <stdio.h>

#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	P_Graphics_setup_main();

	scanKeys();
	consoleDemoInit();
	//const char* names[] = {"A","B","SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN", "R", "L", "X", "Y"};


	touchPosition last_touch = {0,0,0,0,0,0};
	last_touch.px = 999;
	//The main infinite loop

	while(1) {

		swap_buffers(MAIN);
		/*************
		 * Exercise 1
		 *************/
		//Scan the keys
		scanKeys();
		u16 keys = keysHeld();
		int i;
			for(i=50; i < 200; i++){
				DrawPixel(get_buffer_pointer(MAIN),256,i,30,1);
			}
		if(keys & KEY_TOUCH){
			touchPosition touch;
			touchRead(&touch);
			if(last_touch.px == 999){
				FillRectangle(MAIN,touch.py,touch.py,touch.px,touch.px,1);
				printf("NEW\n");
			}
			else{

				//printf("LINE (%d,%d:%d,%d)\n",last_touch.px,last_touch.py,touch.px,touch.py);
				DrawLine(MAIN,last_touch.px,last_touch.py,touch.px,touch.py,1);
			}
			last_touch.px = touch.px;
			last_touch.py = touch.py;
		}
		else{
			last_touch.px = 999;
		}

		swiWaitForVBlank();

	}
}
