#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#include <SDL/SDL.h>
#include "LMP3D/LMP3D.h"




LMP3D_Buffer LMP3D_Window(char *name)
{
    SDL_WM_SetCaption(name, NULL);

    SDL_SetVideoMode(640,480, 24,SDL_OPENGL);

	GL_Graphic_Init();

	LMP3D_Buffer buffer;

	return buffer;

}

void LMP3D_Fps(int fps)
{
	static LMP3D_FPS time;

    int time_dif,slp;
    slp = 1000/fps;

    time.end = SDL_GetTicks();
    time_dif = time.end - time.begin;

    if(time_dif <= 0) time_dif = 0;
    if(time_dif < slp) SDL_Delay(slp - time_dif);

    time.begin = SDL_GetTicks();
}


int LMP3D_VBlank()
{
	LMP3D_Fps(60);
	SDL_GL_SwapBuffers();

	return 0;
}


#endif
