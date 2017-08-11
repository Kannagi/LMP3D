#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

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
    LMP3D_Texture *image;
    for(i = 0;i < obj->ntexture;i++)
    {
        sprintf(string,"%s%s",folder,obj->name[i]);
        image = LMP3D_Load_Texture(string);
        LMP3D_Texture_Upload(image);
        obj->texture[i] = image->id;
        LMP3D_Texture_Free(image);
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
