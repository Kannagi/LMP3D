#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include <kernel.h>
#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"

void AURAE_Texture_Upload_VRAM_Pal(AURAE_Texture *texture)
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[64] __attribute__((aligned(64)));
	int remaining;

	gif = UNCACHED_SEG(gif_array);

	remaining = 32;
	int n = texture->palsize>>1;
	int h = 16;
	int w = 16;

	if(n != 256)
	{
		w = h = 8;
		n = 64;
	}

	int address = texture->addresspal;

	// Setup the transfer
	gif[i++] = DMA_CNT_TAG(5);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(4,0,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_BITBLTBUF(0,0,0,address>>6,n>>6,GS_PSMCT16);
	gif[i++] = GS_REG_BITBLTBUF;

	gif[i++] = GS_SET_TRXPOS(0,0,0,0,0);
	gif[i++] = GS_REG_TRXPOS;

	gif[i++] = GS_SET_TRXREG(w,h);
	gif[i++] = GS_REG_TRXREG;

	gif[i++] = GS_SET_TRXDIR(0);
	gif[i++] = GS_REG_TRXDIR;

	gif[i++] = GS_SET_DMATAG(1,0,1,0,0,0);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(remaining,0,0,0,2,0);
	gif[i++] = 0x00;

	gif[i++] = DMA_REF_TAG((u32)texture->palette,remaining);
	gif[i++] = 0;

	gif[i++] = DMA_END_TAG(2);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = 1;
	gif[i++] = GS_REG_TEXFLUSH;

	//RW_REGISTER_U32(D_STAT) = DMA_SET_STAT(1 << 2,0,0,0,0,0,0);

	//SyncDCache(gif, gif + (i<<3));

	RW_REGISTER_U32(D2_QWC ) = 0;
	RW_REGISTER_U32(D2_MADR) = 0;
	RW_REGISTER_U32(D2_TADR) = (u32)gif_array;
	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,1,0,0,1,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) &0x100);
}

void AURAE_Texture_Upload_VRAM(AURAE_Texture *texture)
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[64] __attribute__((aligned(64)));
	int remaining;

	remaining  = texture->size>>4;
	int address = texture->address&0x00FFFFFF;

	gif = UNCACHED_SEG(gif_array);

	// Setup the transfer
	gif[i++] = DMA_CNT_TAG(5);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(4,0,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_BITBLTBUF(0,0,0,address>>6,texture->w>>6,texture->psm);
	gif[i++] = GS_REG_BITBLTBUF;

	gif[i++] = GS_SET_TRXPOS(0,0,0,0,0);
	gif[i++] = GS_REG_TRXPOS;

	gif[i++] = GS_SET_TRXREG(texture->w,texture->h);
	gif[i++] = GS_REG_TRXREG;

	gif[i++] = GS_SET_TRXDIR(0);
	gif[i++] = GS_REG_TRXDIR;

	gif[i++] = GS_SET_DMATAG(1,0,1,0,0,0);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(remaining,0,0,0,2,0);
	gif[i++] = 0x00;

	gif[i++] = DMA_REF_TAG((u32)texture->pixel,remaining);
	gif[i++] = 0;

	gif[i++] = DMA_END_TAG(2);
	gif[i++] = 0;

	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = 1;
	gif[i++] = GS_REG_TEXFLUSH;

	RW_REGISTER_U32(D_STAT) = DMA_SET_STAT(1 << 2,0,0,0,0,0,0);

	SyncDCache(gif, gif + (i<<3));

	RW_REGISTER_U32(D2_QWC ) = 0;
	RW_REGISTER_U32(D2_MADR) = 0;
	RW_REGISTER_U32(D2_TADR) = (u32)gif_array;
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

void AURAE_Texture_Setup(AURAE_Texture *texbuf)
{
	if(texbuf == NULL) return;
	static AURAE_Texture *texbufrepeat = NULL;
	if(texbufrepeat == texbuf) return;
	texbufrepeat = texbuf;

	PS2_clut clut;
	PS2_lod lod;

	lod.mipmap_select = 0;
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

	int address;
	clut.address =  texbuf->addresspal;

	clut.psm = GS_PSMCT16; //PSM
	clut.storage_mode = 0; //CSM

	clut.start = 0;
	clut.load_method = 1;

	address = texbuf->address&0x00FFFFFF;

	unsigned long *gif;
	unsigned long gif_array[16] __attribute__((aligned(64)));

	gif = UNCACHED_SEG(gif_array);

	gif[0] = GS_SET_GIFTAG(3,1,0,0,0,1);
	gif[1] = GS_REG_AD;

	gif[2] = GS_SET_TEXA(0,0,255);
	gif[3] = GS_REG_TEXA;

	gif[4] = GS_SET_TEX0(address>>6,texbuf->w>>6,texbuf->psm,
							   info_width,info_height,info_components,info_function,
							   clut.address>>6,clut.psm,clut.storage_mode,clut.start,clut.load_method);
	gif[5] = GS_REG_TEX0_1;

	gif[6] = GS_SET_TEX1(lod.calculation,lod.max_level,lod.mag_filter,lod.min_filter,lod.mipmap_select,lod.l,(int)(lod.k*16.0f));
	gif[7] = GS_REG_TEX1_1;

	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = 4;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);

	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);
}

void AURAE_Texture_Upload(AURAE_Texture *texture)
{
	if(texture == NULL) return;

	texture->address = PS2_vram_allocate(texture->w,texture->h, texture->psm);
	AURAE_Texture_Upload_VRAM(texture);

	if(texture->palette == NULL) return;

	if(texture->palsize == 512)
		texture->addresspal = PS2_vram_allocate(256,1,GS_PSMCT16);
	else
		texture->addresspal = PS2_vram_allocate(128,1,GS_PSMCT16);

	AURAE_Texture_Upload_VRAM_Pal(texture);
}


#endif


