#include "P_Gameplay.h"


void Give_Hint(const Player* player,const Goal* goal){
	float dx = (goal->x<<MAZE_BLOCK_BITS) - player->x;
	float dy = (goal->y<<MAZE_BLOCK_BITS) - player->y;

	float angle = atan2(dy,dx) - player->angle;

	Audio_PlaySoundEX( SFX_LASER , 126, (u8)(125 * sin(angle) + 128));
}
