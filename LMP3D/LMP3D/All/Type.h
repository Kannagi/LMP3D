#ifndef __LMP3D_Type__
#define __LMP3D_Type__

typedef struct
{
    float x,y;
}Vector2;

typedef struct
{
    int x,y;
}IVector2;

typedef struct
{
    float x,y,z;
}Vector3;

typedef struct
{
    int x,y,z;
}IVector3;

typedef struct
{
    float x,y,z,w;
}Vector4;

typedef struct
{
    int x,y,z,w;
}IVector4;

typedef struct
{
    Vector3 p,r;
}Matrix3x2;

typedef struct
{
    int x,y,h,w;
}Rect;

typedef struct
{
    float x,y,h,w;
}Rectf;

typedef char LMP3D_STRING[100];

#define ftoi4(ft4) (int)(( (float)ft4 )*16.0f)
#define ftoi8(ft8) (int)(( (float)ft8 )*256.0f)
#define ftoi12(ft12) (int)(( (float)ft12 )*4096.0f)
#define ftoi15(ft15) (int)(( (float)ft15 )*32768.0f)

#define itof4(it4) (float)(( (float)it4 )/16.0f)
#define itof8(it8) (float)(( (float)it8 )/256.0f)
#define itof12(it12) (float)(( (float)it12 )/4096.0f)
#define itof15(it15) (float)(( (float)it15 )/32768.0f)

#define  PI 3.141592654


#endif
