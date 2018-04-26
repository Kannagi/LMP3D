#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

/*
Portable Network Graphics (.png)
Windows Bitmap (.bmp)
ZSoft Paintbrush PCX (.pcx)

Basic Model (.bcm)

*/


LMP3D_Texture* LMP3D_Load_Texture(char *address,int offset,void *buffer,int size)
{
    LMP3D_Texture *texture;

	texture = LMP3D_Load_bmp(address,offset,buffer,size);
	if(texture != NULL) return texture;

	texture = LMP3D_Load_pcx(address,offset,buffer,size);
	if(texture != NULL) return texture;

	texture = LMP3D_Load_png(address,offset,buffer,size);
	if(texture != NULL) return texture;

	printf("Texture loading failed : %s\n",address);

    return NULL;
}


void LMP3D_Load_Texture_Array(LMP3D_Model *obj,char *filename,char *folder,int offset,void *buffer,int size)
{
    int i;
    char string[100];

    LMP3D_TAR tar;

    for(i = 0;i < obj->ntexture;i++)
    {
        sprintf(string,"%s%s",folder,obj->name[i]);

        if(offset != 0)
        {
        	LMP3D_Tar(&tar,filename,string,LMP3D_TAR_OFFSET,buffer,size);
			obj->texture[i] = LMP3D_Load_Texture(filename,tar.offset,buffer,tar.size);
        }else
        {
        	obj->texture[i] = LMP3D_Load_Texture(string,0,NULL,0);
        }

        LMP3D_Texture_Upload(obj->texture[i]);
        LMP3D_Texture_Free_Pixel(obj->texture[i]);
    }
}

LMP3D_Model *LMP3D_Load_Model(char *filename,int offset,void *buffer,int size)
{
	LMP3D_Model *model;

	model = LMP3D_Load_bcm(filename,offset,buffer,size);
	if(model != NULL) return model;

	return NULL;
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
