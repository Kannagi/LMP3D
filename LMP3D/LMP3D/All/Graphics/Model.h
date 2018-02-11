#ifndef __LMP3D_Model__
#define __LMP3D_Model__


#include "LMP3D/LMP3D.h"

typedef struct
{
    Matrix3x2 *skeleton;
    short ntime,ncadence,n;
}LMP3D_Anim3D;

typedef struct
{
    //Mesh
    void *v,*vt,*vn,*va;
    int nv,nf;
    unsigned short *f;

    //groupe
    unsigned short *groupe;
    short ngroupe;

    //Texture
    int *texture_begin;
    LMP3D_Texture **texture;
    short ntexture,uv;
	char **name;

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

    //Autre information
    short type; //Drapeau 0x01 = (0)Array/(1)Index ,0x02 = (0)Static/(1)Anim

    //Opengl
    //int VBO[3]; //VBO
    //int shaders; //shader

	Vector3 position;
	Vector3 scale;
	Vector3 rotate;
	float Xmin,Xmax,Ymin,Ymax,Zmin,Zmax;

	int test;


}LMP3D_Model;

void LMP3D_Draw_Model(LMP3D_Model *model);

void LMP3D_Model_Init(LMP3D_Model *model);
void LMP3D_Model_Draw(LMP3D_Model *model);

void LMP3D_Animation3D(LMP3D_Model *model);



#endif
