#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>
#include <psxgpu.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"

void PS1_GsSetDrawEnv(int x,int y,int w ,int h)
{
	int end_y, end_x;
	int mf;

	GsDrawEnv drawenv;
	drawenv.dither = 0;
	drawenv.draw_on_display = 1;
	drawenv.x = 0;
	drawenv.y = 0;
	drawenv.w = 320;
	drawenv.h = 240;
	drawenv.ignore_mask = 0;
	drawenv.set_mask = 0;

/*
	 * Store the 0xe1 packet - we need it because we have to
	 * modify drawing environment for sprites
	 */

	int draw_mode_packet = (0xe1<<24)|(drawenv.draw_on_display>=1)<<10|
		(drawenv.dither>=1)<<9;


	gpu_data_ctrl(0xe1, draw_mode_packet);
	gpu_data_ctrl(0xe2, 0);
	gpu_data_ctrl(0xe3, (y<<10)|x);

	end_x = (x + w)-1;
	end_y = (y + h)-1;

	gpu_data_ctrl(0xe4, (end_y<<10)|end_x);

	//#warning "Check drawing offset better."
	gpu_data_ctrl(0xe5, (y<<11)|x);
	//gpu_data_ctrl(0xe5, 0);


	mf = 0;
	if(drawenv.set_mask) mf|=MASK_SET;
	if(drawenv.ignore_mask) mf|=MASK_IGNORE;

	GsSetMasking(mf);

	//GsCurDrawEnvW = drawenv->w;
	//GsCurDrawEnvH = drawenv->h;
}

void PS1_GsSetDispEnv(int x,int y)
{
	gpu_ctrl(5, (y<<10)|x); // Display offset
}


void PS1_VBlank_handler();

//Init Console or other
void LMP3D_Init()
{
	PSX_InitEx(PSX_INIT_CD);

	GsInit();
	GsClearMem();

	SetVBlankHandler(PS1_VBlank_handler);

	PS1_GTE_Init();
	PS1_Buffer_Init();
}


#endif


