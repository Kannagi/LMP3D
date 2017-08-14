#ifndef __LMP3D__
#define __LMP3D__

#include "LMP3D/Type.h"

#include "LMP3D/Graphics/Camera.h"
#include "LMP3D/Graphics/Model.h"
#include "LMP3D/Graphics/Texture.h"

#include "LMP3D/Window.h"
#include "LMP3D/Event.h"
#include "LMP3D/Load.h"
#include "LMP3D/Math.h"
#include "LMP3D/Script.h"


void LMP3D_Clear();

void LMP3D_Init();

LMP3D_Buffer LMP3D_Window(char *name);
void bitmap_font2(unsigned char *texte,int tx,int ty);

void LMP3D_MatrixRotateY(float* matrix, float angle);
void LMP3D_MatrixRotateX(float* matrix, float angle);
void LMP3D_MatrixRotateZ(float* matrix, float angle);
void LMP3D_MatrixTranslate(float* matrix, float x, float y, float z);
void LMP3D_MatrixScale(float* matrix, float xscale, float yscale, float zscale);
void LMP3D_MatrixProjection(float* matrix);
void LMP3D_MatrixMultiply(float* dest,const float* src1,const float* src2);

void LMP3D_MatrixIdentity(float* matrix);

LMP3D_Buffer LMP3D_Init_Graphic(int width, int height);

#define ftoi4(ft4) (int)(( (float)ft4 )*16.0f)
#define ftoi8(ft8) (int)(( (float)ft8 )*64.0f)
#define ftoi15(ft15) (int)(( (float)ft15 )*225.0f)

#define itof4(it4) (float)(( (float)it4 )/16.0f)
#define itof8(it8) (float)(( (float)it8 )/64.0f)
#define itof15(it15) (float)(( (float)it15 )/225.0f)

#endif


