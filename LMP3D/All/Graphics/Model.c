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
//	model->name = NULL;
/*
	model->anim = NULL;
	model->id = NULL;
	model->bones = NULL;
	model->nodes = NULL;
*/
	model->nv = 0;
	model->nf = 0;
	model->ntexture = 0;
	/*
	model->time = 0;
	model->cadence = 0;
	model->play = 0;
	model->play_old = 0;
	model->boucle = 1;
	model->end = 0;
	model->nbones = 0;*/
	model->size = 0;
/*
	model->defaut.n = 0;
	model->defaut.ntime = 0;
	model->defaut.ncadence = 0;
*/
	model->nbones = 0;

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

int LMP3D_Anim_Play(LMP3D_Anim *anim,int cadence,int n)
{
	if(anim->act != anim->old)
	{
		anim->itmp = 0;
		anim->i = 0;
		anim->end = 0;
		anim->old = anim->act;
		return 2;
	}

	anim->itmp++;
	if(anim->itmp >= cadence)
	{
		anim->itmp = 0;
		anim->i++;
	}

	anim->end = 0;
	if(anim->i >= n)
	{
		anim->i = 0;
		anim->itmp = 0;
		anim->end = 1;
		return 1;
	}

	return 0;
}

void LMP3D_Anim_Init(LMP3D_Anim *anim)
{
	anim->itmp = 0;
	anim->i = 0;
	anim->end = 0;
	anim->act = 0;
	anim->old = 0;
}

void LMP3D_Convert_Model_Index32ToIndex16(LMP3D_Model *model)
{
	unsigned int i;

	if(model->flag & LMP3D_MODEL_INDEX_U32)
	{
		unsigned int *i_index = model->index;
		unsigned short *index = malloc(model->nf*3  * sizeof(unsigned short));
		for(i = 0;i < model->nf*3;i++)
		{
			index[i] = i_index[i];
		}

		free(i_index);
		model->index = index;
	}

	model->flag ^= LMP3D_MODEL_INDEX_U32;
}

void LMP3D_Convert_Model_Index16ToIndex32(LMP3D_Model *model)
{
	unsigned int i;

	if(!(model->flag & LMP3D_MODEL_INDEX_U32))
	{
		unsigned int *i_index = malloc(model->nf*3  * sizeof(unsigned int));
		unsigned short *index = model->index;
		for(i = 0;i < model->nf*3;i++)
		{
			i_index[i] = index[i];
		}

		free(index);
		model->index = i_index;
	}

	model->flag |= LMP3D_MODEL_INDEX_U32;
}

void LMP3D_Model_Free(LMP3D_Model *model)
{
	int i;
	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);
	if(model->va != NULL) free(model->va);

	if(model->index != NULL) free(model->index);

	if(model->ntexture > 0)
	{
		for(i = 0;i < model->ntexture;i++)
		{
			//free(model->name[i]);
			LMP3D_Texture_Free(model->texture[i]);
		}

		free(model->texture);
		free(model->texture_begin);
		//free(model->name);
	}

	if(model->ngroup > 0)
	{
		free(model->groupvertex);
		free(model->groupface);
	}

	free(model);

	model = NULL;
}
