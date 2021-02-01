#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AURAE/AURAE.h"

#ifdef PLAYSTATION2
#include <kernel.h>
#include "AURAE/PS2/PS2.h"

void AURAE_Texture_Format_Convert(AURAE_Texture *texture)
{
	switch (texture->format)
	{
		case AURAE_FORMAT_LUM:
			texture->psm = GS_PSMT8;
		break;

		case AURAE_FORMAT_LUMA:
			texture->psm = GS_PSMT8;
		break;

		case AURAE_FORMAT_RGB888:
			texture->psm = GS_PSMCT24;
		break;

		case AURAE_FORMAT_RGBA8888:
			texture->psm =  GS_PSMCT32;
		break;

		case AURAE_FORMAT_RGB555:
			texture->psm =  GS_PSMCT16;
		break;

		case AURAE_FORMAT_RGBA1555:
			texture->psm =  GS_PSMCT16;
		break;

		case AURAE_FORMAT_8BPP:
			texture->psm = GS_PSMT8;
		break;

		case AURAE_FORMAT_4BPP:
			texture->psm = GS_PSMT4;
		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void PS2_Convert_Pal(AURAE_Texture *texture)
{
	unsigned short *palette = malloc(512);
	unsigned short *pal = texture->palette;

	int n = texture->palsize>>1,l,z;
	for(l = 0;l < n;l++)
	{
		z = l&0x07;
		z += (l&0x08)<<1;

		z += (l&0x10)>>1;
		z += (l&0xE0);

		palette[z] = pal[l];
	}

	free(texture->palette);
	texture->palette = palette;
	texture->palsize =256<<1;

}



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
			AURAE_Texture_Convert(texture,AURAE_FORMAT_RGB555);
		break;

		case AURAE_FORMAT_RGBA8888:
			AURAE_Texture_Convert(texture,AURAE_FORMAT_RGBA1555);
		break;

		case AURAE_FORMAT_RGB555:
		break;

		case AURAE_FORMAT_RGBA1555:
		break;

		case AURAE_FORMAT_8BPP:
			ncolor = AURAE_Texture_Palette_Count_Get(texture);

			if(ncolor <= 16) AURAE_Texture_Convert(texture,AURAE_FORMAT_4BPP);

			AURAE_Texture_Convert_Pal(texture,AURAE_FORMAT_RGBA1555);
			if(ncolor > 16)
				PS2_Convert_Pal(texture);
		break;

		case AURAE_FORMAT_4BPP:
			AURAE_Texture_Convert_Pal(texture,AURAE_FORMAT_RGBA1555);
			PS2_Convert_Pal(texture);
		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

	FlushCache(0);

}

unsigned short AURAE_Convert_Pixel(unsigned short pixel)
{

	if(pixel == 0) pixel = 1;
	if(pixel == 0x7C1F) pixel = 0;
	else
	pixel = pixel | 0x8000;

	return pixel;
}

void AURAE_File_Path(char *inpath,char *outpath)
{
/*
	int i,n = strlen(inpath);

	for(i =0;i < n;i++)
	{
		if(inpath[i] >= 'a' && inpath[i] <= 'z')
			inpath[i] += 'A' -'a';
	}

	sprintf(outpath,"cdrom:%s;1",inpath);*/

	strcpy(outpath,inpath);
}

#endif

