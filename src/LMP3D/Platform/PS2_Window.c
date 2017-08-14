#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2
#include <kernel.h>
#include "LMP3D/LMP3D.h"
#include "LMP3D/Platform/PS2.h"


LMP3D_Buffer LMP3D_Window(char *name)
{
	LMP3D_Buffer buffer;
	buffer.width = 640;
	buffer.height = 256;
	buffer.fpsm = GS_PSMCT32;
	buffer.zpsm = GS_PSMZ16S;
	buffer.faddress1 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.faddress2 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.zaddress  = PS2_vram_allocate(buffer.width,buffer.height, buffer.zpsm);

	buffer.switchBuffer = 0;


	PS2_Graphic_Init(buffer.width ,buffer.height ,buffer.fpsm ,buffer.faddress1);
	PS2_Graphic_Setup(buffer);

	PS2_VU_Init();

	return buffer;
}

void LMP3D_Fps(int fps)
{

}

int LMP3D_VBlank()
{
	int VBlankCnt = 0;

	//VBlank interrupt enable
	GS_SET_CSR(0,0,0,1,0,0,0,0,0,0,0,0);

	//wait VBlank

	while( !( ( RW_REGISTER_U32(GS_CSR) ) & 0x08) ) VBlankCnt++;

	return VBlankCnt;
}


#endif
