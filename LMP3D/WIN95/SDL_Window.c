#include <stdlib.h>
#include <stdio.h>

#ifdef WINDOWS95

#include <SDL/SDL.h>
#include "LMP3D/LMP3D.h"

static SDL_Surface *ecran,*window;

LMP3D_Buffer LMP3D_Window(char *name)
{
	SDL_WM_SetCaption(name, NULL);

	ecran = SDL_SetVideoMode(256, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF );
	//window = SDL_CreateRGBSurface(0,256,240,24,0,0,0,0);

	window = ecran;

	LMP3D_Buffer buffer;

	return buffer;

}

//Clear Screen
void LMP3D_Clear()
{
	int *pixels = window->pixels;
	int i;

	for(i = 0;i < (128*240);i+=4)
	{

		pixels[i] = 0x84108410;
		pixels[i+1] = 0x84108410;
		pixels[i+2] = 0x84108410;
		pixels[i+3] = 0x84108410;


	}


	//SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,128,128,128));
}

void *LMP3D_FrameBuffer_WIN95()
{
	return window->pixels;
}


int LMP3D_Fps()
{
	static time_end,time_begin;

	int time_dif,slp;
	slp = 60;

	time_end = SDL_GetTicks();
	time_dif = time_end - time_begin;

	if(time_dif <= 0) time_dif = 0;
	if(time_dif < slp) SDL_Delay(slp - time_dif);

	time_begin = SDL_GetTicks();

	return time_dif;
}


int LMP3D_VBlank()
{

	int nvbl = LMP3D_Fps();

	//SDL_BlitSurface(window,NULL,ecran,NULL);
/*
	unsigned int *inpixels = window->pixels;
	unsigned int *pixels = ecran->pixels;
	int i;

	for(i = 0;i < (128*240);i+=4)
	{

		pixels[i] = inpixels[i];
		pixels[i+1] = inpixels[i+1];
		pixels[i+2] = inpixels[i+2];
		pixels[i+3] = inpixels[i+3];
	}
*/

	SDL_Flip(ecran);

	return nvbl;
}


#endif
