
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
#include "Controls.h"
Player camera= {60,140,0};

int main(void)
{
	consoleDemoInit();
	Maze_Init();
	initInput();
	P_Graphics_setup_main();




	while(1) {
		//printf("allo");
		//FillScreen(MAIN,0x1);
		//FillRectangle(MAIN,30,100,30,100,2);
		swap_buffers(MAIN);
		Render_screen(MAIN,camera,32);
			//Render_map(MAIN,player);
		handleInput(&camera);
		swiWaitForVBlank();
	}

}

