#include <stdio.h>
#include <stdlib.h>
#include "AURAE/AURAE.h"

#ifdef DREAMCAST

#include "AURAE/DC/DC.h"
unsigned int DC_BufferVertex_adr();

void AURAE_Convert_Model_Index16ToIndex32(AURAE_Model *model);
void AURAE_Convert_Model(AURAE_Model *model)
{
	AURAE_Convert_Model_Index32ToIndex16(model);

	unsigned int i,tv = 0,l = 0;

	float *v  = malloc(model->nv*4 * sizeof(float));
	unsigned int *iv = v;

	float *mv = model->v;
	unsigned int *mvt = model->vt;

	short *sv  =  model->v;
	unsigned short *svt =  model->vt;
	float *fvt = mvt;

	float ut,vt;
	unsigned int *pu = &ut,*pv = &vt;

	unsigned short *index =  model->index;

	if(model->flag & AURAE_MODEL_INDEX)
	{
		for(i = 0;i < model->nf*3;i++)
			index[i] = (index[i]<<4);
	}

	if(model->flag & AURAE_MODEL_FIXEDPOINT)
	{
		for(i = 0;i < model->nv*3;tv += 4)
		{
			ut = itof15(svt[l++]);
			vt = itof15(svt[l++]);

			unsigned int ufl2 = *pu;
			unsigned int ufl = *pv;
			ufl = (ufl>>16) + (ufl2&0xFFFF0000);

			v[tv + 0]  = itof4(sv[i++]);
			v[tv + 1]  = itof4(sv[i++]);
			v[tv + 2]  = itof4(sv[i++]);
			iv[tv + 3] = ufl;
		}

	}else
	{
		for(i = 0;i < model->nv*3;tv += 4)
		{
			fvt[l+1] = -fvt[l+1];

			unsigned int ufl2 = mvt[l++];
			unsigned int ufl  = mvt[l++];
			ufl = (ufl>>16) + (ufl2&0xFFFF0000);

			v[tv + 0]  = mv[i++];
			v[tv + 1]  = mv[i++];
			v[tv + 2]  = mv[i++];
			iv[tv + 3] = ufl;
		}
	}

	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	model->v = v;

	model->vt = NULL;
	model->vn = NULL;

	model->size = (model->nv*4*4)+(model->nf*3*2);
}
#endif
