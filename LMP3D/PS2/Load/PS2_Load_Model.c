#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION2

#include "LMP3D/PS2/PS2.h"




void LMP3D_Convert_Model(LMP3D_Model *model)
{
	LMP3D_Convert_Model_Index16ToIndex32(model);

	if(model->flag & LMP3D_MODEL_INDEX)
		LMP3D_Convert_Model_Index(model);


}

void LMP3D_Convert_Model_Index(LMP3D_Model *model)
{
	int tv,l,l2,i;
	model->nv = model->nf*3;

	u16 *v  = malloc(model->nf*3 * 8 * sizeof(u16));

	float *mv = model->v;
	float *mvt = model->vt;
	unsigned short *index = model->index;

	u16 *iv = model->v;
	u16 *ivt = model->vt;

	if(model->flag & LMP3D_MODEL_FIXEDPOINT)
	{
		for(i = 0;i < model->nv;i++)
		{
			tv = i<<3;
			l = index[i]*3;
			l2 = index[i]<<1;

			v[tv + 0] = iv[l+0];
			v[tv + 1] = iv[l+1];
			v[tv + 2] = iv[l+2];
			v[tv + 3] = ftoi4(1.0f);

			v[tv + 4] = -ivt[l2+0];
			v[tv + 5] = -ivt[l2+1];
			v[tv + 6] = ftoi15(1.0f);
			v[tv + 7] = 0;
		}
	}else
	{
		for(i = 0;i < model->nv;i++)
		{
			tv = i<<3;
			l = index[i]*3;
			l2 = index[i]<<1;

			v[tv + 0] = ftoi4(mv[l+0]);
			v[tv + 1] = ftoi4(mv[l+1]);
			v[tv + 2] = ftoi4(mv[l+2]);
			v[tv + 3] = ftoi4(1.0f);

			v[tv + 4] = ftoi15(-mvt[l2+0]);
			v[tv + 5] = ftoi15(-mvt[l2+1]);
			v[tv + 6] = ftoi15(1.0f);
			v[tv + 7] = 0;
		}
	}



	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->index != NULL)  free(model->index);

	model->v = v;

	model->vt = NULL;
	model->vn = NULL;
	model->index = NULL;

	model->size = model->nv*8*2;
}

#endif

