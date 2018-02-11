#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION2
#include "LMP3D/PS2/PS2.h"

void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
			texture->psm = GS_PSMT8;
		break;

		case LMP3D_FORMAT_LUMA:
			texture->psm = GS_PSMT8;
		break;

		case LMP3D_FORMAT_RGB888:
			texture->psm = GS_PSMCT24;
		break;

		case LMP3D_FORMAT_RGBA8888:
			texture->psm =  GS_PSMCT32;
		break;

		case LMP3D_FORMAT_RGB555:

		break;

		case LMP3D_FORMAT_RGBA1555:

		break;

		case LMP3D_FORMAT_8BPP:
			texture->psm = GS_PSMT8;
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

#endif

