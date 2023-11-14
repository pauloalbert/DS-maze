/*
 * Constants.h
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#pragma once
/* VISUALS */
#define MAZE_FOV_MIN 0.9
#define MAZE_FOV_MAX 1.4
float MAZE_FOV;

#define PULLBACK_MIN 3
#define PULLBACK_MAX 64
float PULLBACK;

#define RAYCAST_RECURSION 20

/* MAZE */
#define MAZE_BLOCK_BITS 5
#define MAZE_BLOCK_SIZE 32//(2<<MAZE_BLOCK_BITS);
#define MAZE_HEIGHT 20
#define MAZE_WIDTH 20
extern int maze[];

/* MOVEMENT */
#define TURN_MAX 0.15
#define TURN_MIN 0.05
#define TURN_GAIN 0.005

#define SPEED_FORWARD 3.5
#define SPEED_STRAFE 2.2
