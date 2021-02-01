#ifndef __AURAE_Camera__
#define __AURAE_Camera__

#include "AURAE/All/Type.h"

typedef struct
{
	Vector3 position;
	Vector3 vision;
	Vector3 top;
	Vector3 angle;
	float viewangle,ratio,min,max,speed,speedz;
	int key[12];

}AURAE_Camera;

AURAE_Camera AURAE_Camera_Init(void);
void AURAE_Camera_Perspective(AURAE_Camera camera);
void AURAE_Camera_Orthogonal(AURAE_Camera camera);
void AURAE_Camera_Ortho2D(void);
void AURAE_Camera_ViewSub(AURAE_Camera *camera);
void AURAE_Camera_ViewObj(AURAE_Camera *camera,Vector3 *position);
void AURAE_Camera_LookAt(float *MatrixLookAt,Vector3 eye,Vector3 center,Vector3 up);

#endif

