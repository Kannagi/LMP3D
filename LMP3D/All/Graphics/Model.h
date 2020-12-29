#ifndef __LMP3D_Model__
#define __LMP3D_Model__

#include "LMP3D/LMP3D.h"

enum
{
	LMP3D_MODEL_VERTEX = 0x01,
	LMP3D_MODEL_TEXTCOORD = 0x02,
	LMP3D_MODEL_NORMAL = 0x04,
	LMP3D_MODEL_INDEX = 0x08,
	LMP3D_MODEL_ANIM = 0x10,

	LMP3D_MODEL_INDEX_U32 = 0x20,
	LMP3D_MODEL_FIXEDPOINT = 0x40,

	LMP3D_MODEL_POLY_TRIANGLE = 0x100,
	LMP3D_MODEL_POLY_TRIANGLE_STRIP = 0x200,
	LMP3D_MODEL_POLY_TRIANGLE_AND_STRIPS = 0x400,
	LMP3D_MODEL_TEXTUREDISABLE = 0x1000,
};

typedef struct
{
	float matrix[16];
}MATRIX4x4;

typedef struct
{
	MATRIX4x4 *skeleton;
	short ntime,ncadence,n;
}LMP3D_Anim3D;

typedef struct
{
	//Mesh
	void *v,*vt,*vn,*va;
	unsigned int nv,nf,ngroup;
	void *index;
	unsigned int *groupvertex,*groupface;
	unsigned int *bones,nbones;
	float *matrix_bones;

	//Texture
	int *texture_begin;
	LMP3D_Texture **texture;
	int ntexture;
	char **name;


	LMP3D_Anim3D anim;
/*
	//Animation
	short time; //frame
	short play_old,cadence; // variable temporaire
	short play,boucle,end; //control de animation
	short nanim;//nombre animation
	Vector3 *nodes; //skelette nodes


	LMP3D_Anim3D defaut,**anim; //Squelette
	unsigned char *id; //attribut pour chaque vertex
	short *bones; //relation entre bones parent/enfant
	short nbones; //nombre de bones
*/
	Vector3 position;
	Vector3 scale;
	Vector3 rotate;
	float Xmin,Xmax,Ymin,Ymax,Zmin,Zmax;
/*
	Vector3i iposition;
	Vector3s iscale,irotate;
	short iXmin,iXmax,iYmin,iYmax,iZmin,iZmax;
*/
	float box[32];
	int test,flag,size;
	void *displaylist[2];
}LMP3D_Model;

void LMP3D_Draw_Model(LMP3D_Model *model);

void LMP3D_Model_Init(LMP3D_Model *model);
void LMP3D_Model_Draw(LMP3D_Model *model);

void LMP3D_Animation3D(LMP3D_Model *model);
void LMP3D_Convert_Model_Index16ToIndex32(LMP3D_Model *model);
void LMP3D_Convert_Model_Index32ToIndex16(LMP3D_Model *model);
void LMP3D_Convert_Model(LMP3D_Model *model);
void LMP3D_Model_Free(LMP3D_Model *model);

#endif
