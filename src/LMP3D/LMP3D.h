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

#endif


