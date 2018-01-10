#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include "LMP3D/LMP3D.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define sqrtfast(var) asm ( "sqrt.s	%0,%0\n	" : "=f"(var): "f"(var));

void LookAt(float *matrix,float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz )
{
   float m[16];
   float x[3], y[3], z[3];
   float mag;



   /* Make rotation matrix */

   /* Z vector */
   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;


   mag = ( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );
   sqrtfast(mag);

   if (mag)
   {  /* mpichler, 19950515 */
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   //printf("n %f %f %f\n",z[0],z[1],z[2]);

/*
   mag = 1.0f/LMP3D_sqrtf( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );

   if (mag)
   {
      z[0] *=mag;
      z[1] *= mag;
      z[2] *= mag;
   }*/

   //printf("n %f %f %f\n",z[0],z[1],z[2]);

   /* Y vector */
   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   /* X vector = Y cross Z */
   x[0] =  y[1]*z[2] - y[2]*z[1];
   x[1] = -y[0]*z[2] + y[2]*z[0];
   x[2] =  y[0]*z[1] - y[1]*z[0];

   /* Recompute Y = Z cross X */
   y[0] =  z[1]*x[2] - z[2]*x[1];
   y[1] = -z[0]*x[2] + z[2]*x[0];
   y[2] =  z[0]*x[1] - z[1]*x[0];

   /* mpichler, 19950515 */
   /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */

   mag = ( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
   sqrtfast(mag);


   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = ( y[0]*y[0] + y[1]*y[1] + y[2]*y[2] );
   sqrtfast(mag);
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

	float matrixts[16];
	matrix[(0<<2)+0] = x[0];
	matrix[(0<<2)+1] = x[1];
	matrix[(0<<2)+2] = x[2];
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = y[0];
	matrix[(1<<2)+1] = y[1];
	matrix[(1<<2)+2] = y[2];
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = z[0];
	matrix[(2<<2)+1] = z[1];
	matrix[(2<<2)+2] = z[2];
	matrix[(2<<2)+3] = 0;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = 0;
	matrix[(3<<2)+3] = 1.0f;



	LMP3D_MatrixTS(matrixts, -eyex, -eyey, -eyez ,1,1,1);

	LMP3D_MatrixMultiply(matrix,matrixts, matrix);
/*
#define M(row,col)  m[col*4+row]
   M(0,0) = x[0];  M(0,1) = x[1];  M(0,2) = x[2];  M(0,3) = 0.0;
   M(1,0) = y[0];  M(1,1) = y[1];  M(1,2) = y[2];  M(1,3) = 0.0;
   M(2,0) = z[0];  M(2,1) = z[1];  M(2,2) = z[2];  M(2,3) = 0.0;
   M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
   glMultMatrixd( m );


   glTranslated( -eyex, -eyey, -eyez );*/

}



void LMP3D_MatrixProjection(float* matrix)
{
	matrix[(0<<2)+0] = 640;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = 480;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 2048;
	matrix[(2<<2)+1] = 2048-128;
	matrix[(2<<2)+2] = 2048;
	matrix[(2<<2)+3] = 1;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = 10000;
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
	matrix[(3<<2)+2] = 2048;
	matrix[(3<<2)+3] = 1;


}


void LMP3D_MatrixMultiply(float* dest,const float* src1,const float* src2)
{
  asm __volatile__(
    "lqc2            vf16,0x00(%1)\n"
    "lqc2            vf17,0x10(%1)\n"
    "lqc2            vf18,0x20(%1)\n"
    "lqc2            vf19,0x30(%1)\n"
    "lqc2            vf20,0x00(%2)\n"
    "lqc2            vf21,0x10(%2)\n"
    "lqc2            vf22,0x20(%2)\n"
    "lqc2            vf23,0x30(%2)\n"
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

