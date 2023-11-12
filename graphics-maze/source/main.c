
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
struct Player player= {60,140,1.5};
void update_player(){
	player.angle += 0.1;
}
int main(void)
{
	consoleDemoInit();
	Maze_Init();
	P_Graphics_setup_main();





	while(1) {
		//printf("allo");
		//FillScreen(MAIN,0x1);
		//FillRectangle(MAIN,30,100,30,100,2);
		swap_buffers(MAIN);
		Render_screen(MAIN,player,128);
			//Render_map(MAIN,player);
		update_player();
		swiWaitForVBlank();
	}

}

