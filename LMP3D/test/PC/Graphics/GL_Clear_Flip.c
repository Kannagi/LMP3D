#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "LMP3D/LMP3D.h"

void LMP3D_Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{
	SDL_GL_SwapBuffers();
}

#endif

