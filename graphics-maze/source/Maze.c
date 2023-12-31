/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */
#include "Maze.h"
maze[] = {1,1,1,1,1, 1,1,1,1,1, 2,2,2,2,2, 2,2,2,2,2,
		1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,1, 0,0,0,0,2,
		1,0,0,1,0, 0,0,0,0,1, 0,0,0,0,1, 0,0,0,0,2,
		1,0,0,0,0, 0,0,1,1,1, 0,0,0,0,0, 0,0,0,0,2,
		2,0,0,0,0, 1,0,0,0,1, 0,0,0,0,2, 0,0,0,0,2,
		2,0,0,0,0, 1,0,0,0,1, 2,2,2,2,2, 0,0,0,0,2,
		1,0,0,0,0, 1,0,2,0,1, 0,0,0,0,0, 0,0,0,0,2,
		4,3,0,0,0, 1,0,0,0,1, 0,0,1,0,0, 0,0,0,2,4,
		0,0,0,2,0, 0,0,0,0,1, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,1,0, 0,0,0,0,1, 3,3,3,3,0, 3,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,1, 0,0,0,3,0, 3,0,0,0,0,
		4,3,0,0,0, 0,0,0,0,1, 0,0,0,3,0, 3,0,0,2,4,
		2,0,0,0,0, 0,0,0,0,0, 0,0,0,0,3, 3,0,0,0,1,
		1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,2,
		1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,2,
		1,0,0,2,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,2,
		2,0,0,0,0, 1,0,0,0,1, 0,0,0,0,0, 0,0,0,0,2,
		1,0,0,0,0, 1,0,0,0,1, 1,1,0,1,1, 1,0,0,0,2,
		1,0,0,0,0, 1,0,0,0,0, 0,1,0,0,0, 1,0,0,0,2,
		1,0,0,2,0, 0,0,0,0,0, 0,1,0,0,0, 1,0,0,0,2,
		1,1,1,1,1, 1,1,1,1,1, 2,2,2,2,2, 2,2,1,2,1};

Goal goal = {0, 0, 1};

u16 color_from_wall(int wall_type, bool is_x_wall){
#ifdef FB0
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
#endif
#ifdef ROTOSCALE
	return wall_type*2 - 1 + is_x_wall;
#endif
}

void Maze_Init(){
	MAZE_FOV = MAZE_FOV_MIN;
	PULLBACK = PULLBACK_MIN;

	shuffleGoal();

}

float Maze_get_raycast_distance(int px, int py, float angle, bool x_wall, int* wall_type){
	float slope = tan(angle);

	bool facing_down = sin(angle) > 0;
	bool facing_right = cos(angle) > 0;
	inline int mod(int x, int amount){
		return ((x % amount) + amount) % amount;
	}
	inline float mod_float(float x, int amount){
		return fmod(fmod(x, amount) + amount,amount);
	}

	if(x_wall){
		float float_py = py;
		//set a limit
		int i = 0;
		float distance = 0;

		for(i = 0; i < RAYCAST_RECURSION; i++){
			int last_px = px;
			px = (((px+(facing_right ? MAZE_BLOCK_SIZE : -1))>>MAZE_BLOCK_BITS))<<MAZE_BLOCK_BITS;
			float_py += slope*(px - last_px);
			distance += sqrt( (1+slope*slope)*(px-last_px)*(px-last_px) );

			px = mod(px - !facing_right, MAZE_WIDTH<<MAZE_BLOCK_BITS) + !facing_right;
			float_py = mod_float(float_py,MAZE_HEIGHT<<MAZE_BLOCK_BITS);

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
		for(i = 0; i < RAYCAST_RECURSION; i++){
			int last_py = py;
			py = ((py+(facing_down ? MAZE_BLOCK_SIZE : -1))>>MAZE_BLOCK_BITS)<<MAZE_BLOCK_BITS;
			float_px += (1/slope)*(py - last_py);
			distance += sqrt( (1+(1/slope)*(1/slope))*(py-last_py)*(py-last_py) );

			py = mod(py - !facing_down, MAZE_HEIGHT<<MAZE_BLOCK_BITS) + !facing_down;
			float_px = mod_float(float_px,MAZE_WIDTH<<MAZE_BLOCK_BITS);
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

void Render_screen(enum BUFFER_TYPE bT, Camera player, int columns){
	//FillRectangle(MAIN, 0,85,0,255, RGB15(20,25,31));
	//FillRectangle(MAIN, 86,191,0,255, RGB15(20,31,20));
	int i = 0;
	for(i = 0; i < columns; i++){
		float angle = player.angle + MAZE_FOV*(-0.5 + (i+1)/(float)(columns+1));

		int x_wall_type = 0;
		int y_wall_type = 0;
		float x_wall_distance = Maze_get_raycast_distance(player.x, player.y, angle, true, &x_wall_type);
		float y_wall_distance = Maze_get_raycast_distance(player.x, player.y, angle, false, &y_wall_type);

		float distance = x_wall_distance < y_wall_distance ? x_wall_distance : y_wall_distance;

		//int color_falloff = ((int)distance / 30) & 0x1f;
		u16 wall_color = color_from_wall(x_wall_distance < y_wall_distance ? x_wall_type : y_wall_type, x_wall_distance > y_wall_distance);
		//wall_color = RGB15((wall_color & 0x1f ) /(color_falloff+1),((wall_color & 0x1f<<5 )>>5/(color_falloff+1)), (((wall_color>>10)&0x1f)/(color_falloff+1)));
		float half_length_wall = 150/(1+(distance*cos(MAZE_FOV*(-0.5+i/(float)columns)))/MAZE_BLOCK_SIZE);
		//FillRectangle(MAIN, 0, clamp(96-(int)half_length_wall,0,192), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, 1);
		//FillRectangle(MAIN, clamp(96+(int)half_length_wall,0,192), 192, (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, 2);
		FillRectangle(bT, clamp(96-(int)half_length_wall,0,192), clamp(96+(int)half_length_wall,0,192), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
		//DrawAngledLine(bT,player.x,player.y,angle,x_wall_distance,RGB15(31,0,0));
		//DrawAngledLine(bT,player.x,player.y,angle+0.05,y_wall_distance,RGB15(0,0,31));
	}
}

int map_border = 0;
void Render_map(enum BUFFER_TYPE bT, Camera player){


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

byte getMaze(int x, int y){
	return maze[coords(x,y,MAZE_WIDTH)];
}

byte getMazeFromWorld(float px, float py){
	return getMaze(round_float(px)>>MAZE_BLOCK_BITS,round_float(py)>>MAZE_BLOCK_BITS);
}

u16 shuffleGoal(){
	maze[coords(goal.x,goal.y,MAZE_WIDTH)] = goal.old_block;
	u16 old = (goal.y<<8) + goal.x;
	goal.x = rng() % MAZE_WIDTH;
	goal.y = rng() % MAZE_HEIGHT;
	goal.old_block = getMaze(goal.x,goal.y);

	maze[coords(goal.x,goal.y,MAZE_WIDTH)] = BLOCK_GOAL;

	return old;
}


void tryGoal(float x, float y, float angle, Player* player){
	if(getMazeFromWorld(x + 32 * cos(angle),y + 32 * sin(angle)) == BLOCK_GOAL ||
			getMazeFromWorld(x + 16 * cos(angle),y + 16 * sin(angle)) == BLOCK_GOAL ||
			getMazeFromWorld(x + 8 * cos(angle),y + 8 * sin(angle)) == BLOCK_GOAL){
		shuffleGoal();
		Audio_PlaySoundEX( SFX_SWISH , 255, 128);
		printf("found one!, score %d\n",++player->score);
	}
}

