#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1

#include "LMP3D/PS1/PS1.h"

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	LMP3D_Convert_Model_Index32ToIndex16(model);

	unsigned int tv,l,l2,i;

	model->nv = model->nf*3;
	short *v  = malloc(model->nv*3 * sizeof(short));
	unsigned int *vt = malloc(model->nf*3 * sizeof(unsigned int));

	//printf("%d %d %d\n",(int)model->Xmin,(int)model->Ymin,(int)model->Zmin);

	float *mv = model->v;
	float *mvt = model->vt;

	short *iv = model->v;
	unsigned short *ivt = model->vt;

	unsigned short *index = model->index;

	LMP3D_Texture dtexture;
	dtexture.w = 256;
	dtexture.h = 256;
	dtexture.addresspal = 0;
	dtexture.address = 0;
	dtexture.psm = 0;

	LMP3D_Texture *texture = model->texture[0];

	if(texture == NULL) texture = &dtexture;

	int w=texture->w,h=texture->h;

	int tx = (texture->address&0x3F);
	int ty = 0;
	if(texture->address&0x40) ty = 1;

	int destY = 240 + (texture->addresspal&0x0F);
	int destX = (texture->addresspal&0x30)<<7;
	unsigned int dest = (destX+(destY<<6))<<16;
	int j = 0;

	unsigned int gpo_texpage = (GP0_TEXPAGE(tx,ty,0,texture->psm,0,1,0,0,0)&0xFFFF)<<16;

	if(model->flag & LMP3D_MODEL_FIXEDPOINT)
	{
		if(w == 256) w = 0;
		else w = 1;
		h = h>>8;
		for(i = 0;i < model->nf*3;i++)
		{
			l2 = index[i]<<1;
			l = l2+index[i];

			v[tv + 0] = (iv[l+0]);
			v[tv + 1] = (iv[l+1]);
			v[tv + 2] = (iv[l+2]);
			tv += 3;

			vt[i]  = ( ( (ivt[l2+0])>>(7+w))&0x00FF ) | ( (ivt[l2+1]<<h)&0xFF00 );

			j++;

			if(j == 1)
				vt[i] += dest;

			if(j == 2)
				vt[i] += gpo_texpage;

			if(j == 3) j = 0;
		}

	}else
	{
		w--;
		h--;
		for(i = 0;i < model->nf*3;i++)
		{
			l = index[i]*3;
			l2 = index[i]*2;

			v[tv + 0] = (short)(mv[l+0]*16.0f);
			v[tv + 1] = (short)(mv[l+1]*16.0f);
			v[tv + 2] = (short)(mv[l+2]*16.0f);
			tv += 3;

			vt[i]  = (int)(mvt[l2+0]*w) | ( ((int)(mvt[l2+1]*h))<<8);
			j++;

			if(j == 1)
				vt[i] += dest;

			if(j == 2)
				vt[i] += gpo_texpage;

			if(j == 3) j = 0;
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
