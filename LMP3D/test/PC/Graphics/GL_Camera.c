#include <stdio.h>
#include <stdlib.h>
#ifdef PC

#include <GL/gl.h>

#include "LMP3D/LMP3D.h"

static float matrixProjection[16];
void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{
	float matrixLookAt[16];
	LMP3D_MatrixPerspective(matrixProjection,camera.viewangle,camera.ratio,camera.min,camera.max);
	LMP3D_Camera_LookAt(matrixLookAt,camera.position,camera.vision,camera.top);

	LMP3D_MatrixMultiply(matrixProjection,matrixLookAt,matrixProjection);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matrixProjection);
	glMatrixMode( GL_MODELVIEW );
}

float *LMP3D_MatrixProjection_Get()
{
	return matrixProjection;
}

void LMP3D_Camera_Ortho2D()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
}

#endif

