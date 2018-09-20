#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "LMP3D/LMP3D.h"


#include "LMP3D/PS2/PS2.h"

void LMP3D_Clear()
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[64] __attribute__((aligned(64)));

	gif = UNCACHED_SEG(gif_array);

	u64 primv = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT, GS_TME_TEXTURE_OFF, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_STQ, GS_CTXT_0, GS_FIX_0);
	gif[i++] = GS_SET_GIFTAG(5,1,1,primv,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_TEST(DRAW_DISABLE,ATEST_METHOD_EQUAL,0x80,ATEST_KEEP_FRAMEBUFFER,DRAW_DISABLE,DRAW_DISABLE,DRAW_ENABLE,ZTEST_METHOD_ALLPASS);
	gif[i++] = 0x47;

	gif[i++] = GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x00, 0);
	gif[i++] = GS_REG_RGBAQ;


	gif[i++] = GS_SET_XYZ((2048-320)<<4, (2048-256)<<4, 1024<<4);
	gif[i++] = GS_REG_XYZ2;

	gif[i++] = GS_SET_XYZ((2048+320)<<4, (2048+256)<<4, 1024<<4);
	gif[i++] = GS_REG_XYZ2;


	gif[i++] = GS_SET_TEST(DRAW_ENABLE,ATEST_METHOD_GREATER_EQUAL,0x80,ATEST_KEEP_FRAMEBUFFER,DRAW_DISABLE,DRAW_DISABLE,DRAW_ENABLE,ZTEST_METHOD_GREATER_EQUAL );
	gif[i++] = 0x47;



	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i>>1;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);

	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);
}

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{
	//printf("buf2 %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);
	int i = 0;
	u64 *gif;
	u64 gif_array[64] __attribute__((aligned(64)));

	gif =  UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	if(buffer->switchBuffer == 0)
	{
		RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(buffer->faddress1>>11,buffer->width>>6,buffer->fpsm,0,0);
		RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(buffer->faddress1>>11,buffer->width>>6,buffer->fpsm,0,0);

		gif[i++] = GS_SET_FRAMEX(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0);
		gif[i++] = GS_REG_FRAME_1;

	}else
	{
		RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0,0);
		RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0,0);

		gif[i++] = GS_SET_FRAMEX(buffer->faddress1>>11,buffer->width>>6,buffer->fpsm,0);
		gif[i++] = GS_REG_FRAME_1;
	}

	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i>>1;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);

	buffer->switchBuffer = !buffer->switchBuffer;

	while( (RW_REGISTER_U32(D2_CHCR)) & ((unsigned int)1<<8));
}


#endif


