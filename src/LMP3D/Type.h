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
#define  PI 3.141592654


#endif
