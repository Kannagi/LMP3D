#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#include <SDL/SDL.h>
#include "LMP3D/Window.h"
#include <GL/gl.h>

static LMP3D_FPS _LMP3D_time;


LMP3D_Buffer LMP3D_Window(char *name)
{
	//SDL_Joystick *joystick = NULL;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    SDL_ShowCursor(0);
    //SDL_JoystickEventState(SDL_ENABLE);
    //joystick = SDL_JoystickOpen(0);

    SDL_WM_SetCaption(name, NULL);

    SDL_SetVideoMode(640,480, 24,SDL_OPENGL);


    glClearColor(0.5,0.5,0.5,0);
	glEnable(GL_TEXTURE_2D);


    glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_BLEND);

	LMP3D_Buffer buffer;

	return buffer;

    //glEnable(GL_CULL_FACE);
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

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{

}
int LMP3D_VBlank()
{
	LMP3D_Fps(60);
	SDL_GL_SwapBuffers();

	return 0;
}


#endif
