
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
#include "Controls.h"
Player player= {60,140,1.5};

int main(void)
{
	consoleDemoInit();
	Maze_Init();
	initInput();
	P_Graphics_setup_main();
	int i = 0;
	for(i = 0; i < 4; i++){
		printf("for %d:\ngfx: %p\nmap: %p\nprio %d\n\n",i, bgGetGfxPtr(i), bgGetMapPtr(i), bgGetPriority(i));
	}




	while(1) {
		//printf("allo");
		//FillScreen(MAIN,0x1);
		//FillRectangle(MAIN,30,100,30,100,2);
		swap_buffers(MAIN);
		Render_screen(MAIN,player,32);
			//Render_map(MAIN,player);
		handleInput(&player);
		swiWaitForVBlank();
	}

}

