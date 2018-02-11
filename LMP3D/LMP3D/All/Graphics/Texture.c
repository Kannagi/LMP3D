#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"

int LMP3D_Texture_Palette_Get(LMP3D_Texture *texture,unsigned char *palette)
{
    int i,l;
    unsigned char *pixel = texture->pixel;
    unsigned char r,g,b;
    int taille = texture->size;
    int n = 0,black = 0;
    //printf("%d %d = %d octet\n",image->w,image->h,taille);

	if( ! ( (texture->format == LMP3D_FORMAT_RGB888) || (texture->format == LMP3D_FORMAT_RGBA8888) ) ) return 0;

    for(i = 0;i < 0x300;i++)
        palette[i] = 0;

    for(i = 0;i < taille;i += texture->pixelsize)
    {
    	r = pixel[i+0];
    	g = pixel[i+1];
    	b = pixel[i+2];

        for(l = 0;l < 0x300;l+=3)
        {
            if(palette[l+0] == r && palette[l+1] == g && palette[l+2] == b)
            {
                break;
            }else
            {
                if(n < 0x300)
				{
					palette[n+0] = r;
					palette[n+1] = g;
					palette[n+2] = b;
				}
				n +=3;

				if(n >= 0x303) return n;
            }
        }

        if(black == 0 && 0 == r && 0 == g && 0 == b)
        {
        	if(n < 0x300)
        	{
        		palette[n+0] = r;
				palette[n+1] = g;
				palette[n+2] = b;
        	}

            n +=3;
            black = 1;
            if(n >= 0x303) return n;
        }

    }

    return n;
}

int LMP3D_Texture_Convert(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,j,ncolor,n,r,g,b,a;
	unsigned char palette[0x300];
	void *pixel = NULL;
	unsigned char *cpixel;
	unsigned short *spixel;

	if( ! ( (texture->format == LMP3D_FORMAT_RGB888) || (texture->format == LMP3D_FORMAT_RGBA8888) ) ) return 0;

	ncolor = LMP3D_Texture_Palette_Get(texture,palette);
	printf("ncolor : %d\n",ncolor);

	if( (psmfinal == LMP3D_FORMAT_8BPP ) && (ncolor <= 0x300) )
	{
		n = texture->w*texture->h;
		cpixel = pixel = malloc(n);
		a = 0;
		for(i = 0;i < n;i++)
		{
			r = texture->pixel[l+0];
			g = texture->pixel[l+1];
			b = texture->pixel[l+2];

			for(j = 0;j < ncolor;j+=3)
            {
                if(palette[j+0] == r && palette[j+1] == g && palette[j+2] == b)
                    break;
            }

			cpixel[i] = j/3;
			l += texture->pixelsize;
		}

		texture->palette = malloc(ncolor);

		for(i = 0;i < ncolor;i++)
			texture->palette[i] = palette[i];

	}

	if(psmfinal == LMP3D_FORMAT_RGBA1555 || psmfinal == LMP3D_FORMAT_RGB555 )
	{
		n = texture->w*texture->h*2;
		spixel = pixel = malloc(n);
		a = 0;

		n = n>>1;
		for(i = 0;i < n;i++)
		{
			r = texture->pixel[l+0]>>3;
			g = texture->pixel[l+1]>>3;
			b = texture->pixel[l+2]>>3;
			if(texture->format == LMP3D_FORMAT_RGBA8888) a = (texture->pixel[l+3] != 0)<<15;
			spixel[i] = (r +(g<<5) + (b<<10) )| a;
			l += texture->pixelsize;
		}
	}

	if(pixel == NULL) return 0;


	free(texture->pixel);
	texture->pixel = pixel;
	texture->psm = 2;


	texture->format = psmfinal;
	LMP3D_Texture_Format_Init(texture);


	return 1;

}

void LMP3D_Texture_Format_Init(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h;
		break;

		case LMP3D_FORMAT_LUMA:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h;
		break;

		case LMP3D_FORMAT_RGB888:
			texture->pixelsize = 3;
			texture->bpp = 24;
			texture->size = texture->w*texture->h*3;
		break;


		case LMP3D_FORMAT_RGBA8888:
			texture->pixelsize = 4;
			texture->bpp = 32;
			texture->size = (texture->w*texture->h)<<2;
		break;


		case LMP3D_FORMAT_RGB555:
			texture->pixelsize = 2;
			texture->bpp = 16;
			texture->size = (texture->w*texture->h)<<1;
		break;

		case LMP3D_FORMAT_RGBA1555:
			texture->pixelsize = 2;
			texture->bpp = 16;
			texture->size = (texture->w*texture->h)<<1;
		break;

		case LMP3D_FORMAT_8BPP:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h;
		break;

		case LMP3D_FORMAT_4BPP:
			texture->pixelsize = -1;
			texture->bpp = 4;
			texture->size = (texture->w*texture->h)>>1;
		break;

		case LMP3D_FORMAT_2BPP:
			texture->pixelsize = -2;
			texture->bpp = 2;
			texture->size = (texture->w*texture->h)>>2;
		break;

		default:
			printf("Warning format unknown!\n");
			texture->pixelsize = 3;
			texture->bpp = 24;
			texture->size = texture->w*texture->h*3;
		break;
	}
}


void LMP3D_Texture_Free_RAM(LMP3D_Texture *image)
{
    if(image->pixel != NULL) free(image->pixel);
    if(image->palette != NULL) free(image->palette);
    if(image != NULL) free(image);

    image->pixel = NULL;
    image->palette = NULL;
    image = NULL;
}

void LMP3D_Texture_Free_Pixel(LMP3D_Texture *image)
{
    if(image->pixel != NULL) free(image->pixel);
    if(image->palette != NULL) free(image->palette);

    image->pixel = NULL;
    image->palette = NULL;
}

