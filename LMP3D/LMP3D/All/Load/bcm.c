#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

#include "bcm.h"

LMP3D_Model *LMP3D_Load_Model_bcm(char *path)
{
	BCM_Header bcm;
	int i;
	FILE *file;
	file = fopen(path,"rb");

	if(file == NULL) return NULL;

	LMP3D_Model *model = malloc(sizeof(LMP3D_Model));
    LMP3D_Model_Init(model);

	fread(&bcm,1,sizeof(BCM_Header),file);
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

	printf("%d %d\n",bcm.nv,bcm.nf);


	if(bcm.nv > 0)
	{
		model->v  = malloc(bcm.nv*sizeof(float)*3);
		model->vt = malloc(bcm.nv*sizeof(float)*2);
		model->vn = malloc(bcm.nv*sizeof(float)*3);

		if(bcm.flags1 & BCM_VERTEX)
			fread(model->v,sizeof(float),bcm.nv*3,file);

		if(bcm.flags1 & BCM_TEXTCOORD)
			fread(model->vt,sizeof(float),bcm.nv*2,file);

		if(bcm.flags1 & BCM_NORMAL)
			fread(model->vn,sizeof(float),bcm.nv*3,file);
	}

	if(bcm.flags1 & BCM_INDEX)
	{
		model->f  = (unsigned short *)malloc(bcm.nf*sizeof(unsigned short)*3);

		fread(model->f,sizeof(unsigned short),bcm.nf*3,file);
	}

	if(bcm.ntexture > 0)
	{
		model->texture = malloc(bcm.ntexture*sizeof(int));
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


	LMP3D_Convert_Model(model);

	char folder[100];

    LMP3D_Folder_Out(path,folder);
    LMP3D_Load_Texture_Array(model,folder);

	return model;


}
