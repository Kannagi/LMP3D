#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include <kernel.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/PS2/PS2.h"

static float matrixProjection[16];
void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{
	float matrixLookAt[16];
	LMP3D_MatrixPerspective(matrixProjection,camera.viewangle,camera.ratio,camera.min,camera.max);
	LMP3D_Camera_LookAt(matrixLookAt,camera.position,camera.vision,camera.top);

	LMP3D_MatrixMultiply(matrixProjection,matrixLookAt,matrixProjection);
}

float *LMP3D_MatrixProjection_Get()
{
	return matrixProjection;
}


void LMP3D_Camera_Ortho2D()
{

}

#endif

