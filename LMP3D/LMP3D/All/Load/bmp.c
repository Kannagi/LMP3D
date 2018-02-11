#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

typedef struct
{
    unsigned short signature;
    unsigned int sizefile;
    unsigned short reserved[2];
    unsigned int offset,BITMAPINFOHEADER,width,height;
    unsigned short nplanes,nbits;
    unsigned int compression,size,hpixel,vpixel,noclors,ncolorsi;
}BMP_Header;

LMP3D_Texture *LMP3D_Load_bmp(const char *filename,int offset,void *buffer)
{
	FILE *file;
	file = fopen(filename,"rb");

	if(file == NULL) return NULL;

	fseek(file, offset, SEEK_SET);

	BMP_Header bmp;
	fread(&bmp.signature,1,sizeof(unsigned short),file);


	if(bmp.signature != 0x4D42)
	{
		fclose(file);
		return NULL;
	}

	fread(&bmp.sizefile,1,sizeof(BMP_Header)-2,file);

	LMP3D_Texture *texture = NULL;
	texture = malloc(sizeof(LMP3D_Texture));
	texture->pixel = NULL;
	texture->palette = NULL;

	texture->w = bmp.width;
	texture->h = bmp.height;

	if(bmp.nbits == 8)
		texture->format = LMP3D_FORMAT_8BPP;

	if(bmp.nbits == 24)
		texture->format = LMP3D_FORMAT_RGB888;

	if(bmp.nbits == 32)
		texture->format = LMP3D_FORMAT_RGBA8888;

	LMP3D_Texture_Format_Init(texture);
	texture->pixel = malloc(texture->size);

	fseek(file, bmp.offset, SEEK_SET);

	if(bmp.nbits == 8)
	{
		texture->palsize = 0x300;
		texture->palette = malloc(0x300);
		fread(texture->palette,1,0x300,file);
	}

	int i,x,y,pitch,n = texture->w*texture->pixelsize;
	if(bmp.nbits >= 24)
	{
		for(y = texture->h-1;y > 0;y--)
		{
			pitch = y*n;
			for(x = 0;x < n;x += texture->pixelsize )
			{
				i = x + pitch;
				texture->pixel[i+2]= fgetc(file);
				texture->pixel[i+1]= fgetc(file);
				texture->pixel[i+0]= fgetc(file);

				if(bmp.nbits == 32)
				{
					texture->pixel[i+3]= fgetc(file);
				}
			}
		}

	}else
		fread(texture->pixel,1,texture->size,file);

	fclose(file);
	LMP3D_Texture_Format_Convert(texture);

	return texture;
}

