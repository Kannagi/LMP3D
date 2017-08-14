#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "LMP3D/Math.h"


void LMP3D_Math_Rotation3D(float rotx,float roty,float rotz,float *x,float *y,float *z)
{
    float tx,ty,tz,c,s;

    //x axis
    ty = *y,tz = *z;

    c = cos(rotx);
    s = sin(rotx);
    *y = ( c * ty ) - ( s * tz );
    *z = ( s * ty ) + ( c * tz );

    //y axis
    tx = *x,tz = *z;

    c = cos(roty);
    s = sin(roty);
    *z = ( c * tz ) - ( s * tx );
    *x = ( s * tz ) + ( c * tx );

    //z axis
    tx = *x,ty = *y;

    c = cos(-rotz);
    s = sin(-rotz);
    *y = ( c * ty ) - ( s * tx );
    *x = ( s * ty ) + ( c * tx );
}




void LMP3D_Math_Rotation_Position(float px,float py,float pz,float rotx,float roty,float rotz,float *posx,float *posy,float *posz)
{
    float tx,ty,tz,c,s;


    *posx -= px;
    *posy -= py;
    *posz -= pz;

    ty = *posy;
    tz = *posz;
    c = cos(rotx);
    s = sin(rotx);
    *posy = ( c * ty ) - ( s * tz );
    *posz = ( s * ty ) + ( c * tz );

    tx = *posx;
    ty = *posy;
    c = cos(roty);
    s = sin(roty);
    *posz = ( c * tz ) - ( s * tx );
    *posx = ( s * tz ) + ( c * tx );


    tx = *posx;
    tz = *posz;
    c = cos(rotz);
    s = sin(rotz);
    *posy = ( c * ty ) - ( s * tx );
    *posx = ( s * ty ) + ( c * tx );


    *posx += px;
    *posy += py;
    *posz += pz;


}

float LMP3D_Math_LawOfCosines(float ax,float bx,float cx,float ay,float by,float cy)
{
    float a,b,c,angle;

    a = LMP3D_Math_Hypotenuse2D( (ax - cx), (ay - cy) );
    b = LMP3D_Math_Hypotenuse2D( (bx - cx), (by - cy) );
    c = LMP3D_Math_Hypotenuse2D( (ax - bx), (ay - by) );

    angle = (a*a) + (b*b) - (c*c);
    angle = angle/(2*a*b);
    angle = acosf(angle);

    if(isnan(angle) == 1) angle = 0;

    return angle;
}

float LMP3D_Math_Hypotenuse3D(float x,float y,float z)
{
    float h;
    h = sqrtf( (x*x) + (z*z) );
    h = (h*h) + (y*y);
    return sqrtf(h);
}
