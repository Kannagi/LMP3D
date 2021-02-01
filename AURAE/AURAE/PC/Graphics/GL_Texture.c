#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>

#include "AURAE/AURAE.h"


void AURAE_Texture_Upload_VRAM(AURAE_Texture *texture)
{
	glBindTexture(GL_TEXTURE_2D,texture->address);
	glTexImage2D (
	GL_TEXTURE_2D, 	//Type : texture 2D
	0, 	//Mipmap : aucun
	texture->color, 	//Couleurs : 4
	texture->w, 	//Largeur
	texture->h, 	//Hauteur
	0, 	//Largeur du bord : 0
	texture->psm,
	texture->psm2, 	//Type des couleurs
	texture->pixel 	//Addresse de l'texture
	);
}

void AURAE_Texture_Upload(AURAE_Texture *texture)
{
	if(texture == NULL) return;

	glGenTextures(1,&texture->address);

	AURAE_Texture_Upload_VRAM(texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}


void AURAE_Texture_Setup(AURAE_Texture *texture)
{
	if(texture == NULL) return;

	glBindTexture(GL_TEXTURE_2D,texture->address);
}


void AURAE_Texture_Free_VRAM(AURAE_Texture *texture)
{
	GLuint tid = (GLuint)texture->address;
	glDeleteTextures(1,&tid);
}

void AURAE_VRAM_Info(char *info)
{
	//int vram = (PS2_vram_pointer&0xFFFFFF);
	//sprintf(info,"vram :%x/0x100000\n",vram);
}

#endif

