#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>

#include "AURAE/AURAE.h"

void GL_Graphic_Init()
{
	glClearColor(0.5,0.5,0.5,0);
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0f);

	//glCullFace(GL_FRONT);

	/*
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	*/


	//glEnable(GL_CULL_FACE);



}


void AURAE_VRAM_Set(unsigned int address)
{
	//DC_vram_pointer = address;
}

unsigned int AURAE_VRAM_Get()
{
	return 0;
}

void AURAE_MatrixMultiply(float* dest,float* src1,float* src2)
{
	int i;
	float x,y,z,w;
	for(i = 0;i < 4;i++)
	{
		x = src2[i+0];
		y = src2[i+4];
		z = src2[i+8];
		w = src2[i+12];

		dest[i+0]  = (src1[0 +0]*x) + (src1[0 +1]*y) + (src1[0 +2]*z) + (src1[0 +3]*w);
		dest[i+4]  = (src1[4 +0]*x) + (src1[4 +1]*y) + (src1[4 +2]*z) + (src1[4 +3]*w);
		dest[i+8]  = (src1[8 +0]*x) + (src1[8 +1]*y) + (src1[8 +2]*z) + (src1[8 +3]*w);
		dest[i+12] = (src1[12+0]*x) + (src1[12+1]*y) + (src1[12+2]*z) + (src1[12+3]*w);
	}
}

void AURAE_MatrixPerspective(float* MatrixProjection,float fovy, float aspect, float zNear, float zFar)
{
	float deltaZ =zFar-zNear;

	float ps = 640.0f/480.0f;

	MatrixProjection[(0<<2)+0] = ps;
	MatrixProjection[(0<<2)+1] = 0;
	MatrixProjection[(0<<2)+2] = 0;
	MatrixProjection[(0<<2)+3] = 0;

	MatrixProjection[(1<<2)+0] = 0;
	MatrixProjection[(1<<2)+1] = 2.0f;
	MatrixProjection[(1<<2)+2] = 0;
	MatrixProjection[(1<<2)+3] = 0;

	MatrixProjection[(2<<2)+0] = 0;
	MatrixProjection[(2<<2)+1] = 0;
	MatrixProjection[(2<<2)+2] = -(zFar + zNear) / deltaZ;
	MatrixProjection[(2<<2)+3] = -1.0f;

	MatrixProjection[(3<<2)+0] = 0;
	MatrixProjection[(3<<2)+1] = 0;
	MatrixProjection[(3<<2)+2] = -(2.0f * zNear * zFar )/ deltaZ;
	MatrixProjection[(3<<2)+3] = 0;
}

#endif
