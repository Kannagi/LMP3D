/*
Test Dreamcast pvr lib




*/
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST

#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

void DC_InitClip()
{
	u32 *tpoly = UNCACHED_P4(0);

	tpoly[0] = TA_PARA_UTC;

	tpoly[4] = 0x0;
	tpoly[5] = 0x0;
	tpoly[6] = 0xF+4;
	tpoly[7] = 0x8+6;

	asm("pref @%0" : : "r" (tpoly));
}

void DC_Init_Poly(LMP3D_Texture *texture,int para)
{
	u32 *tpoly = UNCACHED_P4(0);

	int psm = texture->psm&0x0F;
	int vqt = (texture->psm&0xF0);
	int tw = 1,vq = 0;

	if(vqt & TA_TEXTURE_VQ) vq = 1;
	if(vqt & TA_TEXTURE_TWILLED) tw = 0;
	int usize = 5,vsize = 5;

	if(texture->w ==  128) usize = 4;
	if(texture->h ==  128) vsize = 4;
	if(texture->w ==  512) usize = 6;
	if(texture->h ==  512) vsize = 6;
	if(texture->w == 1024) usize = 7;
	if(texture->h == 1024) vsize = 7;

	u32 address = (texture->address&0x7FFFFF)>>3;

	if(para == TA_PARA_POLYGON)
	{
		tpoly[0] = TA_PARA_POLYGON | TA_GROUP_ENABLE | TA_GROUP_USERTILECLIP_ENABLEIN | TA_GROUP_STRIPLEN1 | TA_OBJ_16BITUV |TA_OBJ_TEXTURE | TA_PARA_OPOLY;// | TA_OBJ_OFFSET;
		tpoly[1] = TA_ISP_DEPTH_GREATER | TA_ISP_CULLING_POSITIVE;
		tpoly[2] = TA_TSP_SRCALPHA(1) | TA_TSP_DSTALPHA(0) | TA_TSP_FOG_DISABLE | TA_TSP_USIZE(usize) | TA_TSP_VSIZE(vsize) | TA_TSP_TEXTURE_DECAL | TA_TSP_FILTER_BILINEAR;
	}else
	{
		tpoly[0] = TA_PARA_SPRITE | TA_GROUP_ENABLE | TA_GROUP_USERTILECLIP_DISABLE | TA_OBJ_16BITUV | TA_OBJ_TEXTURE | TA_PARA_TPOLY;
		tpoly[1] = TA_ISP_ZWRITE_DISABLE;
		tpoly[2] = TA_TSP_SRCALPHA(1) | TA_TSP_DSTALPHA(5) | TA_TSP_FOG_DISABLE | TA_TSP_USIZE(usize) | TA_TSP_VSIZE(vsize) | TA_TSP_TEXTURE_DECAL | TA_TSP_FILTER_POINT;
	}

	if( (psm & TA_TEXTURE_8BPP) || (psm & TA_TEXTURE_4BPP) )
		tpoly[3] = TEXTURE_CONTROL_WORD_PAL(0,0,psm,texture->addresspal,address);
	else
		tpoly[3] = TEXTURE_CONTROL_WORD_RGB(0,vq,psm,tw,0,address);

	tpoly[4] = 0xFF000000;
	tpoly[5] = 0xFF000000;
	tpoly[6] = 0x00000000;
	tpoly[7] = 0xFFFFFFFF;

	asm("pref @%0" : : "r" (tpoly));


	//tpoly[6] = 0xFFFFFFFF; //modulate
	tpoly[7] = 0x00808080; //Offset
}

#endif
