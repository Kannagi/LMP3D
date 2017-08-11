#ifndef __LMP3D_Camera__
#define __LMP3D_Camera__

#include "LMP3D/Type.h"

typedef struct
{
    Vector3 position;
    Vector3 vision;
    Vector3 top;
    float angle,ratio,min,max;
}LMP3D_Camera;


void LMP3D_Camera_LookAt_GL(LMP3D_Camera camera);
void LMP3D_Camera_Perspective_GL(LMP3D_Camera camera);
void LMP3D_Camera_Ortho_GL();


LMP3D_Camera LMP3D_Camera_Init();
void LMP3D_Camera_LookAt(LMP3D_Camera camera);
void LMP3D_Camera_Perspective(LMP3D_Camera camera);
void LMP3D_Camera_Ortho();
void LMP3D_camera_vue_obj(LMP3D_Camera *camera,Vector3 *p,Vector3 *a,float v,float vit,int *touche);
void LMP3D_Camera_vue_sub(LMP3D_Camera *camera,Vector3 *a,float v,int *touche);

#endif

