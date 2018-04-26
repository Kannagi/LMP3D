#include <stdio.h>
#include <stdlib.h>



#include "LMP3D/LMP3D.h"

void LMP3D_Model_Init(LMP3D_Model *model)
{
    model->v = NULL;
    model->vt = NULL;
    model->vn = NULL;
    model->va = NULL;
    model->index = NULL;
    model->texture = NULL;
    model->texture_begin = NULL;
    model->name = NULL;
    model->anim = NULL;
    model->id = NULL;
    model->bones = NULL;
    model->nodes = NULL;

    model->nv = 0;
    model->nf = 0;
    model->ntexture = 0;
    model->time = 0;
    model->cadence = 0;
    model->play = 0;
    model->play_old = 0;
    model->boucle = 1;
    model->end = 0;
    model->nbones = 0;
    model->size = 0;

    model->defaut.n = 0;
    model->defaut.ntime = 0;
    model->defaut.ncadence = 0;


    model->position.x = 0;
	model->position.y = 0;
	model->position.z = 0;

    model->rotate.x = 0;
	model->rotate.y = 0;
	model->rotate.z = 0;

    model->scale.x = 1;
	model->scale.y = 1;
	model->scale.z = 1;

	model->test = 0;
}


