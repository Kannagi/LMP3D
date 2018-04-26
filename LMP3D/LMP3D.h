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
void LMP3D_Texture_Format_Init(LMP3D_Texture *texture);


typedef struct
{
    unsigned char *buffer;
    int size,type,offset;
}LMP3D_TAR;

#define LMP3D_TAR_DEBUG		0x01
#define LMP3D_TAR_INFO		0x02
#define LMP3D_TAR_BUFFER	0x04
#define LMP3D_TAR_OFFSET	0x08

void LMP3D_Tar(LMP3D_TAR *tar,char *pathtar,char *filename,int option,void *buffer,int size);


void *LMP3D_fopen(char *path,char *mode,void *buffer,int size);
int LMP3D_fclose(void *file);
int LMP3D_fseek(void *file,int offset,int whence);
int LMP3D_fread(void *ptr,int size,int nmemb,void *file);
int LMP3D_ftell(void *file);
int LMP3D_fgetc(void *file);

#endif


