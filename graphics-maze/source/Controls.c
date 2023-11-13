/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"

int startHeldKeys = -1;

void initInput(){
	scanKeys();
	startHeldKeys = keysHeld();
	printf("init keys: %d\n", startHeldKeys);
}

void handleInput(Player* player){
	scanKeys();
	u16 keys = keysHeld();
	if(startHeldKeys != -1 && keys == startHeldKeys)
		return;
	startHeldKeys = -1;

	if(keys & KEY_LEFT){
		player->x -= 1;
	}
	if(keys & KEY_RIGHT){
			player->x += 1;
	}
	if(keys & KEY_UP){
			player->y += 1;
		}
	if(keys & KEY_DOWN){
			player->x -= 1;
		}
	if(keys & KEY_L){
			player->angle += 0.05;
		}
	if(keys & KEY_R){
			player->angle -= 0.05;
		}

}
