#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"


void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	int format = texture->format&0x3F;
	switch (format)
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
			texture->psm = TA_TEXTURE_RGB1555;
		break;

		case LMP3D_FORMAT_RGBA1555:
			texture->psm = TA_TEXTURE_RGB1555;
		break;

		case LMP3D_FORMAT_8BPP:
			texture->psm = TA_TEXTURE_8BPP;
		break;

		case LMP3D_FORMAT_4BPP:
			texture->psm = TA_TEXTURE_4BPP;
		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}







void DC_twiddle_encode(LMP3D_Texture *texture);


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
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_BGR565);
		break;

		case LMP3D_FORMAT_RGBA8888:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_BGRA1555);
		break;

		case LMP3D_FORMAT_RGB555:
		break;

		case LMP3D_FORMAT_RGBA1555:
		break;

		case LMP3D_FORMAT_8BPP:
			ncolor = LMP3D_Texture_Palette_Count_Get(texture);
			DC_twiddle_encode(texture);

			if(ncolor <= 16) LMP3D_Texture_Convert(texture,LMP3D_FORMAT_4BPP);

			LMP3D_Texture_Convert_Pal(texture,LMP3D_FORMAT_BGRA1555);
		break;

		case LMP3D_FORMAT_4BPP:
			LMP3D_Texture_Convert_Pal(texture,LMP3D_FORMAT_BGRA1555);
		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short LMP3D_Convert_Pixel(unsigned short pixel)
{
	if(pixel == 0x7C1F) pixel = 0;
	else pixel |= 0x8000;

	return pixel;
}

void LMP3D_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}



#endif

