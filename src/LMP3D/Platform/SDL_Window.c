#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#include <SDL/SDL.h>
#include "LMP3D/Window.h"

static LMP3D_FPS _LMP3D_time;


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
    int time_dif = 0,slp;
    slp = 1000000/fps;

    _LMP3D_time.end = clock();
    time_dif = _LMP3D_time.end - _LMP3D_time.begin;

    if(time_dif <= 0) time_dif = 0;
    if(time_dif < slp) usleep(slp - time_dif);

    _LMP3D_time.begin = clock();
}


int LMP3D_VBlank()
{
	LMP3D_Fps(60);
	SDL_GL_SwapBuffers();

	return 0;
}


#endif
