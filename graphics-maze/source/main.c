
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
#include "Controls.h"
Camera camera= {60,140,0};
Player player = {60,140,0,0,0,0,0,10};
int main(void)
{
	consoleDemoInit();
	Maze_Init();
	initInput();
	P_Graphics_setup_main();


	while(1) {
		swap_buffers(MAIN);
		Render_screen(MAIN,camera,32);

		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
