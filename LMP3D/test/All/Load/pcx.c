
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

LMP3D_Texture *LMP3D_Load_pcx(char *filename,int offset,void *buffer,int size)
{
	void *file;
	file = LMP3D_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	LMP3D_fseek(file, offset, SEEK_SET);

	PCX_Header pcx;
	LMP3D_fread(&pcx,1,sizeof(PCX_Header),file);

	if(pcx.manufacturer != 0x0A)
	{
		LMP3D_fclose(file);
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

	int i,pal1,pal2,l,psize = texture->size+0x301;

	//printf("%d\n",psize);
	unsigned char buf[0x60000];
	LMP3D_fread(buf,1,psize,file);

	LMP3D_fclose(file);

	int k = 0;
	for(i = 0;i<texture->size;)
	{
		pal1 = buf[k++];
		if(pal1 <= 0xC0)
		{
			texture->pixel[i] = pal1;
			i++;
		}else
		{
			pal2 = buf[k++];
			pal1 &= 0x3F;
			for(l = 0;l < pal1;l++)
			{
				texture->pixel[i] = pal2;
				i++;
			}
		}

	}

	pal1 = buf[k++];
	if(pal1 == 0x0C)
	{
		texture->palsize = 0x300;
		texture->palette = malloc(0x300);
		memcpy(texture->palette,&buf[k],0x300);
	}

	//free(buf);

	LMP3D_Texture_Format_Convert(texture);

	return texture;
}
