#include "P_Graphics.h"

u16* P_Graphics_mainBuffer;
u16* P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;

bool using_vram_a = true;
bool using_vram_c = true;
void P_Graphics_setup_main()
{
#ifdef FB0
	//Graphics (Part 1)
	//Set the mode to FB0
	REG_DISPCNT = MODE_FB0;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_LCD;


	P_Graphics_assignBuffer(MAIN, (u16*)VRAM_A,256,192);

#endif

#ifdef ROTOSCALE
	//BG0 will be a tilemap for the sky and floor, using VRAM B
	//BG2 will be a a rotoscale for the walls, using VRAM A
	REG_DISPCNT = MODE_4_2D | DISPLAY_BG2_ACTIVE ;//| DISPLAY_BG0_ACTIVE;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	//VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG;

	BGCTRL[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
	//BGCTRL[0] = BG_MAP_BASE(8) | BG_32x32;

	BG_PALETTE[1] = RGB15(15,15,31);
	BG_PALETTE[2] = RGB15(15,31,15);
	BG_PALETTE[3] = RGB15(31,15,15);

	P_Graphics_assignBuffer(MAIN, (u16*)BG_GFX,256,192);
	REG_BG2PA = 256;
	REG_BG2PC = 0;
	REG_BG2PB = 0;
	REG_BG2PD = 256;

#endif
}

inline u16* get_buffer_pointer(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainBuffer : P_Graphics_subBuffer;}
inline int get_buffer_width(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainW : P_Graphics_subW;}
inline int get_buffer_height(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainH : P_Graphics_subH;}

void P_Graphics_assignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h)
{

    switch(bT)
    {
        case MAIN: P_Graphics_mainBuffer = buffer;
            P_Graphics_mainW = w;
            P_Graphics_mainH = h;
            break;
        case SUB: P_Graphics_subBuffer = buffer;
            P_Graphics_subW = w;
            P_Graphics_subH = h;
            break;
    }
}


void FillScreen(enum BUFFER_TYPE t, u16 color)
{
	int i;
	u16* P_Buffer = get_buffer_pointer(t);
	int P_BufferH = get_buffer_height(t);
	int P_BufferW = get_buffer_width(t);
		//Fill the whole screen (256*192) with the given color
	for(i = 0; i<P_BufferH*P_BufferW; i++){
		//color=color&0b11111 |(color&(0b11111<<5)) | color&(0b11111<<10);
		P_Buffer[i] = color;
	}

}

void FillRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferH = get_buffer_height(bT);
	int P_BufferW = get_buffer_width(bT);

	top = clamp(top, 0, P_BufferH);
	bottom = clamp(bottom, -1, P_BufferH-1);
	left = clamp(left, 0, P_BufferW);
	right = clamp(right, -1, P_BufferW-1);

	if(right < left || bottom < top)
		return;

	short i, j;

	for(j = top; j <= bottom; j++){
		for(i = left; i <= right; i++){
			P_Buffer[coords(i,j,P_BufferW)] = color;
		}
	}
}

void DrawHorizontalLine(enum BUFFER_TYPE bT, int x, int x2, int y, u16 color){
	if(x2 >= x)
		FillRectangle(bT,y,y,x,x2,color);
	else
		FillRectangle(bT,y,y,x2,x,color);
}
void DrawVerticalLine(enum BUFFER_TYPE bT, int y, int y2, int x, u16 color)
{
	if(y2 >= y)
		FillRectangle(bT,y,y2,x,x,color);
	else
		FillRectangle(bT,y2,y,x,x,color);
}

void DrawRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	DrawVerticalLine(bT,top,bottom,left,color);
	DrawVerticalLine(bT,top,bottom,right,color);
	DrawHorizontalLine(bT,left,right,top,color);
	DrawHorizontalLine(bT,left,right,bottom,color);

}

void swap_buffers(enum BUFFER_TYPE bT){

#ifdef FB0

	switch(bT){
	case MAIN:
		if(using_vram_a) P_Graphics_assignBuffer(MAIN,(u16*)VRAM_B,256,192);
		else P_Graphics_assignBuffer(MAIN,(u16*)VRAM_A,256,192);

		//Set the coresponding VRAM bank
		if(using_vram_a) {REG_DISPCNT = MODE_FB0;}
		else {REG_DISPCNT = MODE_FB1;}
		using_vram_a = !using_vram_a;
		break;
	case SUB:
		break;
	}
#endif


#ifdef ROTOSCALE
	switch(bT){
	case MAIN:
		break;
	case SUB:
		break;
	}
#endif
}
