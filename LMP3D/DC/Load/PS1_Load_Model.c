#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1

#include "LMP3D/PS1/PS1.h"

void LMPD3D_Convert_Model(LMP3D_Model *model)
{
	int tv,l2;

	free(obj->vn);
	free(obj->v);

	//obj->v  = malloc(obj->nf*3 * 8 * sizeof(u16));


	obj->v  = malloc(obj->nf*3 * 8 * sizeof(float));

	u16 * uv = (u16 *)obj->v;
	for(i = 0;i < obj->nf*3;i++)
	{
		tv = i*8;
		l = obj->f[i]*3;
		l2 = obj->f[i]*2;
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



		uv[tv + 0] = ftoi4(obj->va[l+0]);
		uv[tv + 1] = ftoi4(obj->va[l+1]);
		uv[tv + 2] = ftoi4(obj->va[l+2]);
		uv[tv + 3] = ftoi4(1.0f);


		uv[tv + 4] = ftoi15(-obj->vt[l2+0]);
		uv[tv + 5] = ftoi15(-obj->vt[l2+1]);
		uv[tv + 6] = ftoi15(1.0f);
		uv[tv + 7] = ftoi15(1.0f);



	}
	free(obj->va);
	free(obj->vt);
	free(obj->f);
}

#endif
