#pragma once
#include <stdio.h>
#include <nds.h>
#include "P_Graphics.h"

//         north (-y)
//	(0,0) (1,0) (2,0)
//  (0,1) (1,1) (2,1)   east(+x)
//  (0,2) (1,2) (2,2)
//         south (+y)
//player lives in 256 by 256 grid, but each block is 16*16

struct Player{
	int x;
	int y;
	float angle;
};

extern int maze[];
void Maze_Init();
float MAZE_FOV; //radian

float Maze_get_raycast_distance(int x, int y, float angle, bool x_wall, int* wall_type);

void Render_screen(enum BUFFER_TYPE bT, struct Player player, int columns);

void Render_map(enum BUFFER_TYPE bT, struct Player player);
