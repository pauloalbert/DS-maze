/*
 * game.c
 *
 *  Created on: Mar 22, 2011
 *      Author: nds
 */

#include "game.h"

void playSoundEffect()
{
	mmEffectEx( &sound );
}

void gameInit()
{
	/*
	 * Sound initialization
	 */
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoadEffect( SFX_LASER );
	sound.id      = SFX_LASER;			 // sample ID (make sure it is loaded)
	sound.rate    = 0x400/2;     // playback rate, 1024 = original sound
	sound.handle  = 0;           // 0 = allocate new handle
	sound.volume  = 255;         // 200/255 volume level
	sound.panning = 128;         // centered panning

	/*
	 * Chrono Displays initialization
	 */
	initChronoDisp_Main();
	initChronoDisp_Sub();
	updateChronoDisp_Main(0, 0, 0);
	updateChronoDisp_Sub(0, 0, 0);
	changeColorDisp_Main(YELLOW);
	changeColorDisp_Sub(YELLOW);

	/*
	 * KEYS Initialization
	 */
	REG_KEYCNT = (1<<14)| KEY_A | KEY_LEFT | KEY_START;
}
