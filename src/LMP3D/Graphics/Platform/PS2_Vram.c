#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include "LMP3D/LMP3D.h"

#include "LMP3D/Platform/PS2.h"

static int PS2_vram_pointer = 0;

int PS2_vram_allocate(int width, int height, int psm)
{

	int size;

	// Calculate the size and increment the pointer
	size = PS2_vram_size(width,height,psm);

	PS2_vram_pointer += size;

	// If the pointer overflows the vram size
	if (PS2_vram_pointer > 0X100000)
	{

		PS2_vram_pointer -= size;
		return -1;

	}

	return PS2_vram_pointer - size;

}

void PS2_vram_set(int address)
{

	PS2_vram_pointer = address;

}

int PS2_vram_get()
{
	return PS2_vram_pointer;

}

void PS2_vram_clear(void)
{

	PS2_vram_pointer = 0;

}


int PS2_vram_size(int width, int height, int psm, int alignment)
{

	int size;

	// Texture storage size is in pixels/word
	switch (psm)
	{

		case GS_PSMT4:		size = (width*height)>>3; break;
		case GS_PSMT8:		size = (width*height)>>2; break;
		case GS_PSMCT32:
		case GS_PSMCT24:
		case GS_PSMT8H:
		case GS_PSMT4HL:
		case GS_PSMT4HH:
		case GS_PSMZ32:
		case GS_PSMZ24:	size = width*height; break;
		case GS_PSMCT16:
		case GS_PSMCT16S:
		case GS_PSMZ16:
		case GS_PSMZ16S:	size = (width*height)>>1; break;
		default: return 0;

	}

	return size;

}

#endif
