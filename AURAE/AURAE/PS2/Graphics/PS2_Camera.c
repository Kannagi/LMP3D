#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include <kernel.h>

#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"

static float matrixProjection[16];
void AURAE_Camera_Perspective(AURAE_Camera camera)
{
	float matrixLookAt[16];
	AURAE_MatrixPerspective(matrixProjection,camera.viewangle,camera.ratio,camera.min,camera.max);
	AURAE_Camera_LookAt(matrixLookAt,camera.position,camera.vision,camera.top);

	AURAE_MatrixMultiply(matrixProjection,matrixLookAt,matrixProjection);
}

float *AURAE_MatrixProjection_Get()
{
	return matrixProjection;
}


void AURAE_Camera_Ortho2D()
{

}

#endif

