/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

#define TURN_MAX 0.09
#define TURN_MIN 0.04
#define TURN_GAIN 0.015

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
	int round(double b){
		return (int)(b+0.5);
	}
	printf("%.2f: s%.2f, c%.2f\n",camera->angle, sin(camera->angle), cos(camera->angle));
	int speed = 3.5;
	int strafe_speed = 2.2;

	float x_vec;
	float y_vec;
	if(keys & KEY_LEFT){
		x_vec += strafe_speed *sin(camera->angle);
		y_vec -= strafe_speed*cos(camera->angle);
	}
	if(keys & KEY_RIGHT){
		x_vec -= strafe_speed*sin(camera->angle);
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

	if(true){
		player->x += x_vec;
		player->y += y_vec;
	}

	if(keys & KEY_L){
			player->torque = clamp_float(player->torque - TURN_GAIN, -TURN_MAX, -TURN_MIN);
			player->angle += player->torque;
		}
	if(keys & KEY_R){

			player->torque = clamp_float(player->torque + TURN_GAIN, TURN_MIN, TURN_MAX);
			player->angle += player->torque;
		}

	//set camera based on player:
	camera->angle = player->angle;
	camera->x = round(player->x);
	camera->y = round(player->y);


}
