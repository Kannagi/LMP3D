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

#endif

