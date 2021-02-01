#include <stdio.h>
#include <stdlib.h>
#include "AURAE/AURAE.h"

#ifdef PLAYSTATION2

#include "AURAE/PS2/PS2.h"

void AURAE_Convert_Model_Index2(AURAE_Model *model)
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

	if(model->flag & AURAE_MODEL_FIXEDPOINT)
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

/*
void CullFace(u16 *v,u16 *index,int l)
{
	int i1 = index[l+0];
	int i2 = index[l+1];
	int i3 = index[l+2];

	Vector2i vec1,vec2,vec3;
	int x = 0;
	int y = 1;

	vec1.x = v[i1+x];
	vec1.y = v[i1+y];

	vec2.x = v[i2+x];
	vec2.y = v[i2+y];

	vec3.x = v[i2+x];
	vec3.y = v[i2+y];

	printf("%d %d \n",vec1.x,vec1.y);
	printf("%d %d \n",vec2.x,vec2.y);
	printf("%d %d \n",vec2.x,vec2.y);
}*/

void AURAE_Convert_Model_Index(AURAE_Model *model)
{
	int tv,l=0,l2,i,j,n,l3;

	float *mv = model->v;
	float *mvt = model->vt;
	unsigned short *sindex = model->index;

	u16 *iv = model->v;
	u16 *ivt = model->vt;
	u16 u1fixe15 = ftoi15(1.0f);
	u16 u1fixe4 = ftoi4(1.0f);

	int nv  = 0,nf = 0;
	for(i = 0;i < model->ngroup;i++)
	{
		n = model->groupface[i];
		n += n&1;
		nf += n;

		n = model->groupvertex[i];
		n += n&1;
		nv += n;
	}

	nv *= 4;
	nf *= 4;
	u16 *v  = malloc(nv * sizeof(u16));
	u16 *vt  = malloc(nv * sizeof(u16));
	u16 *index  = malloc(nf * sizeof(u16));

	int k = 0;
	l = 0;
	int gf = 0;
	int db = 0;
	int t= 0;

	model->size = nv*4 + nf*2;

	for(i = 0;i < model->ngroup;i++)
	{

		db = (i&1)*256;
		n = model->groupface[i];

		for(j = 0;j < model->groupface[i];j++)
		{
			index[k + 0] = sindex[l+0]-gf+db;
			index[k + 1] = sindex[l+1]-gf+db;
			index[k + 2] = sindex[l+2]-gf+db;
			index[k + 3] = 0xFFFF;
/*
			if(l == 0)
				CullFace(iv,sindex,l);
*/
			l += 3;
			k += 4;
		}

		j = n-j+(n&1);
		k += j*4;
		//model->size += n*2*4;

		gf += model->groupvertex[i];
	}


	l = 0;
	l2 = 0;
	k = 0;



	if(model->flag & AURAE_MODEL_FIXEDPOINT)
	{
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

			j = n-j+ (n&1);
			k += j*4;

			gf += n;
			//model->size += n*2*8;
		}
	}else
	{
		for(i = 0;i < model->ngroup;i++)
		{
			n = model->groupvertex[i];
			for(j = 0;j < n;j++)
			{
				v[k + 0] = ftoi4(iv[l+0]);
				v[k + 1] = ftoi4(iv[l+1]);
				v[k + 2] = ftoi4(iv[l+2]);
				v[k + 3] = u1fixe4;

				vt[k + 0] = ftoi15(-ivt[l2+0]);
				vt[k + 1] = ftoi15(-ivt[l2+1]);
				vt[k + 2] = u1fixe15;
				vt[k + 3] = 0;
				l += 3;
				l2 += 2;
				k += 4;
			}

			j = n-j+(n&1);
			k += j*4;

			gf += n;
			//model->size += n*2*8;
		}
	}

	if(model->v  != NULL) free(model->v);
	if(model->vt != NULL) free(model->vt);
	if(model->vn != NULL) free(model->vn);

	if(model->index != NULL)  free(model->index);

	model->v = v;

	model->vt = vt;
	model->vn = NULL;
	model->index = index;



	PS2_VU_Load(model);

}

void AURAE_Convert_Model(AURAE_Model *model)
{
	AURAE_Convert_Model_Index32ToIndex16(model);

	if(model->flag & AURAE_MODEL_INDEX)
		AURAE_Convert_Model_Index(model);
}

#endif

