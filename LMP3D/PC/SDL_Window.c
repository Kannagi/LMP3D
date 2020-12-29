#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#include <SDL/SDL.h>
#include "LMP3D/LMP3D.h"

void GL_Graphic_Init();


LMP3D_Buffer LMP3D_Window(char *name)
{
	SDL_WM_SetCaption(name, NULL);

	SDL_SetVideoMode(640,480, 24,SDL_OPENGL);

	GL_Graphic_Init();

	LMP3D_Buffer buffer;

	return buffer;

}

int LMP3D_Fps(int fps)
{
	static int time_end,time_begin;

	int time_dif,slp;
	slp = 1000/fps;
	slp +=1;
	time_end = SDL_GetTicks();
	time_dif = time_end - time_begin;

	if(time_dif <= 0) time_dif = 0;
	if(time_dif < slp) SDL_Delay(slp - time_dif);
	//printf("b %d\n",time_dif);
	time_begin = SDL_GetTicks();


	return time_dif;
}


int LMP3D_VBlank()
{
	int nvbl = LMP3D_Fps(60);


	return nvbl;
}


#endif
