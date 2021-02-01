#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#ifdef EMSCRIPTEN
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "AURAE/AURAE.h"

void GL_Graphic_Init();

#include <GL/gl.h>
AURAE_Buffer AURAE_Window(char *name)
{
#ifdef EMSCRIPTEN
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;
	SDL_CreateWindowAndRenderer(512, 512, SDL_WINDOW_OPENGL, &window, &renderer);
#else
	SDL_WM_SetCaption(name, NULL);
	SDL_Surface *screen = SDL_SetVideoMode(640,480, 16,SDL_OPENGL);
	if ( !screen ) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}
#endif

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

	GL_Graphic_Init();

	AURAE_Buffer buffer;

	return buffer;

}

int AURAE_Fps(int fps)
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


int AURAE_VBlank()
{
	int nvbl = AURAE_Fps(60);


	return nvbl;
}


#endif
