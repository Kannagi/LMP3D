#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

static unsigned int DC_vram_pointer = 0;

unsigned int DC_vram_size(int width,int height, int psm);


unsigned int DC_vram_allocate(int width, int height, int psm)
{
	unsigned int size;

	size = DC_vram_size(width,height,psm);

	if(height == 1)
	{
		size = width>>4;
		if(width&0x0F) size++;

		size = size<<24;
		int tampon = ((DC_vram_pointer+size)>>24)&0xFF;
		if(tampon >= 0x40) return - 1;
	}

	DC_vram_pointer += size;
/*
	if( (height != 1) && (DC_vram_pointer > 0X800000) )
	{
		DC_vram_pointer -= size;
		return -1;
	}
*/
	return DC_vram_pointer - size;
}

void DC_vram_set(int address)
{
	DC_vram_pointer = address;
}

int DC_vram_get()
{
	return DC_vram_pointer;
}

void DC_vram_clear(void)
{
	DC_vram_pointer = 0;
}


unsigned int DC_vram_size(int width,int height, int psm)
{
	int size = width*height;

	// Texture storage size is in pixels/word

	if( (psm&0xF0) & TA_TEXTURE_VQ)
		return ((width*width)>>2)+512;

	switch (psm&0x0F)
	{
		case TA_TEXTURE_4BPP:		size = size>>1; break;
		case TA_TEXTURE_8BPP:
		case TA_TEXTURE_YUV422:		size = size; break;
		case TA_TEXTURE_RGB1555:
		case TA_TEXTURE_RGB565:
		case TA_TEXTURE_RGB4444:	size = size<<1; break;


		default:

		return 0;
	}

	return size;
}

void LMP3D_VRAM_Set(unsigned int address)
{
	DC_vram_pointer = address;
}

unsigned int LMP3D_VRAM_Get()
{
	return DC_vram_pointer;
}

void LMP3D_VRAM_Info(char *info)
{
	int palette = (DC_vram_pointer>>24)&0xFF;
	int vram = (DC_vram_pointer&0xFFFFFF);
	sprintf(info,"pal :%d/32\nvram :%x/0x800000\n",palette,vram);
}

#endif



