#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION2

#include "LMP3D/PS2/PS2.h"

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	int tv,l,l2,i;
	model->nv = model->nf*3;

	u16 *v  = malloc(model->nf*3 * 8 * sizeof(u16));
	//obj->v  = malloc(obj->nf*3 * 8 * sizeof(float));

	float *mv = model->v;
	float *mvt = model->vt;
	for(i = 0;i < model->nf*3;i++)
	{
		tv = i*8;
		l = model->f[i]*3;
		l2 = model->f[i]*2;
/*
		obj->v[tv+0] = obj->va[l+0];
		obj->v[tv+1] = obj->va[l+1];
		obj->v[tv+2] = obj->va[l+2];
		obj->v[tv+3] = 1.0f;

		obj->v[tv+4] = obj->vt[l2+0];
		obj->v[tv+5] = obj->vt[l2+1];
		obj->v[tv+6] = 1.0f;
		obj->v[tv+7] = 1.0f;

*/



		v[tv + 0] = ftoi4(mv[l+0]);
		v[tv + 1] = ftoi4(mv[l+1]);
		v[tv + 2] = ftoi4(mv[l+2]);
		v[tv + 3] = ftoi4(1.0f);


		v[tv + 4] = ftoi15(-mvt[l2+0]);
		v[tv + 5] = ftoi15(-mvt[l2+1]);
		v[tv + 6] = ftoi15(1.0f);
		v[tv + 7] = 0;

	}

	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->f != NULL)  free(model->f);

	model->v = v;

	model->vt = NULL;
	model->vn = NULL;
	model->f = NULL;
}

#endif

