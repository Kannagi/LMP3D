
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AURAE/AURAE.h"

typedef struct
{
	unsigned char manufacturer,version,encoding,bitsPerPlane;
	unsigned short Xmin,Ymin,Xmax,Ymax,VertDPI,HorzDPI;
	unsigned char palette[48];
	unsigned char reserved,colorPlanes;
	unsigned short bytesPerPlaneLine,paltype,hScrSize,vScrSize;
	char pad[54];
}PCX_Header;

AURAE_Texture *AURAE_Load_pcx(char *filename,int offset,void *buffer,int size)
{
	void *file;
	file = AURAE_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	AURAE_fseek(file, offset, SEEK_SET);

	PCX_Header pcx;
	AURAE_fread(&pcx,1,sizeof(PCX_Header),file);

	if(pcx.manufacturer != 0x0A)
	{
		AURAE_fclose(file);
		return NULL;
	}

	AURAE_Texture *texture = NULL;
	texture = malloc(sizeof(AURAE_Texture));
	texture->pixel = NULL;
	texture->palette = NULL;

	texture->w = pcx.Xmax - pcx.Xmin + 1;
	texture->h = pcx.Ymax - pcx.Ymin + 1;

	texture->format = AURAE_FORMAT_8BPP;

	AURAE_Texture_Format_Init(texture);
	texture->pixel = malloc(texture->size);

	int i,pal1,pal2,l,psize = texture->size+0x301;

	//printf("%d\n",psize);
	unsigned char *buf = malloc(psize+1);
	AURAE_fread(buf,1,psize,file);

	AURAE_fclose(file);

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

	free(buf);

	AURAE_Texture_Format_Convert(texture);

	return texture;
}
