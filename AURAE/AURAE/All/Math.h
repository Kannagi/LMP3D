#ifndef __AURAE_Math__
#define __AURAE_Math__

#include "AURAE/All/Type.h"


float AURAE_sinf(float x);
double AURAE_sind(double x);
float AURAE_cosf(float x);
double AURAE_cosd(double x);

float AURAE_sqrtf(float s);
float AURAE_invsqrtf(float s);

unsigned int AURAE_sqrti(const unsigned int s);
unsigned int AURAE_sqrti4(const unsigned int s);
unsigned int AURAE_sqrti8(const unsigned int s);
unsigned int AURAE_sqrti12(const unsigned int s);

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef DPI
#define DPI 3.14159265358979323846
#endif

#endif

