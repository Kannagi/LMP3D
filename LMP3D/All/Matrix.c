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
	for(i = 0;i < 16;i+=4)
	{
		x = src2[i+0];
		y = src2[i+1];
		z = src2[i+2];
		w = src2[i+3];

		dest[i+0] = (src1[0+0]*x) + (src1[4+0]*y) + (src1[8+0]*z) + (src1[12+0]*w);
		dest[i+1] = (src1[0+1]*x) + (src1[4+1]*y) + (src1[8+1]*z) + (src1[12+1]*w);
		dest[i+2] = (src1[0+2]*x) + (src1[4+2]*y) + (src1[8+2]*z) + (src1[12+2]*w);
		dest[i+3] = (src1[0+3]*x) + (src1[4+3]*y) + (src1[8+3]*z) + (src1[12+3]*w);
	}
}

void LMP3D_MatrixMultiply2(float* dest,float* src1,float* src2)
{
	int i;
	float x,y,z,w;
	float accx,accy,accz,accw;

	float matrix[32];
	for(i = 0;i < 16;i+=4)
	{
		x = src1[i+0];
		y = src1[i+1];
		z = src1[i+2];
		w = src1[i+3];

		accx = (src2[0+0]*x);
		accy = (src2[0+1]*x);
		accz = (src2[0+2]*x);
		accw = (src2[0+3]*x);

		accx += (src2[4+0]*y);
		accy += (src2[4+1]*y);
		accz += (src2[4+2]*y);
		accw += (src2[4+3]*y);

		accx += (src2[8+0]*z);
		accy += (src2[8+1]*z);
		accz += (src2[8+2]*z);
		accw += (src2[8+3]*z);

		matrix[i+0] = accx + (src2[12+0]*w);
		matrix[i+1] = accy + (src2[12+1]*w);
		matrix[i+2] = accz + (src2[12+2]*w);
		matrix[i+3] = accw + (src2[12+3]*w);

	}

	for(i = 0;i < 16;i+=4)
	{
		dest[i+0] = (matrix[i+0]);
		dest[i+1] = (matrix[i+1]);
		dest[i+2] = (matrix[i+2]);
		dest[i+3] = (matrix[i+3]);

	}
}



void LMP3D_MatrixVec3MultiplyFPU(float* src1,float* src2)
{
	float x,y,z;
	float accx,accy,accz,accw;

	x = src1[0];
	y = src1[1];
	z = src1[2];

	accx = (src2[0+0]*x);
	accy = (src2[0+1]*x);
	accz = (src2[0+2]*x);
	accw = (src2[0+3]*x);

	accx += (src2[4+0]*y);
	accy += (src2[4+1]*y);
	accz += (src2[4+2]*y);
	accw += (src2[4+3]*y);

	accx += (src2[8+0]*z);
	accy += (src2[8+1]*z);
	accz += (src2[8+2]*z);
	accw += (src2[8+3]*z);

	src1[0] = accx + src2[12+0];
	src1[1] = accy + src2[12+1];
	src1[2] = accz + src2[12+2];
}

Vector4 LMP3D_MatrixVecMultiplyFPU(float* matrix,Vector4 vec)
{
	register float x,y,z,w;

	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;

	vec.x = (matrix[0+0]*x) + (matrix[4+0]*y) + (matrix[8+0]*z) + (matrix[12+0]*w);
	vec.y = (matrix[0+1]*x) + (matrix[4+1]*y) + (matrix[8+1]*z) + (matrix[12+1]*w);
	vec.z = (matrix[0+2]*x) + (matrix[4+2]*y) + (matrix[8+2]*z) + (matrix[12+2]*w);
	vec.w = (matrix[0+3]*x) + (matrix[4+3]*y) + (matrix[8+3]*z) + (matrix[12+3]*w);

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
	w	 = (matrix[0+3]*x) + (matrix[4+3]*y) + (matrix[8+3]*z) + (matrix[12+3]);

	if(w != 1.0f)
	{
		w = 1.0f/w;

		vec.x *= w;
		vec.y *= w;
		vec.z *= w;
	}

	return vec;
}

/*
void mtxf_rotate_xyz_and_translate(Mat4 dest, Vec3f b, Vec3s c) {
	register f32 sx = sins(c[0]);
	register f32 cx = coss(c[0]);

	register f32 sy = sins(c[1]);
	register f32 cy = coss(c[1]);

	register f32 sz = sins(c[2]);
	register f32 cz = coss(c[2]);

	dest[0][0] = cy * cz;
	dest[0][1] = cy * sz;
	dest[0][2] = -sy;
	dest[0][3] = 0;

	dest[1][0] = sx * sy * cz - cx * sz;
	dest[1][1] = sx * sy * sz + cx * cz;
	dest[1][2] = sx * cy;
	dest[1][3] = 0;

	dest[2][0] = cx * sy * cz + sx * sz;
	dest[2][1] = cx * sy * sz - sx * cz;
	dest[2][2] = cx * cy;
	dest[2][3] = 0;

	dest[3][0] = b[0];
	dest[3][1] = b[1];
	dest[3][2] = b[2];
	dest[3][3] = 1;
}*/


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
			LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.z);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}
	}else
	{
		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix, model->rotate.x);

			if(model->rotate.z != 0)
			{
				LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.z);
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

