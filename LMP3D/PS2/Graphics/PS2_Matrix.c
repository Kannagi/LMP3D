#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include "LMP3D/LMP3D.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define sqrtfast(var) asm ( "sqrt.s	%0,%0\n	" : "=f"(var): "f"(var));

void LMP3D_MatrixPerspective(float* matrix,float fovy, float aspect, float zNear, float zFar)
{
	matrix[(0<<2)+0] = 640.0f;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = -480.0f;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	float deltaZ=zFar-zNear;

	matrix[(2<<2)+0] = -2048.0f;
	matrix[(2<<2)+1] = -(2048.0f);
	matrix[(2<<2)+2] = -((zFar + zNear) / deltaZ)*(2048.0f-1024.0f);
	matrix[(2<<2)+3] = -1.0f;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = ((2.0f * zNear * zFar )/ deltaZ)*10000.0f;
	matrix[(3<<2)+3] = 0.0f;
}

void LMP3D_MatrixOrthogonal(float* matrix)
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

	matrix[(3<<2)+0] = 2048;
	matrix[(3<<2)+1] = 2048-128;
	matrix[(3<<2)+2] = 1024;
	matrix[(3<<2)+3] = 1;
}

void LMP3D_MatrixMultiply(float* dest,float* src1,float* src2)
{
  asm __volatile__(
    "lqc2            vf16,0x00(%1)\n"
    "lqc2            vf20,0x00(%2)\n"

    "lqc2            vf21,0x10(%2)\n"
    "lqc2            vf22,0x20(%2)\n"
    "lqc2            vf23,0x30(%2)\n"

    "lqc2            vf17,0x10(%1)\n"
    "lqc2            vf18,0x20(%1)\n"
    "lqc2            vf19,0x30(%1)\n"

    "vmulax          ACC,vf20,vf16\n"
    "vmadday         ACC,vf21,vf16\n"
    "vmaddaz         ACC,vf22,vf16\n"
    "vmaddw          vf16,vf23,vf16\n"

    "vmulax          ACC,vf20,vf17\n"
    "vmadday         ACC,vf21,vf17\n"
    "vmaddaz         ACC,vf22,vf17\n"
    "vmaddw          vf17,vf23,vf17\n"

    "vmulax          ACC,vf20,vf18\n"
    "vmadday         ACC,vf21,vf18\n"
    "vmaddaz         ACC,vf22,vf18\n"
    "vmaddw          vf18,vf23,vf18\n"

    "vmulax          ACC,vf20,vf19\n"
    "vmadday         ACC,vf21,vf19\n"
    "vmaddaz         ACC,vf22,vf19\n"
    "vmaddw          vf19,vf23,vf19\n"

    "sqc2            vf16,0x00(%0)\n"
    "sqc2            vf17,0x10(%0)\n"
    "sqc2            vf18,0x20(%0)\n"
    "sqc2            vf19,0x30(%0)\n"
    : : "r"(dest), "r"(src1), "r"(src2) : "memory");
}


#endif

