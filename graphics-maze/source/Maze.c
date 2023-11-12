/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */
#include "Maze.h"
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include <math.h>
const int MAZE_BLOCK_BITS = 5;
const int MAZE_BLOCK_SIZE = 32;//(2<<MAZE_BLOCK_BITS);
const int MAZE_HEIGHT = 10;
const int MAZE_WIDTH = 10;
int maze[] = {1,1,1,1, 1,1,2,1,1,1,
		1,0,0,0, 0,0,0,0,0,1,
		1,0,0,1, 0,0,0,0,0,1,
		1,0,0,0, 0,0,0,0,0,1,
		2,0,0,0, 0,1,0,0,0,1,
		2,0,0,0, 0,1,0,0,0,1,
		1,0,0,0, 0,1,0,0,0,1,
		1,0,0,0, 0,1,0,0,0,1,
		1,0,0,2, 0,0,0,0,0,1,
		1,1,2,1, 1,1,1,1,1,1};

u16 color_from_wall(int wall_type, bool is_x_wall){
	switch(wall_type){
	case 1:
		if (is_x_wall) return RGB15(15,0,15);
		return RGB15(25,10,25);
	case 2:
		if (is_x_wall) return RGB15(15,5,5);
		return RGB15(25,0,0);
	default:
		return RGB15(0,0,0);
	}
}

void Maze_Init(){
	MAZE_FOV = 0.5;
}

float Maze_get_raycast_distance(int px, int py, float angle, bool x_wall, int* wall_type){
	float slope = tan(angle);

	bool facing_down = sin(angle) > 0;
	bool facing_right = cos(angle) > 0;


	if(x_wall){
		float float_py = py;
		//set a limit
		int i = 0;
		float distance = 0;

		for(i = 0; i < 10; i++){
			int last_px = px;
			px = (((px+(facing_right ? MAZE_BLOCK_SIZE : -1))>>MAZE_BLOCK_BITS))<<MAZE_BLOCK_BITS;
			float_py += slope*(px - last_px);
			distance += sqrt( (1+slope*slope)*(px-last_px)*(px-last_px) );

			if(px < 0 || (int)float_py < 0 || px>>MAZE_BLOCK_BITS > MAZE_WIDTH || (int)float_py>>MAZE_BLOCK_BITS > MAZE_HEIGHT) return 1000000;
			int current_wall = maze[coords((((px)>>MAZE_BLOCK_BITS) - !facing_right),((int)float_py)>>MAZE_BLOCK_BITS,MAZE_WIDTH)];
			if(current_wall != 0){
				if(wall_type) *(wall_type) = current_wall;
				return distance;
			}
		}
		return 1000000;
	}
	else{
		float float_px = px;
		//set a limit
		int i = 0;
		float distance = 0;
		for(i = 0; i < 10; i++){
			int last_py = py;
			py = ((py+(facing_down ? MAZE_BLOCK_SIZE : -1))>>MAZE_BLOCK_BITS)<<MAZE_BLOCK_BITS;
			float_px += (1/slope)*(py - last_py);
			distance += sqrt( (1+(1/slope)*(1/slope))*(py-last_py)*(py-last_py) );
			if(py < 0 || (int)float_px < 0 || py>>MAZE_BLOCK_BITS > MAZE_WIDTH || (int)float_px>>MAZE_BLOCK_BITS > MAZE_HEIGHT) return 1000000;
			int current_wall = maze[coords(((int)(float_px)>>MAZE_BLOCK_BITS),(py>>MAZE_BLOCK_BITS)-!facing_down,MAZE_WIDTH)];
			if(current_wall != 0){
				if(wall_type) *(wall_type) = current_wall;
				return distance;
			}
		}
		return 1000000;

	}

}

void Render_screen(enum BUFFER_TYPE bT, struct Player player, int columns){
	//FillRectangle(MAIN, 0,85,0,255, RGB15(20,25,31));
	//FillRectangle(MAIN, 86,191,0,255, RGB15(20,31,20));
	int i = 0;
	for(i = 0; i < columns; i++){
		float angle = player.angle - MAZE_FOV*(-0.5 + (i+1)/(float)(columns+1));

		int x_wall_type = 0;
		int y_wall_type = 0;
		float x_wall_distance = Maze_get_raycast_distance(player.x, player.y, angle, true, &x_wall_type);
		float y_wall_distance = Maze_get_raycast_distance(player.x, player.y, angle, false, &y_wall_type);

		float distance = x_wall_distance < y_wall_distance ? x_wall_distance : y_wall_distance;

		//int color_falloff = ((int)distance / 30) & 0x1f;
		u16 wall_color = color_from_wall(x_wall_distance < y_wall_distance ? x_wall_type : y_wall_type, x_wall_distance > y_wall_distance);
		//wall_color = RGB15((wall_color & 0x1f ) /(color_falloff+1),((wall_color & 0x1f<<5 )>>5/(color_falloff+1)), (((wall_color>>10)&0x1f)/(color_falloff+1)));
		float half_length_wall = 150/(1+(distance*cos(MAZE_FOV*(-0.5+i/(float)columns)))/MAZE_BLOCK_SIZE);
		FillRectangle(MAIN, 0, clamp(96-(int)half_length_wall,0,192), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, 1);
		FillRectangle(MAIN, clamp(96+(int)half_length_wall,0,192), 192, (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, 2);
		FillRectangle(bT, clamp(96-(int)half_length_wall,0,192), clamp(96+(int)half_length_wall,0,192), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1,3);
		//DrawAngledLine(bT,player.x,player.y,angle,x_wall_distance,RGB15(31,0,0));
		//DrawAngledLine(bT,player.x,player.y,angle+0.05,y_wall_distance,RGB15(0,0,31));
	}
}

int map_border = 0;
void Render_map(enum BUFFER_TYPE bT, struct Player player){


	int i = 0;
	for(i = 0; i < MAZE_WIDTH; i++){
		int j;
		for(j=0; j < MAZE_HEIGHT; j++){

			if(maze[coords(i,j,MAZE_WIDTH)]) FillRectangle(bT,16*j + map_border,16*(j+1)-1 - map_border,16*i + map_border,16*(i+1)-1- map_border,RGB15(10,10,10));
			else FillRectangle(bT,16*j,16*(j+1)-1,16*i,16*(i+1)-1,RGB15(0,0,0));
		}
	}
	DrawCircle(bT,player.x,player.y,5,RGB15(31,31,31));
	DrawCircle(bT,player.x,player.y,5.5,RGB15(31,31,31));

	DrawAngledLine(bT,player.x,player.y,player.angle,10,RGB15(31,0,0));
}
