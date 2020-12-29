#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1
#include "LMP3D/PS1/PS1.h"


void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:
		break;

		case LMP3D_FORMAT_RGBA8888:
		break;

		case LMP3D_FORMAT_RGB555:
			texture->psm = PS1_PSM16;
		break;

		case LMP3D_FORMAT_RGBA1555:
			texture->psm = PS1_PSM16;
		break;

		case LMP3D_FORMAT_8BPP:
			texture->psm = PS1_PSM8;
		break;

		case LMP3D_FORMAT_4BPP:
			texture->psm = PS1_PSM4;
		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void LMP3D_Texture_Convert_Internal(LMP3D_Texture *texture)
{
	int ncolor;
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_RGB555);
		break;

		case LMP3D_FORMAT_RGBA8888:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_RGBA1555);
		break;

		case LMP3D_FORMAT_RGB555:
			texture->psm = PS1_PSM16;
		break;

		case LMP3D_FORMAT_RGBA1555:
			texture->psm = PS1_PSM16;
		break;

		case LMP3D_FORMAT_8BPP:
			ncolor = LMP3D_Texture_Palette_Count_Get(texture);

			if(ncolor <= 16) LMP3D_Texture_Convert(texture,LMP3D_FORMAT_4BPP);

			LMP3D_Texture_Convert_Pal(texture,LMP3D_FORMAT_RGBA1555);
		break;

		case LMP3D_FORMAT_4BPP:
			LMP3D_Texture_Convert_Pal(texture,LMP3D_FORMAT_RGBA1555);
		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short LMP3D_Convert_Pixel(unsigned short pixel)
{
	if(pixel == 0) pixel = 1;
	if(pixel == 0x7C1F) pixel = 0;

	return pixel;
}

#endif

