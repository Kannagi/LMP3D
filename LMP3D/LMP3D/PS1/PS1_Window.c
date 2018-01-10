#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>
#include <psxgpu.h>

#include "LMP3D/LMP3D.h"




#include "LMP3D/PS1/PS1.h"

static volatile unsigned int IVBlankCnt = 0;

void PS1_VBlank_handler()
{
	IVBlankCnt++;

}

//Size Screen , Window...
LMP3D_Buffer LMP3D_Window(char *name)
{
	LMP3D_Buffer buffer;

	buffer.width = 320;
	buffer.height = 240;
	buffer.fpsm = PS1_PSM16;
	buffer.zpsm = 0;
	buffer.faddress1 = PS1_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.faddress2 = PS1_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.zaddress  = 0;

	buffer.switchBuffer = 0;


	// This has to be interlaced
	GsSetVideoModeEx(buffer.width, buffer.height, VMODE_NTSC, 0, 1, 0);



	PS1_GsSetDispEnv(0,0);
	PS1_GsSetDrawEnv(0,0,buffer.width ,buffer.height);


	return buffer;
}

//Wait VBlank
int LMP3D_VBlank()
{
	int VBlankCnt = 0;

	IVBlankCnt = 0;
	while(IVBlankCnt == 0) VBlankCnt++;
	return VBlankCnt;
}



#endif


