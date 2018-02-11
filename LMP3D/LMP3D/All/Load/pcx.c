
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

typedef struct
{
    unsigned char manufacturer,version,encoding,bitsPerPlane;
    unsigned short Xmin,Ymin,Xmax,Ymax,VertDPI,HorzDPI;
    unsigned char palette[48];
    unsigned char reserved,colorPlanes;
    unsigned short bytesPerPlaneLine,paltype,hScrSize,vScrSize;
    char pad[54];
}PCX_Header;

LMP3D_Texture *LMP3D_Load_pcx(const char *filename,int offset,void *buffer)
{
	FILE *file;
	file = fopen(filename,"rb");

	if(file == NULL) return NULL;

	fseek(file, offset, SEEK_SET);

	PCX_Header pcx;
	fread(&pcx,1,sizeof(PCX_Header),file);

	if(pcx.manufacturer != 0x0A)
	{
		fclose(file);
		return NULL;
	}

	LMP3D_Texture *texture = NULL;
	texture = malloc(sizeof(LMP3D_Texture));
	texture->pixel = NULL;
	texture->palette = NULL;

	texture->w = pcx.Xmax - pcx.Xmin + 1;
	texture->h = pcx.Ymax - pcx.Ymin + 1;

	texture->format = LMP3D_FORMAT_8BPP;

	LMP3D_Texture_Format_Init(texture);
	texture->pixel = malloc(texture->size);

	int i,pal1,pal2,l;
	for(i = 0;i<texture->size;)
	{
		pal1 = fgetc(file);
		if(pal1 <= 0xC0)
		{
			texture->pixel[i] = pal1;
			i++;
		}else
		{
			pal2 = fgetc(file);
			pal1 &= 0x3F;
			for(l = 0;l < pal1;l++)
			{
				texture->pixel[i] = pal2;
				i++;
			}
		}
	}

	pal1 = fgetc(file);
	if(pal1 != 0x0C)
	{
		fclose(file);
		return texture;
	}

	texture->palsize = 0x300;
	texture->palette = malloc(0x300);
	fread(texture->palette,1,0x300,file);

	fclose(file);
	LMP3D_Texture_Format_Convert(texture);

	return texture;
}
