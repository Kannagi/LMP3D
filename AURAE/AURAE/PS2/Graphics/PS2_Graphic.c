#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"

/*
	AURAE_VIDEO_MODE0 640x256 interlaced 60 HZ (NTSC)
	AURAE_VIDEO_MODE1 640x240 interlaced 50 HZ (PAL)

	AURAE_VIDEO_MODE2 640x512 interlaced 60 HZ (NTSC)
	AURAE_VIDEO_MODE3 640x480 interlaced 50 HZ (PAL)

	AURAE_VIDEO_MODE4 320x240 interlaced 60 HZ (NTSC)
	AURAE_VIDEO_MODE5 320x240 interlaced 50 HZ (PAL)

*/

/*
GRAPH_MODE graph_mode[22] =
{

	{   0,   0,	0,	0,	0 }, // AUTO
	{ 652,  26, 2560,  224, 0x02 }, // NTSC-NI
	{ 680,  37, 2560,  256, 0x03 }, // PAL-NI
	{ 232,  35, 1440,  480, 0x50 }, // 480P
	{ 320,  64, 1312,  576, 0x53 }, // 576P only in bios>=220
	{ 420,  40, 1280,  720, 0x52 }, // 720P
	{ 300, 120, 1920,  540, 0x51 }, // 1080I
	{ 280,  18, 1280,  480, 0x1A }, // VGA   640x480@60
	{ 330,  18, 1280,  480, 0x1B }, // VGA   640x480@72
	{ 360,  18, 1280,  480, 0x1C }, // VGA   640x480@75
	{ 260,  18, 1280,  480, 0x1D }, // VGA   640x480@85
	{ 450,  25, 1600,  600, 0x2A }, // VGA   800x600@56
	{ 465,  25, 1600,  600, 0x2B }, // VGA   800x600@60
	{ 465,  25, 1600,  600, 0x2C }, // VGA   800x600@72
	{ 510,  25, 1600,  600, 0x2D }, // VGA   800x600@75
	{ 500,  25, 1600,  600, 0x2E }, // VGA   800x600@85
	{ 580,  30, 2048,  768, 0x3B }, // VGA  1024x768@60
	{ 266,  30, 1024,  768, 0x3C }, // VGA  1024x768@70
	{ 260,  30, 1024,  768, 0x3D }, // VGA  1024x768@75
	{ 290,  30, 1024,  768, 0x3E }, // VGA  1024x768@85
	{ 350,  40, 1280, 1024, 0x4A }, // VGA 1280x1024@60
	{ 350,  40, 1280, 1024, 0x4B }, // VGA 1280x1024@75

};
*/
void PS2_Graphic_Init(int width, int height, int psm,int fbaddr)
{
	// Reset GS.
	RW_REGISTER_U64(GS_CSR) = (unsigned long)1<<9;

	// Clear GS CSR.
	RW_REGISTER_U64(GS_CSR) = 0x10;

	// Unmask GS VSYNC Interrupt.
	GsPutIMR(0x00007F00);

	//GS_SET_SMODE1(4,32,1,0,2,0,1,1,0,0,4,0,0,0,0,0,1,1,1,1,0);

	/*
	// Set the requested mode.
	SetGsCrt(0, 2, 0);

	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),((224*1)-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),((224*1)-1));

*/

	SetGsCrt(1, 2, 0);

	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(652,45,3,0,((width*4)-1),((height)-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(652,45,3,0,((width*4)-1),((height)-1));

/*
	// Set the requested mode.
	SetGsCrt(0, 0x50, 0);

	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(232,35,3,0,(1440-1),((480*1)-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(232,35,3,0,(1440-1),((480*1)-1));

/*
	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),((height*1)-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),((height*1)-1));

/*
	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(300,120,2,0,(1920-1),(height-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(300,120,2,0,(1920-1),(height-1));

*/
	RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(fbaddr>>11,width>>6,psm,0,0);
	RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(fbaddr>>11,width>>6,psm,0,0);


	RW_REGISTER_U64(GS_PMODE) = GS_SET_PMODE(1,1,1, 1, 0, 0, 0x70);

	// Set black background
	RW_REGISTER_U64(GS_BGCOLOR) = GS_SET_BGCOLOR(0,0,0);


}

void PS2_Graphic_Setup(AURAE_Buffer buffer)
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


	gif[i++] = GS_SET_XYOFFSET((2048-(buffer.width>>1))<<4,(2048-(buffer.height>>1))<<4);
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

