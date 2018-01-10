#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

/*
JPEG File Interchange Format (.jpg)
Portable Network Graphics (.png)
Truevision Targa (.tga)
Windows Bitmap (.bmp)
ZSoft Paintbrush PCX (.pcx)

Format 3D
.bcm



*/

int LMP3D_Load_Palette(LMP3D_Texture *texture,unsigned char *palette)
{
    int i,l;
    unsigned char *pixel = texture->pixel;
    int taille = texture->size;
    int n = 0,ok,black = 0;
    //printf("%d %d = %d octet\n",image->w,image->h,taille);

	if( ! ( (texture->pixelformat == LMP3D_FORMAT_RGB) || (texture->pixelformat == LMP3D_FORMAT_RGBA) ) ) return 0;

    for(i = 0;i < 0x300;i++)
        palette[i] = 0;

    for(i = 0;i < taille;i += texture->pixelsize)
    {
        ok = 0;
        for(l = 0;l < 0x300;l+=3)
        {
            if(palette[l+0] == pixel[i+0] && palette[l+1] == pixel[i+1] && palette[l+2] == pixel[i+2])
            {
                ok = 0;
                break;
            }else
            {
                ok = 1;
            }
        }

        if(black == 0 && 0 == pixel[i+0] && 0 == pixel[i+1] && 0 == pixel[i+2])
        {
        	ok = 1;
        	black = 1;
        }

        if(ok == 1)
        {
            palette[n+0] = pixel[i+0];
            palette[n+1] = pixel[i+1];
            palette[n+2] = pixel[i+2];
            n +=3;
            if(n >= 0x300) break;
        }
    }

    return n;
}


int LMP3D_Convert_Pixel(LMP3D_Texture *texture,int psmfinal)
{
	int i,l=0,j,ncolor,n,r,g,b,a;
	unsigned char palette[0x300];
	void *pixel = NULL;
	unsigned char *cpixel;
	unsigned short *spixel;

	if( ! ( (texture->pixelformat == LMP3D_FORMAT_RGB) || (texture->pixelformat == LMP3D_FORMAT_RGBA) ) ) return 0;

	ncolor = LMP3D_Load_Palette(texture,palette);
	printf("ncolor : %d\n",ncolor);

	if( (psmfinal & LMP3D_FORMAT_8BPP ) && (ncolor <= 0x300) )
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

            j = j/3;

			cpixel[i] = j;
			l += texture->pixelsize;
		}

		texture->palette = malloc(ncolor);

		for(i = 0;i < ncolor;i++)
			texture->palette[i] = palette[i];

	}

	if(psmfinal & LMP3D_FORMAT_RGBA16 || psmfinal & LMP3D_FORMAT_RGB15 )
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
			if(texture->pixelformat == LMP3D_FORMAT_RGBA) a = (texture->pixel[l+3] != 0)<<15;
			spixel[i] = (r) +(g<<5) + (b<<10) | a;
			l += texture->pixelsize;
		}
	}

	if(pixel == NULL) return 0;


	free(texture->pixel);
	texture->pixel = pixel;
	texture->psm = 2;


	texture->pixelformat = psmfinal;
	LMP3D_Format_Init(texture);


	return 1;

}

void LMP3D_Format_Init(LMP3D_Texture *texture)
{
	switch (texture->pixelformat)
	{
		case LMP3D_FORMAT_LUM:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;

		case LMP3D_FORMAT_LUMA:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;

		case LMP3D_FORMAT_RGB:
			texture->pixelsize = 3;
			texture->bpp = 24;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;


		case LMP3D_FORMAT_RGBA:
			texture->pixelsize = 4;
			texture->bpp = 32;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;


		case LMP3D_FORMAT_RGB15:
			texture->pixelsize = 2;
			texture->bpp = 16;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;

		case LMP3D_FORMAT_RGBA16:
			texture->pixelsize = 2;
			texture->bpp = 16;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;

		case LMP3D_FORMAT_8BPP:
			texture->pixelsize = 1;
			texture->bpp = 8;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;

		case LMP3D_FORMAT_4BPP:
			texture->pixelsize = -1;
			texture->bpp = 4;
			texture->size = (texture->w*texture->h*texture->pixelsize)>>1;
		break;

		case LMP3D_FORMAT_2BPP:
			texture->pixelsize = -2;
			texture->bpp = 2;
			texture->size = (texture->w*texture->h*texture->pixelsize)>>2;
		break;

		default:
			texture->pixelsize = 3;
			texture->bpp = 24;
			texture->size = texture->w*texture->h*texture->pixelsize;
		break;
	}
}

LMP3D_Texture* LMP3D_Load_Texture(const char *address)
{
    LMP3D_Texture *texture;

    texture = LMP3D_Load_png(address);
	if(texture != NULL) return texture;

    return NULL;
}


void LMP3D_Load_Texture_Array(LMP3D_Model *obj,char *folder)
{
    int i;
    char string[100];

    for(i = 0;i < obj->ntexture;i++)
    {
        sprintf(string,"%s%s",folder,obj->name[i]);
        obj->texture[i] = LMP3D_Load_Texture(string);
        if(obj->texture[i] != NULL)LMP3D_Texture_Upload(obj->texture[i]);
        LMP3D_Texture_Free_Pixel(obj->texture[i]);
    }
}


/*
MNS_Model3D *MNS_load_3D(char *nom_du_fichier)
{
    char ch[100];
    MNS_lecture_chaine_ext(nom_du_fichier,ch,20,'.');

    MNS_Model3D *model = NULL;

    if(strcmp(ch,"obj") == 0)
        model = MNS_load_3D_obj(nom_du_fichier);

    if(strcmp(ch,"smd") == 0)
        model = MNS_load_3D_smd(nom_du_fichier);

    if(strcmp(ch,"bcm") == 0)
        model = MNS_load_3D_bcm(nom_du_fichier);

    return model;
}




void MNS_free_anim(MNS_ANIM *anim)
{
    if(anim->skl != NULL) free(anim->skl);
    //if(anim->k != NULL)   free(anim->k);
    if(anim != NULL)      free(anim);
}


void MNS_free_3D(MNS_3D *obj)
{

    if(obj->v != NULL)      free(obj->v);
    if(obj->va != NULL)     free(obj->va);
    if(obj->vt != NULL)     free(obj->vt);
    if(obj->f != NULL)      free(obj->f);

    if(obj->id != NULL)     free(obj->id);
    if(obj->nodes != NULL)  free(obj->nodes);
    if(obj->bone != NULL)   free(obj->bone);
    if(obj->precl != NULL)  free(obj->precl);


    if(obj->texture.vt != NULL)free(obj->texture.vt);

    //printf("ok5.5\n");
    if(obj->texture.p != NULL) free(obj->texture.p);

    if(obj->type == 0)
    glDeleteLists(obj->list,1);

    if(obj->uv == 0)
    glDeleteTextures(obj->texture.n,obj->idt);

    if(obj->idt != NULL)    free(obj->idt);
    int i;

    if(obj->anim != NULL)
    for(i = 0;i<obj->nanim;i++)
    {

        if(obj->anim[i] != NULL);
        {
            if(i != 0)
            MNS_free_anim(obj->anim[i]);
        }

    }

    if(obj->anim != NULL) free(obj->anim);

    if(obj != NULL)         free(obj);
}


void MNS_free_image_3D(MNS_3D *obj)
{
    int i;
    if(obj->texture.image != NULL)
    {
        for(i = 0;i<obj->texture.n;i++)
        {
            if(obj->texture.image[i] != NULL);
            {
                MNS_free(obj->texture.image[i]);
            }
        }
    }

    if(obj->texture.image != NULL)
    free(obj->texture.image);
}

*/
