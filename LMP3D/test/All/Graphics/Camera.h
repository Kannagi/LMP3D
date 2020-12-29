#ifndef __LMP3D_Camera__
#define __LMP3D_Camera__

#include "LMP3D/All/Type.h"

typedef struct
{
    Vector3 position;
    Vector3 vision;
    Vector3 top;
    Vector3 angle;
    float viewangle,ratio,min,max,speed,speedz;
    int key[12];

}LMP3D_Camera;

LMP3D_Camera LMP3D_Camera_Init();
void LMP3D_Camera_Perspective(LMP3D_Camera camera);
void LMP3D_Camera_Orthogonal(LMP3D_Camera camera);
void LMP3D_Camera_Ortho2D();
void LMP3D_Camera_ViewSub(LMP3D_Camera *camera);
void LMP3D_Camera_ViewObj(LMP3D_Camera *camera,Vector3 *position);
void LMP3D_Camera_LookAt(float *MatrixLookAt,Vector3 eye,Vector3 center,Vector3 up);

#endif

