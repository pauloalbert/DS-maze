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

	int width = 4;
	while(1) {

		/*************
		 * Exercise 1
		 *************/
		//Scan the keys
		scanKeys();
		u16 keys = keysHeld();

		if(keys & KEY_R)
			width = 9;
		else
			width = 3;

		if(keys & KEY_TOUCH){
			touchPosition touch;
			touchRead(&touch);

			int i;
			int j;
			if(last_touch.px == 999){
				for(i = -(width-1)/2; i <= width/2; i++)
					for(j = -(width-1)/2; j <= width/2; j++)
							FillRectangle(MAIN,touch.py +j,touch.py+j,touch.px+i,touch.px+i, !(keys & KEY_L));
				printf("NEW\n");
			}
			else{

				for(i = -(width-1)/2; i <= width/2; i++)
					for(j = -(width-1)/2; j <= width/2; j++)
						DrawLine(MAIN,last_touch.px+i,last_touch.py+j,touch.px+i,touch.py+j, !(keys & KEY_L));
			}
			last_touch.px = touch.px;
			last_touch.py = touch.py;
		}
		else{
			last_touch.px = 999;
		}

		//swap_buffers(MAIN);
		swiWaitForVBlank();

	}
}
