#ifndef __LMP3D_Model__
#define __LMP3D_Model__


#include "LMP3D/Type.h"

typedef struct
{
    Matrix3x2 *skeleton;
    short ntime,ncadence,n;
}LMP3D_Anim3D;

typedef struct
{
    //Mesh
    float *v,*vt,*vn,*va;
    int nv,nf;
    unsigned short *f;

    //groupe
    unsigned short *groupe;
    short ngroupe;

    //Texture
    int *texture,*texture_begin;
    short ntexture,uv;
	char **name;

    //Animation
    short time; //frame
    short play_old,cadence; // variable temporaire
    short play,boucle,end; //control de animation read/write
    short nanim;//nombre animation
    Vector3 *nodes; //skelette nodes


    LMP3D_Anim3D defaut,**anim; //Squelette
    short *id; //attribut pour chaque vertex
    short *bones; //relation entre bones parent/enfant
    short nbones; //nombre de bones

    //Autre information
    short format; //format obj = 0 , smd = 1 ,pmd = 2,bcm = 3,md5 = 4
    short type; //Drapeau 0x01 = (0)Array/(1)Index ,0x02 = (0)Static/(1)Anim

    //Opengl
    int VBO[3]; //VBO
    int shaders; //shader
    int dlist; //display list


	Vector3 position;
	Vector3 scale;
	Vector3 rotate;

	int test;


}LMP3D_Model;

void LMP3D_Draw_Model(LMP3D_Model *obj);

void LMP3D_Model_Init(LMP3D_Model *obj);
void LMP3D_Model_Draw(LMP3D_Model *obj);

void LMP3D_Animation3D(LMP3D_Model *obj);

LMP3D_Model *LMP3D_Load_Model_bcm(char *nom_du_fichier);


#endif
