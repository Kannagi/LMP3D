#ifndef __LMP3D_Math__
#define __LMP3D_Math__

#include "LMP3D/All/Type.h"


float LMP3D_sinf(float x);
double LMP3D_sind(double x);
float LMP3D_cosf(float x);
double LMP3D_cosd(double x);

float LMP3D_sqrtf(float s);
float LMP3D_invsqrtf(float s);

unsigned int LMP3D_sqrti(const unsigned int s);
unsigned int LMP3D_sqrti4(const unsigned int s);
unsigned int LMP3D_sqrti8(const unsigned int s);
unsigned int LMP3D_sqrti12(const unsigned int s);

#ifndef PI
#define PI 3.14159265358979323846
#endif



#endif

