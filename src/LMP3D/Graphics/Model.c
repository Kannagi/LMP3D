#include <stdio.h>
#include <stdlib.h>



#include "LMP3D/LMP3D.h"

void LMP3D_Model_Init(LMP3D_Model *obj)
{
    obj->v = NULL;
    obj->vt = NULL;
    obj->vn = NULL;
    obj->va = NULL;
    obj->f = NULL;
    obj->groupe = NULL;
    obj->texture = NULL;
    obj->texture_begin = NULL;
    obj->name = NULL;
    obj->anim = NULL;
    obj->id = NULL;
    obj->bones = NULL;
    obj->nodes = NULL;

    obj->nv = 0;
    obj->nf = 0;
    obj->ngroupe = 0;
    obj->ntexture = 0;
    obj->uv = 1;
    obj->time = 0;
    obj->cadence = 0;
    obj->play = 0;
    obj->play_old = 0;
    obj->boucle = 1;
    obj->end = 0;
    obj->nbones = 0;

    obj->defaut.n = 0;
    obj->defaut.ntime = 0;
    obj->defaut.ncadence = 0;


    obj->position.x = 0;
	obj->position.y = 0;
	obj->position.z = 00;


	obj->test = 0;
}

void LMP3D_Model_Draw(LMP3D_Model *obj)
{

	//LMP3D_Animation3D(obj);

    LMP3D_Draw_Model_GL_VA(obj);

    return;

    if(obj->anim == NULL) return;

	int n = obj->play;
    if(obj->play != obj->play_old)
    {
        obj->play_old = obj->play;
        obj->cadence = 0;
        obj->time = 0;
    }

    obj->cadence++;
    if(obj->cadence >= obj->anim[n]->ncadence)
    {
        obj->cadence = 0;
        obj->time++;
    }

    obj->end = 0;
    if(obj->time >= obj->anim[n]->ntime)
    {
        obj->cadence = 0;
        obj->time = 0;
        obj->end = 1;
    }

}



