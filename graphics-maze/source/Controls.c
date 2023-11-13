/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

int startHeldKeys = -1;

void initInput(){

	scanKeys();
	scanKeys();
	startHeldKeys = keysHeld() & 0xff;
	printf("init keys: %d\n", startHeldKeys);
}

void handleInput(Player* player){
	scanKeys();
	u16 keys = keysHeld();
	if(startHeldKeys != -1 && keys == startHeldKeys)
		return;
	startHeldKeys = -1;
	int round(double b){
		return (int)(b+0.5);
	}
	printf("%.2f: s%.2f, c%.2f\n",player->angle, sin(player->angle), cos(player->angle));
	int speed = 3.5;
	int strafe_speed = 2.2;
	if(keys & KEY_LEFT){
			player->x += round(strafe_speed *sin(player->angle));
			player->y -= round(strafe_speed*cos(player->angle));
		}
	if(keys & KEY_RIGHT){
		player->x -= round(strafe_speed*sin(player->angle));
		player->y += round(strafe_speed * cos(player->angle));
	}
	if(keys & KEY_UP){
		player->x += round(speed * cos(player->angle));
		player->y += round(speed * sin(player->angle));
		}
	if(keys & KEY_DOWN){
		player->x -= round(speed * cos(player->angle));
		player->y -= round(speed * sin(player->angle));
		}
	if(keys & KEY_L){
			player->angle -= 0.05;
		}
	if(keys & KEY_R){
			player->angle += 0.05;
		}



}
