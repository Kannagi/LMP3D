
#include <stdio.h>
#include <stdlib.h>

#ifdef Wii

#include "AURAE/AURAE.h"



void AURAE_Texture_Format_Convert(AURAE_Texture *texture)
{
	switch (texture->format)
	{
		case AURAE_FORMAT_LUM:
		break;

		case AURAE_FORMAT_LUMA:
		break;

		case AURAE_FORMAT_RGB888:
		break;

		case AURAE_FORMAT_RGBA8888:
		break;

		case AURAE_FORMAT_RGB555:

		break;

		case AURAE_FORMAT_RGBA1555:

		break;

		case AURAE_FORMAT_8BPP:
		break;

		case AURAE_FORMAT_4BPP:

		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}


void AURAE_Camera_Perspective(AURAE_Camera camera)
{

}

void AURAE_Convert_Model(AURAE_Model *obj)
{

}


void AURAE_Camera_Ortho2D()
{

}

//Clear Screen
void AURAE_Clear()
{

}

//Switch Buffer
void AURAE_FlipBuffer(AURAE_Buffer *buffer)
{

}

//Draw model 3D
void AURAE_Model_Draw(AURAE_Model *obj)
{

}

//----
void bitmap_font2(char *texte,int tx,int ty)
{

}

//RAM to VRAM
void AURAE_Texture_Upload_VRAM(AURAE_Texture *texture)
{

}

//malloc VRAM + AURAE_Texture_Upload_VRAM
void AURAE_Texture_Upload(AURAE_Texture *texture)
{

}

//Select Texture
void AURAE_Texture_Setup(AURAE_Texture *texture)
{



}

//Delete Texture
void AURAE_Texture_Delete(AURAE_Texture *texture)
{

}

//Event update
void AURAE_Event_Update(AURAE_Event *event)
{

}

//Init Console or other
void AURAE_Init()
{

}

//Size Screen , Window...
AURAE_Buffer AURAE_Window(char *name)
{


	AURAE_Buffer buffer;

	return buffer;

}

//Wait VBlank
int AURAE_VBlank()
{


	return 0;
}

#endif

