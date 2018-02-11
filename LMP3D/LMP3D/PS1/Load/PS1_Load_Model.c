#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1

#include "LMP3D/PS1/PS1.h"

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	int tv,l,l2,i;


	model->nv = model->nf*3;
	short *v  = malloc(model->nv*3 * sizeof(short));
	int *vt   = malloc(model->nf*3 * sizeof(int));

	//printf("%d %d %d\n",(int)model->Xmin,(int)model->Ymin,(int)model->Zmin);

	float *mv = model->v;
	float *mvt = model->vt;
	for(i = 0;i < model->nf*3;i++)
	{
		tv = i*3;
		l = model->f[i]*3;
		l2 = model->f[i]*2;

		v[tv + 0] = (short)(mv[l+0]);
		v[tv + 1] = (short)(mv[l+1]);
		v[tv + 2] = (short)(mv[l+2]);

		//printf("%d %d\n",(int)(model->vt[l2+0]*255.0f),(( int)(model->vt[l2+1]*255.0f)));
		vt[i]  = (int)(mvt[l2+0]*255.0f) | ( ((int)(mvt[l2+1]*-255.0f))<<8);

	}



	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->f != NULL)  free(model->f);

	model->v = v;
	model->vt = vt;

	model->vn = NULL;
	model->f = NULL;
}

#endif
