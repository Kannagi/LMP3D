#include <stdio.h>
#include <stdlib.h>
#include "AURAE/AURAE.h"

#ifdef PC
#include <GL/gl.h>

//GL_UNSIGNED_BYTE_3_3_2 /  GL_UNSIGNED_BYTE_2_3_3_REV /  GL_UNSIGNED_SHORT_5_6_5 /  GL_UNSIGNED_SHORT_4_4_4_4
void AURAE_Texture_Format_Convert(AURAE_Texture *texture)
{
	//printf("%d\n",GL_UNSIGNED_SHORT_1_5_5_5_REV);
	switch (texture->format)
	{
		case AURAE_FORMAT_LUM:
			texture->color = 1;
			texture->psm = 6409;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case AURAE_FORMAT_LUMA:
			texture->color = 1;
			texture->psm = 6410;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case AURAE_FORMAT_RGB888:
			texture->color = 3;
			texture->psm = GL_RGB;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case AURAE_FORMAT_RGBA8888:
			texture->color = 4;
			texture->psm = GL_RGBA;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case AURAE_FORMAT_RGB555:
			texture->color = 3;
			texture->psm = GL_RGB;
			//texture->psm2 = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			texture->psm2 = 33638;
		break;

		case AURAE_FORMAT_RGBA1555:
			texture->color = 4;
			texture->psm = GL_RGBA;
			//texture->psm2 = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			texture->psm2 = 33638;
		break;

		case AURAE_FORMAT_8BPP:
			texture->color = 1;
			texture->psm = 6409;
			texture->psm2 = GL_UNSIGNED_BYTE;
		break;

		case AURAE_FORMAT_4BPP:

		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

void AURAE_Texture_Convert_Internal(AURAE_Texture *texture)
{
	switch (texture->format)
	{
		case AURAE_FORMAT_LUM:
		break;

		case AURAE_FORMAT_LUMA:
		break;

		case AURAE_FORMAT_RGB888:

			 //
		break;

		case AURAE_FORMAT_RGBA8888:
			AURAE_Texture_Convert(texture,AURAE_FORMAT_RGBA1555);
		break;

		case AURAE_FORMAT_RGB555:
		break;

		case AURAE_FORMAT_RGBA1555:
		break;

		case AURAE_FORMAT_8BPP:
			AURAE_Texture_Convert(texture,AURAE_FORMAT_RGBA1555);
		break;

		case AURAE_FORMAT_4BPP:

		break;

		case AURAE_FORMAT_2BPP:

		break;

		default:

		break;
	}

}

unsigned short AURAE_Convert_Pixel(unsigned short pixel)
{
	return pixel;
}

#endif

