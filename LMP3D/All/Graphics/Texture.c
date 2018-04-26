#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"

int LMP3D_Texture_Palette_Get(LMP3D_Texture *texture,unsigned char *palette)
{
    int i,l;
    unsigned char *pixel = texture->pixel;
    unsigned char r,g,b;
    int taille = texture->size;
    int n = 0,black = 0,pal;
    //printf("%d %d = %d octet\n",image->w,image->h,taille);

	if( ! ( (texture->format == LMP3D_FORMAT_RGB888) || (texture->format == LMP3D_FORMAT_RGBA8888) ) ) return 0;

    for(i = 0;i < 0x300;i++)
        palette[i] = 0;

    for(i = 0;i < taille;i += texture->pixelsize)
    {
    	r = pixel[i+0];
    	g = pixel[i+1];
    	b = pixel[i+2];


		pal = 1;
        for(l = 0;l < 0x300;l+=3)
        {
            if(palette[l+0] == r && palette[l+1] == g && palette[l+2] == b)
            {
            	pal = 0;
            	break;
            }
        }

        if(pal == 1)
		{
			if(n < 0x300)
			{
				palette[n+0] = r;
				palette[n+1] = g;
				palette[n+2] = b;
			}
			n +=3;
			if(n >= 0x303) return n;
		}else
		{
			if(black == 0 && r == 0 && g == 0 && b == 0)
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


    }

    return n;
}

int LMP3D_Texture_Convert(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,j,ncolor = 0,n,r,g,b;
	unsigned char palette[0x300];
	void *pixel = NULL;
	unsigned char *cpixel;
	unsigned short *spixel;

	if( ! ( (texture->format == LMP3D_FORMAT_RGB888) || (texture->format == LMP3D_FORMAT_RGBA8888) ) ) return 0;

	if( (psmfinal == LMP3D_FORMAT_8BPP ) | (psmfinal == LMP3D_FORMAT_4BPP ) )
	{
		ncolor = LMP3D_Texture_Palette_Get(texture,palette);
		//printf("ncolor : %d\n",ncolor/3);
	}

	if( (psmfinal == LMP3D_FORMAT_8BPP ) && (ncolor <= 0x300) )
	{
		n = texture->w*texture->h;
		cpixel = pixel = malloc(n);
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
		texture->palsize = ncolor;

		texture->palette = malloc(ncolor);

		for(i = 0;i < ncolor;i++)
			texture->palette[i] = palette[i];

	}

	if(psmfinal == LMP3D_FORMAT_RGBA1555 || psmfinal == LMP3D_FORMAT_RGB555 || psmfinal == LMP3D_FORMAT_RGB565 )
	{
		n = texture->w*texture->h*2;
		spixel = pixel = malloc(n);

		n = n>>1;
		if(psmfinal == LMP3D_FORMAT_RGB565)
		{
			for(i = 0;i < n;i++)
			{
				r = texture->pixel[l+0]>>3;
				g = texture->pixel[l+1]>>2;
				b = texture->pixel[l+2]>>3;
				spixel[i] = (r +(g<<5) + (b<<11) );
				l += texture->pixelsize;
			}
		}else
		{
			for(i = 0;i < n;i++)
			{
				r = texture->pixel[l+0]>>3;
				g = texture->pixel[l+1]>>3;
				b = texture->pixel[l+2]>>3;

				if(texture->format == LMP3D_FORMAT_RGBA8888)
				{
					if(texture->pixel[l+3] < 255)
						spixel[i] = 0 ;
					else
						spixel[i] = r +(g<<5) + (b<<10) + (1<<15) ;
				}else
					spixel[i] = (r +(g<<5) + (b<<10) );

				l += texture->pixelsize;
			}
		}
	}

	if(psmfinal == LMP3D_FORMAT_BGRA1555 || psmfinal == LMP3D_FORMAT_BGR555 || psmfinal == LMP3D_FORMAT_BGR565)
	{
		n = texture->w*texture->h*2;
		spixel = pixel = malloc(n);

		n = n>>1;

		if(psmfinal == LMP3D_FORMAT_BGR565)
		{
			for(i = 0;i < n;i++)
			{
				r = texture->pixel[l+2]>>3;
				g = texture->pixel[l+1]>>2;
				b = texture->pixel[l+0]>>3;

				spixel[i] = (r +(g<<5) + (b<<11) );
				l += texture->pixelsize;
			}
		}else
		{
			for(i = 0;i < n;i++)
			{
				r = texture->pixel[l+2]>>3;
				g = texture->pixel[l+1]>>3;
				b = texture->pixel[l+0]>>3;

				if(texture->format == LMP3D_FORMAT_RGBA8888)
				{
					if(texture->pixel[l+3] < 255)
						spixel[i] = 0;
					else
						spixel[i] = r +(g<<5) + (b<<10) + (1<<15) ;
				}else
					spixel[i] = (r +(g<<5) + (b<<10) );

				l += texture->pixelsize;
			}
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

int LMP3D_Texture_Convert_Pal(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,n,r,g,b;//,ncolor = 0;
	void *pixel = NULL;
	//unsigned char *cpixel;
	unsigned short *spixel;

	if(texture->palette == NULL) return 0;

	n = texture->palsize/3;

	if(psmfinal == LMP3D_FORMAT_RGBA1555 || psmfinal == LMP3D_FORMAT_RGB555 || psmfinal == LMP3D_FORMAT_RGB565 )
	{
		spixel = pixel = malloc(n);

		if(psmfinal == LMP3D_FORMAT_RGB565)
		{
			for(i = 0;i < n;i++)
			{
				r = texture->palette[l+0]>>3;
				g = texture->palette[l+1]>>2;
				b = texture->palette[l+2]>>3;


				spixel[i] = (r +(g<<5) + (b<<11) );
				l += 3;
			}
		}else
		{
			for(i = 0;i < n;i++)
			{
				r = texture->palette[l+0]>>3;
				g = texture->palette[l+1]>>3;
				b = texture->palette[l+2]>>3;

				spixel[i] = (r +(g<<5) + (b<<10) );
				if(spixel[i] == 0) spixel[i] = 1;
				if(spixel[i] == 0x7C1F) spixel[i] = 0;

				l += 3;
			}
		}
	}

	if(psmfinal == LMP3D_FORMAT_BGRA1555 || psmfinal == LMP3D_FORMAT_BGR555 || psmfinal == LMP3D_FORMAT_BGR565)
	{
		spixel = pixel = malloc(n);

		if(psmfinal == LMP3D_FORMAT_BGR565)
		{
			for(i = 0;i < n;i++)
			{
				r = texture->palette[l+2]>>3;
				g = texture->palette[l+1]>>2;
				b = texture->palette[l+0]>>3;

				spixel[i] = (r +(g<<5) + (b<<11) );
				if(spixel[i] == 0x7C1F) spixel[i] = 0;
				l += 3;
			}
		}else
		{
			for(i = 0;i < n;i++)
			{
				r = texture->palette[l+2]>>3;
				g = texture->palette[l+1]>>3;
				b = texture->palette[l+0]>>3;

				spixel[i] = (r +(g<<5) + (b<<10) );
				if(spixel[i] == 0) spixel[i] = 1;
				if(spixel[i] == 0x7C1F) spixel[i] = 0;

				l += 3;
			}
		}

	}

	free(texture->palette);
	texture->palette = pixel;

	return 1;

}

void LMP3D_Texture_Format_Init(LMP3D_Texture *texture)
{
	int inv = texture->format&0x80;
	texture->format &= 0x3F;

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


	if(inv == 0x80) texture->format |=0x80;

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

