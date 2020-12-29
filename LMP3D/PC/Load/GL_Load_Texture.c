#include <stdio.h>
#include <stdlib.h>
#include "LMP3D/LMP3D.h"

#ifdef PC
#include <GL/gl.h>

//GL_UNSIGNED_BYTE_3_3_2 /  GL_UNSIGNED_BYTE_2_3_3_REV /  GL_UNSIGNED_SHORT_5_6_5 /  GL_UNSIGNED_SHORT_4_4_4_4
void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
			texture->color = 1;
			texture->psm = 6409;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case LMP3D_FORMAT_LUMA:
			texture->color = 1;
			texture->psm = 6410;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case LMP3D_FORMAT_RGB888:
			texture->color = 3;
			texture->psm = GL_RGB;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case LMP3D_FORMAT_RGBA8888:
			texture->color = 4;
			texture->psm = GL_RGBA;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case LMP3D_FORMAT_RGB555:
			texture->color = 3;
			texture->psm = GL_RGB;
			texture->psm2 = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;

		case LMP3D_FORMAT_RGBA1555:
			texture->color = 4;
			texture->psm = GL_RGBA;
			texture->psm2 = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;

		case LMP3D_FORMAT_8BPP:
			texture->color = 1;
			texture->psm = 6409;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void LMP3D_Texture_Convert_Internal(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:

			 //
		break;

		case LMP3D_FORMAT_RGBA8888:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_RGBA1555);
		break;

		case LMP3D_FORMAT_RGB555:
		break;

		case LMP3D_FORMAT_RGBA1555:
		break;

		case LMP3D_FORMAT_8BPP:
			LMP3D_Texture_Convert(texture,LMP3D_FORMAT_RGBA1555);
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short LMP3D_Convert_Pixel(unsigned short pixel)
{
	return pixel;
}

#endif

