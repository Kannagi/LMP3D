#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

void LMP3D_MatrixIdentity(float* matrix)
{
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
}

void LMP3D_MatrixRotateY(float* matrix, float angle)
{
  float sinAngle = LMP3D_sinf(angle);
  float cosAngle = LMP3D_cosf(angle);

  matrix[(0<<2)+0] = cosAngle;
  matrix[(0<<2)+1] = 0;
  matrix[(0<<2)+2] = -sinAngle;
  matrix[(0<<2)+3] = 0;

  matrix[(1<<2)+0] = 0;
  matrix[(1<<2)+1] = 1;
  matrix[(1<<2)+2] = 0;
  matrix[(1<<2)+3] = 0;

  matrix[(2<<2)+0] = sinAngle;
  matrix[(2<<2)+1] = 0;
  matrix[(2<<2)+2] = cosAngle;
  matrix[(2<<2)+3] = 0;

  matrix[(3<<2)+0] = 0;
  matrix[(3<<2)+1] = 0;
  matrix[(3<<2)+2] = 0;
  matrix[(3<<2)+3] = 1;
}

void LMP3D_MatrixRotateX(float* matrix, float angle)
{
  float sinAngle = LMP3D_sinf(angle);
  float cosAngle = LMP3D_cosf(angle);

  matrix[(0<<2)+0] = 1;
  matrix[(0<<2)+1] = 0;
  matrix[(0<<2)+2] = 0;
  matrix[(0<<2)+3] = 0;

  matrix[(1<<2)+0] = 0;
  matrix[(1<<2)+1] = cosAngle;
  matrix[(1<<2)+2] = sinAngle;
  matrix[(1<<2)+3] = 0;

  matrix[(2<<2)+0] = 0;
  matrix[(2<<2)+1] = -sinAngle;
  matrix[(2<<2)+2] = cosAngle;
  matrix[(2<<2)+3] = 0;

  matrix[(3<<2)+0] = 0;
  matrix[(3<<2)+1] = 0;
  matrix[(3<<2)+2] = 0;
  matrix[(3<<2)+3] = 1;
}


void LMP3D_MatrixRotateZ(float* matrix, float angle)
{
	float sinAngle = LMP3D_sinf(angle);
	float cosAngle = LMP3D_cosf(angle);

	matrix[(0<<2)+0] = cosAngle;
	matrix[(0<<2)+1] = sinAngle;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = -sinAngle;
	matrix[(1<<2)+1] = cosAngle;
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
}


void LMP3D_MatrixTranslate(float* matrix, float x, float y, float z)
{
	matrix[(3<<2)+0] = x;
	matrix[(3<<2)+1] = y;
	matrix[(3<<2)+2] = z;
}

void LMP3D_MatrixScale(float* matrix, float xscale, float yscale, float zscale)
{
	matrix[(0<<2)+0] = xscale;
	matrix[(1<<2)+1] = yscale;
	matrix[(2<<2)+2] = zscale;
	matrix[(3<<2)+3] = 1;
}


void LMP3D_MatrixTS(float* matrix, float x, float y, float z, float xscale, float yscale, float zscale)
{
	matrix[(0<<2)+0] = xscale;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = yscale;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 0;
	matrix[(2<<2)+1] = 0;
	matrix[(2<<2)+2] = zscale;
	matrix[(2<<2)+3] = 0;

	matrix[(3<<2)+0] = x;
	matrix[(3<<2)+1] = y;
	matrix[(3<<2)+2] = z;
	matrix[(3<<2)+3] = 1;
}

void LMP3D_MatrixMultiplyFPU(float* dest,float* src1,float* src2)
{
	int i;
	register float x,y,z,w;

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

Vector4 LMP3D_MatrixVecMultiplyFPU(float* matrix,Vector4 vec)
{
	register float x,y,z,w;

	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;

	vec.x  = (matrix[0 +0]*x) + (matrix[0 +1]*y) + (matrix[0 +2]*z) + (matrix[0 +3]*w);
	vec.y  = (matrix[4 +0]*x) + (matrix[4 +1]*y) + (matrix[4 +2]*z) + (matrix[4 +3]*w);
	vec.z  = (matrix[8 +0]*x) + (matrix[8 +1]*y) + (matrix[8 +2]*z) + (matrix[8 +3]*w);
	vec.w  = (matrix[12+0]*x) + (matrix[12+1]*y) + (matrix[12+2]*z) + (matrix[12+3]*w);

	return vec;
}


Vector3 LMP3D_TransformPerspectiveFPU(float* matrix,Vector3 vec)
{
	register float x,y,z,w;

	x = vec.x;
	y = vec.y;
	z = vec.z;

	vec.x = (matrix[0+0]*x) + (matrix[4+0]*y) + (matrix[8+0]*z) + (matrix[12+0]);
	vec.y = (matrix[0+1]*x) + (matrix[4+1]*y) + (matrix[8+1]*z) + (matrix[12+1]);
	vec.z = (matrix[0+2]*x) + (matrix[4+2]*y) + (matrix[8+2]*z) + (matrix[12+2]);
	w     = (matrix[0+3]*x) + (matrix[4+3]*y) + (matrix[8+3]*z) + (matrix[12+3]);

	if(w != 1.0f)
	{
		w = 1.0f/w;

		vec.x *= w;
		vec.y *= w;
		vec.z *= w;
	}

	return vec;
}

void LMP3D_MatrixView(LMP3D_Model *model,float * matrixFinal)
{
	float rotateMatrix[16],rotateMatrix2[16];
	int rotate = 1;

	if(model->rotate.y != 0)
	{
		LMP3D_MatrixRotateY(rotateMatrix, model->rotate.y);

		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}

		if(model->rotate.z != 0)
		{
			LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}
	}else
	{
		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix, model->rotate.x);

			if(model->rotate.z != 0)
			{
				LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
				LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
			}
		}else
		{
			if(model->rotate.z != 0)
				LMP3D_MatrixRotateZ(rotateMatrix, model->rotate.z);
			else rotate = 0;
		}
	}

	//T+S
	LMP3D_MatrixTS(matrixFinal, model->position.x, model->position.y,model->position.z, model->scale.x, model->scale.y, model->scale.z);

	if(rotate == 1)
	{
		//T+S+R
		LMP3D_MatrixMultiply(matrixFinal,rotateMatrix, matrixFinal);
	}
}

