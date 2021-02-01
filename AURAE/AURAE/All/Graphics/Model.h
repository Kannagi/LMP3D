#ifndef __AURAE_Model__
#define __AURAE_Model__

#include "AURAE/AURAE.h"

enum
{
	AURAE_MODEL_VERTEX = 0x01,
	AURAE_MODEL_TEXTCOORD = 0x02,
	AURAE_MODEL_NORMAL = 0x04,
	AURAE_MODEL_INDEX = 0x08,
	AURAE_MODEL_ANIM = 0x10,

	AURAE_MODEL_INDEX_U32 = 0x20,
	AURAE_MODEL_FIXEDPOINT = 0x40,

	AURAE_MODEL_POLY_TRIANGLE = 0x100,
	AURAE_MODEL_POLY_TRIANGLE_STRIP = 0x200,
	AURAE_MODEL_POLY_TRIANGLE_AND_STRIPS = 0x400,
	AURAE_MODEL_TEXTUREDISABLE = 0x1000,
};

typedef struct
{
	float matrix[16];
}MATRIX4x4;

typedef struct
{
	MATRIX4x4 *skeleton;
	short ntime,ncadence,n;
}AURAE_Anim3D;

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
	AURAE_Texture **texture;
	int ntexture;
	char **name;


	AURAE_Anim3D anim;
/*
	//Animation
	short time; //frame
	short play_old,cadence; // variable temporaire
	short play,boucle,end; //control de animation
	short nanim;//nombre animation
	Vector3 *nodes; //skelette nodes


	AURAE_Anim3D defaut,**anim; //Squelette
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
}AURAE_Model;

void AURAE_Draw_Model(AURAE_Model *model);

void AURAE_Model_Init(AURAE_Model *model);
void AURAE_Model_Draw(AURAE_Model *model);

void AURAE_Animation3D(AURAE_Model *model);
void AURAE_Convert_Model_Index16ToIndex32(AURAE_Model *model);
void AURAE_Convert_Model_Index32ToIndex16(AURAE_Model *model);
void AURAE_Convert_Model(AURAE_Model *model);
void AURAE_Model_Free(AURAE_Model *model);

#endif
