#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/PS2/PS2.h"

/*
	LMP3D_VIDEO_MODE0 640x256 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE1 640x240 interlaced 50 HZ (PAL)

	LMP3D_VIDEO_MODE2 640x512 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE3 640x480 interlaced 50 HZ (PAL)

	LMP3D_VIDEO_MODE4 320x240 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE5 320x240 interlaced 50 HZ (PAL)

*/

void PS2_Graphic_Init(int width, int height, int psm,int fbaddr)
{
	// Reset GS.
	RW_REGISTER_U64(GS_CSR) = (unsigned long)1<<9;

	// Clear GS CSR.
	RW_REGISTER_U64(GS_CSR) = 0;


	// Unmask GS VSYNC Interrupt.
	GsPutIMR(0x00007F00);

	// Set the requested mode.
	SetGsCrt(0, 2, 0);

	//GS_SET_SMODE1(4,32,1,0,2,0,1,1,0,0,4,0,0,0,0,0,1,1,1,1,0);


	// Set black background
	RW_REGISTER_U64(GS_BGCOLOR) = GS_SET_BGCOLOR(0,0,0);

	// Set the screen up
	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),(height-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),(height-1));


	RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(fbaddr>>11,width>>6,psm,0,0);
	RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(fbaddr>>11,width>>6,psm,0,0);


	RW_REGISTER_U64(GS_PMODE) = GS_SET_PMODE(1,1,1, 1, 0, 0, 0x70);

}

void PS2_Graphic_Setup(LMP3D_Buffer buffer)
{
	int i = 0;
	u64 *gif;
	u64 gif_array[64] __attribute__((aligned(64)));

	gif =  UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(5,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_FRAMEX(buffer.faddress1>>11,buffer.width>>6,buffer.fpsm,0);
	gif[i++] = 0x4C;

	gif[i++] = GS_SET_ZBUFX(buffer.zaddress>>11,buffer.zpsm,0);
	gif[i++] = 0x4E;


	gif[i++] = GS_SET_XYOFFSET((2048-320)<<4,(2048-128)<<4);
	gif[i++] = 0x18;


	gif[i++] = GS_SET_SCISSORX(0,(buffer.width-1),0,(buffer.height-1));
	gif[i++] = 0x40;


	gif[i++] = GS_SET_PRMODECONT(1);
	gif[i++] = 0x1A;



	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i>>1;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & ((unsigned int)1<<8));

}

#endif

