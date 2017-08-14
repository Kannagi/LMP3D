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
	//glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_BLEND);



    //glEnable(GL_CULL_FACE);

}





#endif
