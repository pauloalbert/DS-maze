#include <nds.h>
#include <stdio.h>


#include "P_Modes.h"
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	P_Graphics_setup_main();

	scanKeys();
	consoleDemoInit();
	//const char* names[] = {"A","B","SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN", "R", "L", "X", "Y"};
	//The main infinite loop
	while(1) {
		scanKeys();
		ModeLoop();
		//swap_buffers(MAIN);
		swiWaitForVBlank();

	}
}
