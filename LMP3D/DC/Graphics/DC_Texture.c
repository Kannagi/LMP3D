#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

void __attribute__((optimize("-O1"), noinline)) LMP3D_Texture_Upload_VRAM_Pal(LMP3D_Texture *texture)
{
	unsigned short *palette = UNCACHED_P2(texture->palette);
	unsigned int *paldc = UNCACHED_P2(PALETTE_RAM);

	int address = texture->addresspal<<4;
	int n = texture->palsize>>1,i;
	for(i = 0;i < n;i++)
		paldc[i+address] = palette[i];
}

void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{
	void *pixel;
	int dif;

	pvr_ptr_t address = texture->address&0x7FFFFF;
	pixel = texture->pixel;
	dif = 0x100 - ( (u32)pixel&0xFF);

	while(!pvr_dma_ready());
	pvr_txr_load_dma( pixel+dif,address,texture->size,0,NULL,0);
	while(!pvr_dma_ready());

	return;
/*
	pixel = malloc(texture->size+0x100);
	ncpixel = UNCACHED_P2(pixel);
	dif = 0x100 - ( (u32)pixel&0xFF);
	memcpy(ncpixel+dif,texture->pixel, texture->size);

	while(!pvr_dma_ready());
	pvr_txr_load_dma( pixel+dif,address,texture->size,0,NULL,0);
	while(!pvr_dma_ready());

	free(pixel);*/
}

void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{
	if(texture == NULL) return;

	//-------
	void *pixel,*ncpixel;
	int dif;

	pvr_ptr_t address = texture->address&0x7FFFFF;
	pixel = malloc(texture->size+0x100);
	ncpixel = UNCACHED_P2(pixel);
	dif = 0x100 - ( (u32)pixel&0xFF);
	memcpy(ncpixel+dif,texture->pixel, texture->size);

	free(texture->pixel);
	texture->pixel = pixel;
	//-------

	texture->address = DC_vram_allocate(texture->w,texture->h, texture->psm);
	LMP3D_Texture_Upload_VRAM(texture);


	if(texture->palette == NULL) return;
	texture->addresspal = DC_vram_allocate(texture->palsize>>1,1, texture->psm);
	texture->addresspal = texture->addresspal>>24;
	LMP3D_Texture_Upload_VRAM_Pal(texture);
}

#endif


