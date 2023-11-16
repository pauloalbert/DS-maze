/*
 * P_Modes.c
 *
 *  Created on: Nov 16, 2023
 *      Author: nds
 */
#include "P_Modes.h"
int draw_width = 3;
u8 color_blue = 0;
u8 color_green = 0;
u8 color_red = 0;
u8 color_chosen = 1;
u8 draw_color = 1;

touchPosition swipe = {0,0,0,0,0,0};
touchPosition last_touch = {0,0,0,0,0,0};
touchPosition touch = {0,0,0,0,0,0};
u16 _keys_held;
u16 _keys_pressed;
bool _touch_valid = false;
bool _last_touch_valid = false;
bool _swipe_valid = false;

typedef enum MODE {
	MODE_DRAWING,
	MODE_COLORS,
	MODE_PAUSED
} MODE;
MODE mode = MODE_DRAWING;

void _HandleControls();
bool _HandleSwaps();
void ModeLoop(){
	/* CONTROLS */
	_HandleControls();

	bool mode_changed = _HandleSwaps();
	if(mode_changed){
		_touch_valid = false;
		_last_touch_valid = false;
		_swipe_valid = false;
	}

	switch(mode){
	case MODE_DRAWING:
		DrawMode();
		break;
	case MODE_COLORS:
		ColorMode();
		break;
	case MODE_PAUSED:
		break;
	}
}

void _HandleControls(){
	//update last touch
	last_touch.px = touch.px;
	last_touch.py = touch.py;
	_last_touch_valid = _touch_valid;

	//update last buttons
	_keys_held = keysHeld();
	_keys_pressed = keysDown();

	//update touch readings
	if(_keys_held & KEY_TOUCH){
		touchRead(&touch);
		_touch_valid = true;
		_swipe_valid = true;
	}
	else {
		_touch_valid = false;
	}

	//update swipe
	if(_touch_valid && !_last_touch_valid){
		swipe.px = touch.px;
		swipe.py = touch.py;
		swipe.rawx = 0; swipe.rawy = 0;
		swipe.z1 = 0;
		swipe.z2 = 0;
	}
	if(_touch_valid && _last_touch_valid){
		swipe.rawx = touch.px - last_touch.px;
		swipe.rawy = touch.py - last_touch.py;
	}
	if(!_touch_valid){
		swipe.z2 = 0;
		swipe.z1 += 1;
		int increase = (swipe.z1 >> 4 != swipe.z2 >> 4 ? swipe.z1 >> 4 : 0);

		swipe.rawx -=  clamp((s16)swipe.rawx, -increase, increase);
		swipe.rawy -=   clamp((s16)swipe.rawy, -increase, increase);
	}
	//if(swipe.rawx != 0 || swipe.rawy != 0) printf("%d:(%d, %d) - (%d,%d)\n",!!(_keys_held & KEY_TOUCH),swipe.z1, swipe.z2 ,(s16)swipe.rawx,(s16)swipe.rawy);
}

bool _HandleSwaps(){
	if(_keys_pressed & KEY_START){
		mode = (mode == MODE_PAUSED ? MODE_DRAWING : MODE_PAUSED);
		return true;
	}
	if(_keys_pressed & KEY_X && mode != MODE_PAUSED)
	{
		mode = (mode == MODE_DRAWING ? MODE_COLORS : MODE_DRAWING);
		return true;
	}
	return false;

}

void ModeInit(){
	draw_width = 3;
	mode = MODE_DRAWING;
}

void DrawMode(){
	if(!_touch_valid){
		return;
	}
	//handle inputs
	if(_keys_held & KEY_L)
		draw_width = 9;
	else
		draw_width = 3;

	u16 color = (_keys_held & KEY_L ? 0 : draw_color);
	int i,j;
	if(!_last_touch_valid){
		for(i = -(draw_width-1)/2; i <= draw_width/2; i++)
			for(j = -(draw_width-1)/2; j <= draw_width/2; j++)
					FillRectangle(MAIN,touch.py +j,touch.py+j,touch.px+i,touch.px+i, color);
	}
	else{

		for(i = -(draw_width-1)/2; i <= draw_width/2; i++)
			for(j = -(draw_width-1)/2; j <= draw_width/2; j++)
				DrawLine(MAIN,last_touch.px+i,last_touch.py+j,touch.px+i,touch.py+j, color);
	}
}

int radd,badd,gadd;
void ColorMode(){
	if(!_swipe_valid)
		return;

	int addition = (s16)(touch.py - swipe.py) / 5;

	if(!_touch_valid){
		color_red += radd;
		color_green +=gadd;
		color_blue += badd;
		radd = gadd = badd = 0;
		return;
	}
	if(swipe.px < 256/3)
		radd = clamp(addition, -color_red, 31 - color_red);
	else if(swipe.px < 512/3)
		gadd = clamp(addition,-color_green, 31 - color_green);
	else
		badd = clamp(addition,-color_blue, 31 - color_blue);

	BG_PALETTE[color_chosen] = RGB15(color_red + radd,color_green + gadd,color_blue + badd);
	printf("RGB15(%d,%d,%d)\n",color_red + radd,color_green + gadd,color_blue + badd);
}
