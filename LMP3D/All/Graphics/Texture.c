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

int LMP3D_Texture_Palette_Count_Get(LMP3D_Texture *texture)
{
    int i,l;
    unsigned char *pixel = texture->palette;
    unsigned char r,g,b;
    int taille = texture->palsize;
    int n = 0,pal;

	if(texture->palette == NULL) return 0;

	unsigned char palette[0x300];

    for(i = 0;i < 0x300;i++)
        palette[i] = 0;

    for(i = 0;i < taille;i += 3)
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
			palette[n+0] = r;
			palette[n+1] = g;
			palette[n+2] = b;
			n++;
		}
    }

    return n;
}

int LMP3D_Texture_Convert(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,j,ncolor = 0,n;
	unsigned char palette[0x300];
	void *pixel = NULL;
	unsigned char *cpixel,r,g,b;
	unsigned short *spixel;


	if( (texture->format == LMP3D_FORMAT_8BPP) && (psmfinal == LMP3D_FORMAT_4BPP ) )
	{
		n = (texture->w*texture->h);
		cpixel = pixel = malloc(n>>1);
		l = 0;
		for(i = 0;i < n;i +=2)
		{
			r = texture->pixel[i+0];
			b = texture->pixel[i+1];

			cpixel[l++] = r + (b<<4);
		}

		free(texture->pixel);
		texture->pixel = pixel;
		texture->format = psmfinal;

		LMP3D_Texture_Format_Init(texture);
		LMP3D_Texture_Format_Convert(texture);

		return 1;
	}

	if( (texture->format == LMP3D_FORMAT_8BPP) && (psmfinal == LMP3D_FORMAT_RGBA8888 ) )
	{
		n = texture->w*texture->h;
		cpixel = pixel = malloc(n*4);
		l = 0;
		int id;

		for(i = 0;i < n;i++)
		{
			id = texture->pixel[i];
			id = id*3;

			r = texture->palette[id+0];
			g = texture->palette[id+1];
			b = texture->palette[id+2];

			cpixel[l+0] = r;
			cpixel[l+1] = g;
			cpixel[l+2] = b;

			if( (r == 0xFF) && (g == 0x00) && (b == 0xFF) )
				cpixel[l+3] = 0;
			else
				cpixel[l+3] = 255;

			l += 4;
		}

		free(texture->pixel);
		texture->pixel = pixel;
		texture->format = psmfinal;

		LMP3D_Texture_Format_Init(texture);
		LMP3D_Texture_Format_Convert(texture);

		return 1;
	}

	if( (texture->format == LMP3D_FORMAT_8BPP) && (psmfinal == LMP3D_FORMAT_BGRA1555 ) )
	{
		n = texture->w*texture->h;
		spixel = cpixel = pixel = malloc(n<<1);
		l = 0;
		int id;

		for(i = 0;i < n;i++)
		{
			id = texture->pixel[i];
			id = id*3;

			r = texture->palette[id+0]>>3;
			g = texture->palette[id+1]>>3;
			b = texture->palette[id+2]>>3;

			spixel[l] = (b +(g<<5) + (r<<10)) ;
			spixel[l] = LMP3D_Convert_Pixel(spixel[l]);
			l++;
		}

		free(texture->pixel);
		texture->pixel = pixel;
		texture->format = psmfinal;

		LMP3D_Texture_Format_Init(texture);
		LMP3D_Texture_Format_Convert(texture);

		return 1;
	}

	if( ! ( (texture->format == LMP3D_FORMAT_RGB888) || (texture->format == LMP3D_FORMAT_RGBA8888) ) ) return 0;

	if( (psmfinal == LMP3D_FORMAT_8BPP ) | (psmfinal == LMP3D_FORMAT_4BPP ) )
	{
		ncolor = LMP3D_Texture_Palette_Get(texture,palette);
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
					if(texture->pixel[l+3] < 128)
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
					if(texture->pixel[l+3] < 128)
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
	texture->format = psmfinal;

	LMP3D_Texture_Format_Init(texture);

	LMP3D_Texture_Format_Convert(texture);


	return 1;

}

int LMP3D_Texture_Convert_Pal(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,n;//,ncolor = 0;
	void *pixel = NULL;
	unsigned char r,g,b;
	unsigned short *spixel;

	if(texture->palette == NULL) return 0;

	n = LMP3D_Texture_Palette_Count_Get(texture);

	spixel = pixel = malloc(n<<1);

	if(psmfinal == LMP3D_FORMAT_RGBA1555 || psmfinal == LMP3D_FORMAT_RGB555 || psmfinal == LMP3D_FORMAT_RGB565 )
	{
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
				r = texture->palette[l+0];
				g = texture->palette[l+1];
				b = texture->palette[l+2];
				r = r>>3;
				g = g>>3;
				b = b>>3;
/*
				if(r == 255 && g == 0 && b == 255)
				{
					spixel[i] = 0x0;
				}else
				{
					r = r>>3;
					g = g>>3;
					b = b>>3;*/
					spixel[i] = (r +(g<<5) + (b<<10) );
				//}
				spixel[i] = LMP3D_Convert_Pixel(spixel[i]);

				l += 3;
			}
		}
	}

	if(psmfinal == LMP3D_FORMAT_BGRA1555 || psmfinal == LMP3D_FORMAT_BGR555 || psmfinal == LMP3D_FORMAT_BGR565)
	{
		if(psmfinal == LMP3D_FORMAT_BGR565)
		{
			for(i = 0;i < n;i++)
			{
				r = texture->palette[l+2]>>3;
				g = texture->palette[l+1]>>2;
				b = texture->palette[l+0]>>3;

				spixel[i] = (r +(g<<5) + (b<<11) );

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
				spixel[i] = LMP3D_Convert_Pixel(spixel[i]);

				l += 3;
			}
		}

	}

	free(texture->palette);
	texture->palette = pixel;
	texture->palsize =n<<1;

	return 1;

}

void LMP3D_Texture_Format_Init(LMP3D_Texture *texture)
{
	int format = texture->format&0x7F;

	switch (format)
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
			texture->palsize = 16*3;
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


void LMP3D_Texture_Free(LMP3D_Texture *image)
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

