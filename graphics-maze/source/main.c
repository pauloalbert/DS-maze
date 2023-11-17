
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
#include "Controls.h"
#include "P_Audio.h"
#include "P_Gameplay.h"
Camera camera= {60,140,0};
Player player = {60,140,0,0,0,0,0,10};
extern Goal goal;
int main(void)
{
	consoleDemoInit();
	Maze_Init();
	initInput();
	P_Graphics_setup_main();
	Audio_Init();

	u8 i = 0;
	u8 j = 0;
	while(1) {
		swap_buffers(MAIN);
		Render_screen(MAIN,camera,32);
		if(++i % (20-j) == 0){
			float dx = (goal.x<<MAZE_BLOCK_BITS) - player.x;
			float dy = (goal.y<<MAZE_BLOCK_BITS) - player.y;

			float angle = atan2(dy,dx) - player.angle;
			float distance = (dx*dx + dy*dy);

			u8 volume = (u8)clamp(255-(distance/1000),20,255);
			j = volume>>4;
			if(cos(angle) < 0.5) volume >> 1;
			if(cos(angle) < 0) volume >> 1;
			Audio_PlaySoundEX( SFX_LASER , volume, (u8)(125 * sin(angle) + 128));

			i = 0;
		}
		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
