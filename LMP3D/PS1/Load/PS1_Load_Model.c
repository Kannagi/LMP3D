#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1

#include "LMP3D/PS1/PS1.h"

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	LMP3D_Convert_Model_Index16ToIndex32(model);

	int tv,l,l2,i;

	model->nv = model->nf*3;
	short *v  = malloc(model->nv*3 * sizeof(short));
	unsigned int *vt = malloc(model->nf*3 * sizeof(unsigned int));

	//printf("%d %d %d\n",(int)model->Xmin,(int)model->Ymin,(int)model->Zmin);

	float *mv = model->v;
	float *mvt = model->vt;


	short *iv = model->v;
	unsigned short *ivt = model->vt;

	unsigned short *index = model->index;

	int w=256,h=256;
	int itri = 0;

	if(model->texture != NULL)
	{
		w = model->texture[0]->w;
		h = model->texture[0]->h;
	}

	if(model->flag & LMP3D_MODEL_FIXEDPOINT)
	{
		for(i = 0;i < model->nf*3;i++)
		{
			tv = i*3;
			l = index[i]*3;
			l2 = index[i]<<1;

			v[tv + 0] = iv[l+0]>>4;
			v[tv + 1] = iv[l+1]>>4;
			v[tv + 2] = iv[l+2]>>4;

			vt[i]  = ( ( (ivt[l2+0])>>7)&0x00FF ) | ( (ivt[l2+1]<<1)&0xFF00 ) ;
		}
	}else
	{
		w--;
		h--;
		for(i = 0;i < model->nf*3;i++)
		{
			tv = i*3;
			l = index[i]*3;
			l2 = index[i]*2;

			v[tv + 0] = (short)(mv[l+0]);
			v[tv + 1] = (short)(mv[l+1]);
			v[tv + 2] = (short)(mv[l+2]);

			//printf("%d %d\n",(int)(model->vt[l2+0]*255.0f),(( int)(model->vt[l2+1]*255.0f)));
			vt[i]  = (int)(mvt[l2+0]*w) | ( ((int)(mvt[l2+1]*h))<<8);
		}
	}

	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->index != NULL)  free(model->index);

	model->v = v;
	model->vt = vt;

	model->vn = NULL;
	model->index = NULL;

	model->size = (model->nf*3*2) + (model->nf*3*4);
}

#endif
