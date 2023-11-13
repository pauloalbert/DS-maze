/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

#define TURN_MAX 0.15
#define TURN_MIN 0.05
#define TURN_GAIN 0.005

int startHeldKeys = -1;

void initInput(){

	scanKeys();
	scanKeys();
	startHeldKeys = keysHeld() & 0xff;
	printf("init keys: %d\n", startHeldKeys);
}

void handleInput(Camera* camera, Player* player){
	scanKeys();
	u16 keys = keysHeld();
	if(startHeldKeys != -1 && keys == startHeldKeys)
		return;
	startHeldKeys = -1;
	inline int round(double b){
		return (int)(b+0.5);
	}

	if(keys & KEY_L){
			player->torque = clamp_float(player->torque - TURN_GAIN, -TURN_MAX, -TURN_MIN);
			player->angle += player->torque;
	}
		if(keys & KEY_R){

		player->torque = clamp_float(player->torque + TURN_GAIN, TURN_MIN, TURN_MAX);
		player->angle += player->torque;
	}

	int speed = 3.5;
	int strafe_speed = 2.2;

	float x_vec;
	float y_vec;
	if(keys & KEY_LEFT){
		x_vec += strafe_speed * sin(camera->angle);
		y_vec -= strafe_speed * cos(camera->angle);
	}
	if(keys & KEY_RIGHT){
		x_vec -= strafe_speed * sin(camera->angle);
		y_vec += strafe_speed * cos(camera->angle);
	}
	if(keys & KEY_UP){
		x_vec += speed * cos(camera->angle);
		y_vec += speed * sin(camera->angle);
		}
	if(keys & KEY_DOWN){
		x_vec -= speed * cos(camera->angle);
		y_vec -= speed * sin(camera->angle);
		}

	x_vec += player->x_vel;
	y_vec += player->y_vel;
	if(!getMaze(round(player->x+x_vec)>>5,round(player->y+y_vec)>>5)){
		player->x += x_vec;
		player->y += y_vec;
	}
	else{
		player->x_vel = 0;
		player->y_vel = 0;
	}

	x_vec -= player->x_vel;
	y_vec -= player->y_vel;

	player->x_vel += x_vec/4;
	player->y_vel += y_vec/4;
	if(x_vec == 0)
		player->x_vel /= 1.5;

	if(y_vec == 0)
		player->y_vel /= 1.5;

	if(keys & KEY_A){
		MAZE_FOV = 1.4;
	}
	else{
		MAZE_FOV = 0.9;
	}
	//set camera based on player:
	camera->angle = player->angle;
	camera->x = round(player->x);
	camera->y = round(player->y);


}
