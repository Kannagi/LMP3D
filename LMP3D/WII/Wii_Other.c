
#include <stdio.h>
#include <stdlib.h>

#ifdef Wii

#include "LMP3D/LMP3D.h"



void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:
		break;

		case LMP3D_FORMAT_RGBA8888:
		break;

		case LMP3D_FORMAT_RGB555:

		break;

		case LMP3D_FORMAT_RGBA1555:

		break;

		case LMP3D_FORMAT_8BPP:
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}


void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{

}

void LMP3D_Convert_Model(LMP3D_Model *obj)
{

}


void LMP3D_Camera_Ortho2D()
{

}

//Clear Screen
void LMP3D_Clear()
{

}

//Switch Buffer
void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{

}

//Draw model 3D
void LMP3D_Model_Draw(LMP3D_Model *obj)
{

}

//----
void bitmap_font2(char *texte,int tx,int ty)
{

}

//RAM to VRAM
void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{

}

//malloc VRAM + LMP3D_Texture_Upload_VRAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{

}

//Select Texture
void LMP3D_Texture_Setup(LMP3D_Texture *texture)
{



}

//Delete Texture
void LMP3D_Texture_Delete(LMP3D_Texture *texture)
{

}

//Event update
void LMP3D_Event_Update(LMP3D_Event *event)
{

}

//Init Console or other
void LMP3D_Init()
{

}

//Size Screen , Window...
LMP3D_Buffer LMP3D_Window(char *name)
{


	LMP3D_Buffer buffer;

	return buffer;

}

//Wait VBlank
int LMP3D_VBlank()
{


	return 0;
}

#endif

