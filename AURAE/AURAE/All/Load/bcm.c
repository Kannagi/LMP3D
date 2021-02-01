#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AURAE/AURAE.h"

#include "bcm.h"

AURAE_Model *AURAE_Load_bcm(char *filename,int offset,void *buffer,int size)
{
	BCM_Header bcm;
	int i,sizev = sizeof(float),sizeindex = sizeof(unsigned short);
	void *file;
	file = AURAE_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	AURAE_fseek(file, offset, SEEK_SET);

	AURAE_fread(&bcm,1,sizeof(BCM_Header),file);

	if(strncmp(bcm.tag,"BCM",4) != 0)
	{
		fclose(file);
		return NULL;
	}

	if(bcm.flags1 & BCM_FIXEDPOINT)
		sizev = sizeof(unsigned short);

	if(bcm.flags1 & BCM_INDEX_U32)
		sizeindex = sizeof(unsigned int);
	//printf("%x %x %x %x\n",bcm.flags1,bcm.flags2,bcm.namelen,bcm.weightslen);
	//printf("%f %d\n",bcm.Xmin,bcm.nv);

	AURAE_Model *model = malloc(sizeof(AURAE_Model));
	AURAE_Model_Init(model);

	if(bcm.nv > 0)
	{
		if(bcm.flags1 & BCM_VERTEX)
		{
			model->v  = malloc(bcm.nv*sizev*3);
			AURAE_fread(model->v,sizev,bcm.nv*3,file);
			model->size += (bcm.nv*3*sizev);
		}

		if(bcm.flags1 & BCM_TEXTCOORD)
		{
			model->vt = malloc(bcm.nv*sizev*2);
			AURAE_fread(model->vt,sizev,bcm.nv*2,file);
			model->size += (bcm.nv*2*sizev);
		}

		if(bcm.flags1 & BCM_NORMAL)
		{
			model->vn = malloc(bcm.nv*sizev*3);
			AURAE_fread(model->vn,sizev,bcm.nv*3,file);
			model->size += (bcm.nv*3*sizev);
		}
	}

	if(bcm.flags1 & BCM_INDEX)
	{
		model->index  = malloc(bcm.nf*sizeindex*3);
		AURAE_fread(model->index,sizeindex,bcm.nf*3,file);
		model->size += (bcm.nf*3*sizeindex);
	}


	//printf("face : %d/vertex : %d\n",bcm.nf,bcm.nv);

	if(bcm.ntexture > 0)
	{
		model->texture = malloc(bcm.ntexture*sizeof(AURAE_Texture*));
		model->texture_begin = malloc(bcm.ntexture*sizeof(int));
		model->name		  = malloc(bcm.ntexture*sizeof(char*));

		AURAE_fread(model->texture_begin,sizeof(int),bcm.ntexture,file);

		for(i = 0;i < bcm.ntexture;i++)
		{
			model->name[i] = malloc(bcm.namelen*sizeof(char));
			AURAE_fread(model->name[i],sizeof(char),bcm.namelen,file);
		}

	}

	if(bcm.flags1 & BCM_GROUP)
	{
		model->groupvertex  = malloc(bcm.ngroup*sizeof(unsigned int));
		model->groupface	= malloc(bcm.ngroup*sizeof(unsigned int));
		AURAE_fread(model->groupvertex,sizeof(unsigned int),bcm.ngroup,file);
		AURAE_fread(model->groupface,sizeof(unsigned int),bcm.ngroup,file);
	}

	if(bcm.flags1 & BCM_ANIM)
	{
		model->bones  = malloc((bcm.nbones+1)*sizeof(unsigned int));
		AURAE_fread(model->bones,sizeof(unsigned int),bcm.nbones,file);

		model->bones[bcm.nbones] = bcm.nv;

		model->matrix_bones  = malloc((bcm.nbones+1)*sizeof(float) *16);
		AURAE_fread(model->matrix_bones,sizeof(float),bcm.nbones*16,file);
	}

	AURAE_fclose(file);

	model->nf = bcm.nf;
	model->nv = bcm.nv;
	model->flag = bcm.flags1;
	model->ntexture = bcm.ntexture;
	model->ngroup = bcm.ngroup;
	model->nbones = bcm.nbones;

	model->Xmin = bcm.Xmin;
	model->Ymin = bcm.Ymin;
	model->Zmin = bcm.Zmin;

	model->Xmax = bcm.Xmax;
	model->Ymax = bcm.Ymax;
	model->Zmax = bcm.Zmax;


	char folder[100];
	folder[0] = 0;

	if(filename != NULL)
		AURAE_Folder_Out(filename,folder);


	printf("filename/folder  : %s/%s\n",folder,filename);

	AURAE_Load_Texture_Array(model,filename,folder,offset,buffer,size);

	AURAE_Convert_Model(model);

	printf("byte : %d\n",model->size);

	return model;
}

