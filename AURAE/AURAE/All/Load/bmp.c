#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AURAE/AURAE.h"

typedef struct
{
	unsigned short signature;
	unsigned int sizefile;
	unsigned short reserved[2];
	unsigned int offset,BITMAPINFOHEADER,width,height;
	unsigned short nplanes,nbits;
	unsigned int compression,size,hpixel,vpixel,noclors,ncolorsi;
}BMP_Header;

AURAE_Texture *AURAE_Load_bmp(char *filename,int offset,void *buffer,int size)
{
	void *file;
	file = AURAE_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	AURAE_fseek(file, offset, SEEK_SET);

	BMP_Header bmp;
	AURAE_fread(&bmp.signature,1,sizeof(unsigned short),file);

	if(bmp.signature != 0x4D42)
	{
		AURAE_fclose(file);
		return NULL;
	}

	AURAE_fread(&bmp.sizefile,1,sizeof(BMP_Header)-2,file);

	AURAE_Texture *texture = NULL;
	texture = malloc(sizeof(AURAE_Texture));
	texture->pixel = NULL;
	texture->palette = NULL;

	texture->w = bmp.width;
	texture->h = bmp.height;

	if(bmp.nbits == 8)
		texture->format = AURAE_FORMAT_8BPP;

	if(bmp.nbits == 16)
		texture->format = AURAE_FORMAT_RGBA1555;

	if(bmp.nbits == 24)
		texture->format = AURAE_FORMAT_RGB888;

	if(bmp.nbits == 32)
		texture->format = AURAE_FORMAT_RGBA8888;

	AURAE_Texture_Format_Init(texture);
	texture->pixel = malloc(texture->size);

	AURAE_fseek(file, bmp.offset, SEEK_SET);

	if(bmp.nbits == 8)
	{
		texture->palsize = 0x300;
		texture->palette = malloc(0x300);
		AURAE_fread(texture->palette,1,0x300,file);
	}

	int i,x,y,pitch,n = texture->w*texture->pixelsize;
	if(bmp.nbits >= 24)
	{
		unsigned char *buf = malloc(texture->size);
		AURAE_fread(buf,1,texture->size,file);
		int k = 0;

		for(y = texture->h-1;y > 0;y--)
		{
			pitch = y*n;
			for(x = 0;x < n;x += texture->pixelsize )
			{
				i = x + pitch;
				texture->pixel[i+2]= buf[k+0];
				texture->pixel[i+1]= buf[k+1];
				texture->pixel[i+0]= buf[k+2];
				k+=3;

				if(bmp.nbits == 32)
				{
					texture->pixel[i+3]= buf[k];
					k++;
				}
			}
		}

		free(buf);

	}else
		AURAE_fread(texture->pixel,1,texture->size,file);

	AURAE_fclose(file);
	AURAE_Texture_Format_Convert(texture);

	return texture;
}

