#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PC


void LMP3D_Convert_Model(LMP3D_Model *model)
{
	unsigned int l=0,i,l2=0;

	float *vt;
	float *v;
	//float *vn;

	model->size = (model->nv*5*4) + (model->nf*3*2);

	if(!(model->flag & LMP3D_MODEL_FIXEDPOINT))
		return;



	short *iv = model->v;
	v  = malloc(model->nv*3 * sizeof(float));

	unsigned short *ivt = model->vt;
	vt  = malloc(model->nv*2 * sizeof(float));

	for(i = 0;i < model->nv;i++)
	{
		vt[l+0] = itof15(ivt[l+0]);
		vt[l+1] = itof15(ivt[l+1]);

		v[l2+0] = itof4(iv[l2+0]);
		v[l2+1] = itof4(iv[l2+1]);
		v[l2+2] = itof4(iv[l2+2]);

		l += 2;
		l2 += 3;
	}

	if(model->vt != NULL) free(model->vt);
	if(model->v  != NULL) free(model->v);

	model->vt = vt;
	model->v = v;

	model->flag ^= LMP3D_MODEL_FIXEDPOINT;
}

#endif

