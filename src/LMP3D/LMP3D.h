#ifndef __LMP3D__
#define __LMP3D__

#include "LMP3D/All/Type.h"

#include "LMP3D/All/Graphics/Texture.h"
#include "LMP3D/All/Graphics/Camera.h"
#include "LMP3D/All/Graphics/Model.h"


#include "LMP3D/All/Event.h"
#include "LMP3D/All/Load.h"
#include "LMP3D/All/Math.h"
#include "LMP3D/All/Matrix.h"
#include "LMP3D/All/Script.h"
#include "LMP3D/All/Window.h"

void LMP3D_Clear();
void LMP3D_Init();
void bitmap_font2(char *texte,int tx,int ty);

void LMP3D_Convert_Model(LMP3D_Model *model);
void LMP3D_Format_Init(LMP3D_Texture *texture);



#endif


