#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include <kernel.h>
#include "LMP3D/LMP3D.h"
#include "LMP3D/Platform/PS2_Register.h"




void load_texture2(void *src,int width, int height, int psm, int dest, int dest_width)
{
	int i = 0;
	unsigned long gif[64] __attribute__((aligned(64)));
	int remaining;

	remaining  = 0x4000;


	// Setup the transfer
	gif[i++] = GS_SET_DMATAG(5,0,1,0,0,0);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(4,0,0,0,0,1);
	gif[i++] = 0x0E;


	gif[i++] = GS_SET_BITBLTBUF(0,0,0,dest>>6,dest_width>>6,psm);
	gif[i++] = 0x50;


	gif[i++] = GS_SET_TRXPOS(0,0,0,0,0);
	gif[i++] = 0x51;

	gif[i++] = GS_SET_TRXREG(width,height);
	gif[i++] = 0x52;

	gif[i++] = GS_SET_TRXDIR(0);
	gif[i++] = 0x53;





	gif[i++] = GS_SET_DMATAG(1,0,1,0,0,0);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(remaining,0,0,0,2,0);
	gif[i++] = 0x00;


	gif[i++] = GS_SET_DMATAG(remaining,0,3,0,(u32)src,0);
	gif[i++] = 0;


	gif[i++] = GS_SET_DMATAG(2,0,7,0,0,0);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = 0x0E;


	gif[i++] = 1;
	gif[i++] = 0x3F;

	RW_REGISTER_U32(D_STAT) = DMA_SET_STAT(1 << 2,0,0,0,0,0,0);

	SyncDCache(gif, gif + (i<<3));

	RW_REGISTER_U32(D2_QWC ) = 0;
	RW_REGISTER_U32(D2_MADR) = 0;
	RW_REGISTER_U32(D2_TADR) = (u32)gif;
	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,1,0,0,1,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) &0x100);



}

inline unsigned char draw_log2(unsigned int x)
{

	unsigned char res;

	__asm__ __volatile__ ("plzcw %0, %1\n\t" : "=r" (res) : "r" (x));

	res = 31 - (res + 1);
	res += (x > (1<<res) ? 1 : 0);

	return res;
}




void LMP3D_Texture_Setup(LMP3D_Texture *texbuf)
{
	PS2_clut clut;

	PS2_lod lod;

	lod.calculation = 1;
	lod.max_level = 0;
	lod.mag_filter = 0;
	lod.min_filter = 0;
	lod.l = 0;
	lod.k = 0;

	int info_width = draw_log2(texbuf->w);
	int info_height = draw_log2(texbuf->w);
	int info_components = 1;
	int info_function = 1;

	clut.storage_mode = 0;
	clut.start = 0;
	clut.psm = 0;
	clut.load_method = 0;
	clut.address = 0;

	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[128] __attribute__((aligned(64)));


	gif = UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(2,1,0,0,0,1);
	gif[i++] = 0x0E;

	gif[i++] = GS_SET_TEX0(texbuf->address>>6,texbuf->w>>6,texbuf->psm,
							   info_width,info_height,info_components,info_function,
							   clut.address>>6,clut.psm,clut.storage_mode,clut.start,clut.load_method);
	gif[i++] = 0x06;

	gif[i++] = GS_SET_TEX1(lod.calculation,lod.max_level,lod.mag_filter,lod.min_filter,lod.mipmap_select,lod.l,(int)(lod.k*16.0f));
	gif[i++] = 0x14;


	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);



}



void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{
	if(texture == NULL) return;

	texture->address = PS2_vram_allocate(texture->w,texture->h, texture->psm);
	load_texture2(texture->pixel,256,256,texture->psm,texture->address,texture->w);


}




void LMP3D_Texture_Delete(int id)
{

}

#endif


