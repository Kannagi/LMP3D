#ifndef __LMP3D_Type__
#define __LMP3D_Type__

typedef struct
{
    float x,y;
}Vector2;

typedef struct
{
    int x,y;
}Vector2i;

typedef struct
{
    short x,y;
}Vector2s;

typedef struct
{
    float x,y,z;
}Vector3;

typedef struct
{
    int x,y,z;
}Vector3i;

typedef struct
{
    short x,y,z;
}Vector3s;

typedef struct
{
    float x,y,z,w;
}Vector4;

typedef struct
{
    int x,y,z,w;
}Vector4i;

typedef struct
{
    short x,y,z,w;
}Vector4s;

typedef struct
{
    Vector3 p,r;
}Matrix3x2;

typedef struct
{
    int x,y,h,w;
}Recti;

typedef struct
{
    short x,y,h,w;
}Rects;

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

inline Vector2i LMP3D_Type_Vector2i(int x,int y);
inline Vector3i LMP3D_Type_Vector3i(int x,int y,int z);
inline Recti LMP3D_Type_Recti(int x,int y,int w,int h);

inline Vector2s LMP3D_Type_Vector2s(int x,int y);
inline Vector3s LMP3D_Type_Vector3s(int x,int y,int z);
inline Rects LMP3D_Type_Rects(int x,int y,int w,int h);

inline Vector2 LMP3D_Type_Vector2(float x,float y);
inline Vector3 LMP3D_Type_Vector3(float x,float y,float z);
inline Rectf LMP3D_Type_Rectf(float x,float y,float w,float h);

#endif
