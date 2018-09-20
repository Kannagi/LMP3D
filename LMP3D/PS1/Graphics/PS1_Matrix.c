#include <stdio.h>

#ifdef PLAYSTATION1

#include <psx.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/PS1/PS1.h"

extern short sincostbl[4096];
#define fastsin(v) sincostbl[v&0xFFF];
#define fastcos(v) sincostbl[(v+1024)&0xFFF];

void LMP3D_MatrixRotateXS(short *matrix,short rotate)
{
	short cosAngle = fastcos(rotate);
	short sinAngle = fastsin(rotate);
	matrix[0+0] = 1<<12;
	matrix[0+1] = 0<<12;
	matrix[0+2] = 0<<12;

	matrix[3+0] = 0<<12;
	matrix[3+1] = cosAngle;
	matrix[3+2] = sinAngle;

	matrix[6+0] = 0<<12;
	matrix[6+1] = -sinAngle;
	matrix[6+2] = cosAngle;
}

void LMP3D_MatrixRotateYS(short *matrix,short rotate)
{
	short cosAngle = fastcos(rotate);
	short sinAngle = fastsin(rotate);
	matrix[0+0] = cosAngle;
	matrix[0+1] = 0<<12;
	matrix[0+2] = -sinAngle;

	matrix[3+0] = 0<<12;
	matrix[3+1] = 1<<12;
	matrix[3+2] = 0<<12;

	matrix[6+0] = sinAngle;
	matrix[6+1] = 0<<12;
	matrix[6+2] = cosAngle;
}

void LMP3D_MatrixRotateZS(short *matrix,short rotate)
{
	short cosAngle = fastcos(rotate);
	short sinAngle = fastsin(rotate);
	matrix[0+0] = cosAngle;
	matrix[0+1] = sinAngle;
	matrix[0+2] = 0<<12;

	matrix[3+0] = -sinAngle;
	matrix[3+1] = cosAngle;
	matrix[3+2] = 0<<12;

	matrix[6+0] = 0<<12;
	matrix[6+1] = 0<<12;
	matrix[6+2] = 1<<12;

	matrix[6+3] = 0;
}

void LMP3D_MatrixMultiply(float* dest,float* src1,float* src2)
{

}

#endif



