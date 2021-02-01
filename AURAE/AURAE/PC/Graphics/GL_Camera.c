#include <stdio.h>
#include <stdlib.h>
#ifdef PC

#include <GL/gl.h>

#include "AURAE/AURAE.h"

static float matrixProjection[16];
void AURAE_Camera_Perspective(AURAE_Camera camera)
{
	float matrixLookAt[16];
	AURAE_MatrixPerspective(matrixProjection,camera.viewangle,camera.ratio,camera.min,camera.max);
	AURAE_Camera_LookAt(matrixLookAt,camera.position,camera.vision,camera.top);

	AURAE_MatrixMultiply(matrixProjection,matrixLookAt,matrixProjection);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matrixProjection);
	glMatrixMode( GL_MODELVIEW );
}

float *AURAE_MatrixProjection_Get()
{
	return matrixProjection;
}


void AURAE_Camera_Ortho2D()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	float matrix[16];
	matrix[(0<<2)+0] = 1;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = 1;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 0;
	matrix[(2<<2)+1] = 0;
	matrix[(2<<2)+2] = 1;
	matrix[(2<<2)+3] = 0;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = 0;
	matrix[(3<<2)+3] = 1;

	glLoadMatrixf(matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matrix);
}

#endif

