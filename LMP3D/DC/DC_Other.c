#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"


void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:
		break;

		case LMP3D_FORMAT_RGBA8888:
		break;

		case LMP3D_FORMAT_RGB555:

		break;

		case LMP3D_FORMAT_RGBA1555:

		break;

		case LMP3D_FORMAT_8BPP:
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void LMP3D_Camera_LookAt(LMP3D_Camera camera)
{

}

void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{

}

void LMP3D_Convert_Model(LMP3D_Model *model)
{
	LMP3D_Convert_Model_Index16ToIndex32(model);

	unsigned int l=0,i,tv = 0;

	float *v  = malloc(model->nv*4 * sizeof(float));
	unsigned int *iv = v;

	float *mv = model->v;
	unsigned int *mvt = model->vt;

	short *sv  =  model->v;
	unsigned short *svt =  model->vt;

	float ut,vt;
	unsigned int *pu = &ut,*pv = &vt;

	unsigned short *index =  model->index;

	if(model->flag & LMP3D_MODEL_INDEX)
	{
		for(i = 0;i < model->nf*3;i++)
			index[i] = index[i]<<4;
	}


	if(model->flag & LMP3D_MODEL_FIXEDPOINT)
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
			/*
			l = model->f[i]*3;
			l2 = model->f[i]*2;*/

			unsigned int ufl2 = mvt[l++];
			unsigned int ufl = mvt[l++];
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

void LMP3D_Camera_Ortho2D()
{

}



void LMP3D_Model_Draw(LMP3D_Model *model)
{

}

void bitmap_font2(char *texte,int tx,int ty)
{

}

void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{
	void *pixel,*ncpixel;
	int dif;

	pixel = malloc(texture->size+0x100);
	ncpixel = UNCACHED_P2(pixel);
	dif = 0x100 - ( (u32)pixel&0xFF);
	memcpy(ncpixel+dif,texture->pixel, texture->size);

	while(!pvr_dma_ready());
	pvr_txr_load_dma( pixel+dif,(pvr_ptr_t)texture->address,texture->size,0,NULL,0);
	while(!pvr_dma_ready());

	free(pixel);
}

void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{

}


void LMP3D_Texture_Setup(LMP3D_Texture *texture)
{



}





#endif

