#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"


void AURAE_Texture_Format_Convert(AURAE_Texture *texture)
{
	int format = texture->format&0x3F;
	switch (format)
	{
		case AURAE_FORMAT_LUM:
		break;

		case AURAE_FORMAT_LUMA:
		break;

		case AURAE_FORMAT_RGB888:
		break;

		case AURAE_FORMAT_RGBA8888:
		break;

		case AURAE_FORMAT_RGB555:
			texture->psm = TA_TEXTURE_RGB1555;
		break;

		case AURAE_FORMAT_RGBA1555:
			texture->psm = TA_TEXTURE_RGB1555;
		break;

		case AURAE_FORMAT_8BPP:
			texture->psm = TA_TEXTURE_8BPP;
		break;

		case AURAE_FORMAT_4BPP:
			texture->psm = TA_TEXTURE_4BPP;
		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}







void DC_twiddle_encode(AURAE_Texture *texture);


void AURAE_Texture_Convert_Internal(AURAE_Texture *texture)
{
	int ncolor;
	switch (texture->format)
	{
		case AURAE_FORMAT_LUM:
		break;

		case AURAE_FORMAT_LUMA:
		break;

		case AURAE_FORMAT_RGB888:
			AURAE_Texture_Convert(texture,AURAE_FORMAT_BGR565);
		break;

		case AURAE_FORMAT_RGBA8888:
			AURAE_Texture_Convert(texture,AURAE_FORMAT_BGRA1555);
		break;

		case AURAE_FORMAT_RGB555:
		break;

		case AURAE_FORMAT_RGBA1555:
		break;

		case AURAE_FORMAT_8BPP:
			ncolor = AURAE_Texture_Palette_Count_Get(texture);
			DC_twiddle_encode(texture);

			if(ncolor <= 16) AURAE_Texture_Convert(texture,AURAE_FORMAT_4BPP);

			AURAE_Texture_Convert_Pal(texture,AURAE_FORMAT_BGRA1555);
		break;

		case AURAE_FORMAT_4BPP:
			AURAE_Texture_Convert_Pal(texture,AURAE_FORMAT_BGRA1555);
		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short AURAE_Convert_Pixel(unsigned short pixel)
{
	if(pixel == 0x7C1F) pixel = 0;
	else pixel |= 0x8000;

	return pixel;
}

void AURAE_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}



#endif

