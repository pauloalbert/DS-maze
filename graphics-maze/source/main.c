
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
		swap_buffers(MAIN);
			Render_screen(MAIN,player,25);
			//Render_map(MAIN,player);
			update_player();
		swiWaitForVBlank();
	}

}

