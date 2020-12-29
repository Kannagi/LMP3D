#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>


#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"

static unsigned int PS1_vram_pointer = 0;

int PS1_vram_size(int width, int psm);


unsigned int PS1_vram_allocate(int width, int height, int psm)
{
	int size,tampon,sizebloc;

	size = PS1_vram_size(width,psm);
	sizebloc = size >>6;

	if(height == 1)
	{
		tampon = sizebloc +( (PS1_vram_pointer>>24)&0xFF);
		if(tampon >= 0x40) return - 1;

		sizebloc = 1;

		PS1_vram_pointer += 1<<24;
		PS1_vram_pointer = ( (PS1_vram_pointer&0xFFFFFF00) + ( (PS1_vram_pointer>>24)&0xFF ) ) | 0x80;
	}else
	{
		tampon = sizebloc +( (PS1_vram_pointer>>16)&0xFF);

		if(tampon <= 0x10)
		{
			PS1_vram_pointer += sizebloc<<16;
			PS1_vram_pointer = (PS1_vram_pointer&0xFFFFFF00) + ( (PS1_vram_pointer>>16)&0xFF );
		}else
		{
			PS1_vram_pointer += sizebloc<<8;
			PS1_vram_pointer = ( (PS1_vram_pointer&0xFFFFFF00) + ( (PS1_vram_pointer>>8)&0xFF) )| 0x40;
		}

	}

	return PS1_vram_pointer - sizebloc;
}

void PS1_vram_set(int address)
{
	PS1_vram_pointer = address;
}

int PS1_vram_get()
{
	return PS1_vram_pointer;
}

void PS1_vram_clear(void)
{
	PS1_vram_pointer = 0;
}

int PS1_vram_size(int width, int psm)
{
	int size = 0;

	// Texture storage size is in pixels/word
	switch (psm)
	{
		case PS1_PSM4:	size = width>>2; break;
		case PS1_PSM8:	size = width>>1; break;
		case PS1_PSM16:	size = width; break;

		default: return 0;
	}

	return size;
}

void LMP3D_VRAM_Set(unsigned int address)
{
	PS1_vram_pointer = address;
}

unsigned int LMP3D_VRAM_Get()
{
	return PS1_vram_pointer;
}

void LMP3D_VRAM_Info(char *info)
{
	int palette = (PS1_vram_pointer>>24)&0xFF;
	int vram1 = ( (PS1_vram_pointer>>16)&0xFF)<<6;
	int vram2 = ( (PS1_vram_pointer>>8)&0xFF)<<6;
	sprintf(info,"pal :%d/32\nvram1 :%d/1024\nvram2 :%d/1024\n",palette,vram1,vram2);
}

#endif

