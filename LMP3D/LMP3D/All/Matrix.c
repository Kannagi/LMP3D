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


