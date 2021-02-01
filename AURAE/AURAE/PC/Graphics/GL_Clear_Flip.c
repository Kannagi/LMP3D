#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "AURAE/AURAE.h"

void AURAE_Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AURAE_FlipBuffer(AURAE_Buffer *buffer)
{
	SDL_GL_SwapBuffers();
}

#endif

