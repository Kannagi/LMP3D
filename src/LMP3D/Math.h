#ifndef __LMP3D_Math__
#define __LMP3D_Math__


#include "LMP3D/Type.h"

void  LMP3D_Math_Rotation3D(float rotx,float roty,float rotz,float *x,float *y,float *z);
float LMP3D_Math_LawOfCosines(float ax,float bx,float cx,float ay,float by,float cy);
float LMP3D_Math_Hypotenuse3D(float x,float y,float z);

#define LMP3D_Math_Hypotenuse2D(_x_,_y_) sqrtf((_x_*_x_) + (_y_*_y_))

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#endif

