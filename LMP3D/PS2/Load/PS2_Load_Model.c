#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION2

#include "LMP3D/PS2/PS2.h"

void LMP3D_Convert_Model_Index2(LMP3D_Model *model)
{
	int tv,l,l2,i;
	model->nv = model->nf*3;

	u16 *v  = malloc(model->nf*3 * 8 * sizeof(u16));

	float *mv = model->v;
	float *mvt = model->vt;
	unsigned short *index = model->index;

	u16 *iv = model->v;
	u16 *ivt = model->vt;
	u16 u1fixe15 = ftoi15(1.0f);
	u16 u1fixe4 = ftoi4(1.0f);

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
			v[tv + 3] = u1fixe4;

			v[tv + 4] = -ivt[l2+0];
			v[tv + 5] = -ivt[l2+1];
			v[tv + 6] = u1fixe15;
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
			v[tv + 3] = u1fixe4;

			v[tv + 4] = ftoi15(-mvt[l2+0]);
			v[tv + 5] = ftoi15(-mvt[l2+1]);
			v[tv + 6] = u1fixe15;
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

void LMP3D_Convert_Model_Index(LMP3D_Model *model)
{
	int tv,l=0,l2,i,j,k,n,l3;

	float *mv = model->v;
	float *mvt = model->vt;
	unsigned short *sindex = model->index;

	u16 *iv = model->v;
	u16 *ivt = model->vt;
	u16 u1fixe15 = ftoi15(1.0f);
	u16 u1fixe4 = ftoi4(1.0f);

	int nv = model->ngroup*160*4;
	int nf = model->ngroup*178*4;
	u16 *v  = malloc(nv * sizeof(u16));
	u16 *vt  = malloc(nv * sizeof(u16));
	u16 *index  = malloc(nf * sizeof(u16));

	if(model->flag & LMP3D_MODEL_FIXEDPOINT)
	{
		l = 0;
		k = 0;
		int gf = 0;
		int db = 0;

		for(i = 0;i < model->ngroup;i++)
		{
			db = (i&1)*498;
			n = model->groupface[i];
			for(j = 0;j < model->groupface[i];j++)
			{
				index[k + 0] = sindex[l+0]-gf+db;
				index[k + 1] = sindex[l+1]-gf+db;
				index[k + 2] = sindex[l+2]-gf+db;
				index[k + 3] = 0;

				l += 3;
				k += 4;
			}

			j = 178-j;
			k += j*4;

			gf += model->groupvertex[i];
		}


		l = 0;
		l2 = 0;
		k = 0;

		for(i = 0;i < model->ngroup;i++)
		{
			n = model->groupvertex[i];
			for(j = 0;j < n;j++)
			{
				v[k + 0] = iv[l+0];
				v[k + 1] = iv[l+1];
				v[k + 2] = iv[l+2];
				v[k + 3] = u1fixe4;

				vt[k + 0] = -ivt[l2+0];
				vt[k + 1] = -ivt[l2+1];
				vt[k + 2] = u1fixe15;
				vt[k + 3] = 0;
				l += 3;
				l2 += 2;
				k += 4;
			}

			j = 160-j;
			k += j*4;

			gf += model->groupvertex[i];
		}

	}else
	{

	}

	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->index != NULL)  free(model->index);

	model->v = v;

	model->vt = vt;
	model->vn = NULL;
	model->index = index;

	model->size = (nv +nv +nf)*2;
}

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	LMP3D_Convert_Model_Index32ToIndex16(model);

	if(model->flag & LMP3D_MODEL_INDEX)
		LMP3D_Convert_Model_Index(model);
}

#endif

