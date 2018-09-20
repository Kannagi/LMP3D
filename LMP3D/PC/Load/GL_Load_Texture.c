#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PC


void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
			texture->psm = 6409;
		break;

		case LMP3D_FORMAT_LUMA:
			texture->psm = 6410;
		break;

		case LMP3D_FORMAT_RGB888:
			texture->psm = 6407;
		break;

		case LMP3D_FORMAT_RGBA8888:
			texture->psm =  6408;
		break;

		case LMP3D_FORMAT_RGB555:

		break;

		case LMP3D_FORMAT_RGBA1555:

		break;

		case LMP3D_FORMAT_8BPP:
			texture->psm = 6409;
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void LMP3D_Texture_Convert_Internal(LMP3D_Texture *texture)
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
		break;

		case LMP3D_FORMAT_RGBA1555:
		break;

		case LMP3D_FORMAT_8BPP:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_RGBA8888);
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short LMP3D_Convert_Pixel(unsigned short pixel)
{
	return pixel;
}

#endif

