#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PC


void LMP3D_Convert_Model(LMP3D_Model *model)
{
	if(!(model->flag & LMP3D_MODEL_FIXEDPOINT)) return;

	int l=0,i,l2=0;

	short *iv = model->v;
	float *v  = malloc(model->nv*3 * sizeof(float));

	unsigned short *ivt = model->vt;
	float *vt  = malloc(model->nv*2 * sizeof(float));

	for(i = 0;i < model->nv;i++)
	{
		vt[l++] = itof15(ivt[l]);
		vt[l++] = itof15(ivt[l]);

		v[l2++] = itof4(iv[l2]);
		v[l2++] = itof4(iv[l2]);
		v[l2++] = itof4(iv[l2]);
	}

	if(model->vt != NULL) free(model->vt);
	if(model->v  != NULL) free(model->v);

	model->vt = vt;
	model->v = v;


	model->size = (model->nv*5*4) + (model->nf*3*2);

	model->flag ^= LMP3D_MODEL_FIXEDPOINT;
}

#endif

