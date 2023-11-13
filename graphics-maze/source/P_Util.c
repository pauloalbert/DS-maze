/*
 * util.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */

inline float clamp_float(float value, float min, float max){
	return value < min ? min : (value > max ? max : value);
}

inline int clamp(int value, int min, int max){
	return value < min ? min : (value > max ? max : value);
}
inline int coords(int x, int y, int w){
	return w*y + x;
}

inline int sign(int x){
	if(x > 0)
		return 1;
	else if(x < 0)
		return -1;
	return 0;
}

inline int round_float(float b){
	return (int)(b+0.5);
}
