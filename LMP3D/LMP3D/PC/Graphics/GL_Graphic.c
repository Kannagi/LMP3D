#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"

void GL_Graphic_Init()
{
	glClearColor(0.5,0.5,0.5,0);
	glEnable(GL_TEXTURE_2D);


    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0f);

/*
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
*/


    //glEnable(GL_CULL_FACE);

}





#endif
