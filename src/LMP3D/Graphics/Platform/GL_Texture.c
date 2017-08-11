#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"


void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{
    glTexImage2D (
    GL_TEXTURE_2D, 	//Type : texture 2D
    0, 	//Mipmap : aucun
    texture->internalformat, 	//Couleurs : 4
    texture->w, 	//Largeur
    texture->h, 	//Hauteur
    0, 	//Largeur du bord : 0
    texture->format,
    GL_UNSIGNED_BYTE, 	//Type des couleurs
    texture->pixel 	//Addresse de l'texture
    );
}

void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{
	if(texture == NULL) return;

    glGenTextures(1,&texture->id);

    glBindTexture(GL_TEXTURE_2D,texture->id);

    LMP3D_Texture_Upload_VRAM(texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


}


void LMP3D_Texture_Setup(LMP3D_Texture *texture)
{
	if(texture == NULL) return;

    glBindTexture(GL_TEXTURE_2D,texture->id);


}


void LMP3D_Texture_Delete(int id)
{
    GLuint tid = (GLuint)id;
    glDeleteTextures(1,&tid);
}

#endif

