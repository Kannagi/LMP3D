#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2
#include <kernel.h>
#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"
void PS2_VU_Init2D();

AURAE_Buffer AURAE_Window(char *name)
{
	AURAE_Buffer buffer;
	buffer.width = 640;
	buffer.height = 448;
	buffer.fpsm = GS_PSMCT16;
	buffer.zpsm = GS_PSMZ16;
	buffer.faddress1 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.faddress2 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.zaddress  = PS2_vram_allocate(buffer.width,buffer.height, buffer.zpsm);

	buffer.switchBuffer = 0;


	PS2_Graphic_Init(buffer.width ,buffer.height ,buffer.fpsm ,buffer.faddress1);
	PS2_Graphic_Setup(buffer);

	PS2_VU_Init();
	PS2_VU_Init2D();

	return buffer;
}



int AURAE_VBlank()
{
	int VBlankCnt = 0;

	//VBlank interrupt enable
	GS_SET_CSR(0,0,0,1,0,0,0,0,0,0,0,0);

	//wait VBlank
	while( !( ( RW_REGISTER_U32(GS_CSR) ) & 0x08) ) VBlankCnt++;

	return VBlankCnt;
}


#endif
