#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

#include "bcm.h"

LMP3D_Model *LMP3D_Load_bcm(const char *filename,int offset,void *buffer)
{
	BCM_Header bcm;
	int i;
	void *file;
	file = fopen(filename,"rb");

	if(file == NULL) return NULL;

	fseek(file, offset, SEEK_SET);

	fread(&bcm,1,sizeof(BCM_Header),file);

	if(strncmp(bcm.tag,"BCM",4) != 0)
	{
		fclose(file);
		return NULL;
	}

	//printf("%x %x %x %x\n",bcm.flags1,bcm.flags2,bcm.namelen,bcm.weightslen);
	//printf("%f %d\n",bcm.Xmin,bcm.nv);

	LMP3D_Model *model = malloc(sizeof(LMP3D_Model));
    LMP3D_Model_Init(model);

	if(bcm.nv > 0)
	{
		if(bcm.flags1 & BCM_VERTEX)
		{
			model->v  = malloc(bcm.nv*sizeof(float)*3);
			fread(model->v,sizeof(float),bcm.nv*3,file);
		}

		if(bcm.flags1 & BCM_TEXTCOORD)
		{
			model->vt = malloc(bcm.nv*sizeof(float)*2);
			fread(model->vt,sizeof(float),bcm.nv*2,file);
		}

		if(bcm.flags1 & BCM_NORMAL)
		{
			model->vn = malloc(bcm.nv*sizeof(float)*3);
			fread(model->vn,sizeof(float),bcm.nv*3,file);
		}
	}

	if(bcm.flags1 & BCM_INDEX)
	{
		model->f  = malloc(bcm.nf*sizeof(unsigned short)*3);
		fread(model->f,sizeof(unsigned short),bcm.nf*3,file);
	}

	if(bcm.ntexture > 0)
	{
		model->texture = malloc(bcm.ntexture*sizeof(LMP3D_Texture*));
		model->texture_begin = malloc(bcm.ntexture*sizeof(int));
		model->name          = malloc(bcm.ntexture*sizeof(char*));

		fread(model->texture_begin,sizeof(int),bcm.ntexture,file);

		for(i = 0;i < bcm.ntexture;i++)
		{
			model->name[i] = malloc(bcm.namelen*sizeof(char));
			fread(model->name[i],sizeof(char),bcm.namelen,file);
		}

	}

	if(bcm.flags1 & BCM_ANIM)
	{
		model->id = malloc(bcm.nv*sizeof(unsigned char));
		fread(model->id,sizeof(unsigned char),bcm.nv,file);
	}

	fclose(file);

	model->nf = bcm.nf;
	model->nv = bcm.nv;
	model->uv = 1;
	model->type = 3;
	model->ntexture = bcm.ntexture;

	model->Xmin = bcm.Xmin;
	model->Ymin = bcm.Ymin;
	model->Zmin = bcm.Zmin;

	model->Xmax = bcm.Xmax;
	model->Ymax = bcm.Ymax;
	model->Zmax = bcm.Zmax;

	//printf("%d %lf\n",bcm.nv,bcm.nf);


	LMP3D_Convert_Model(model);

	char folder[100];

    LMP3D_Folder_Out(filename,folder);
    LMP3D_Load_Texture_Array(model,filename,folder,offset,NULL);

	return model;


}
