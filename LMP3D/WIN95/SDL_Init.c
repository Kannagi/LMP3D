#include <stdlib.h>
#include <stdio.h>

#ifdef WINDOWS95

#include <SDL/SDL.h>
#include "LMP3D/LMP3D.h"

void LMP3D_Init()
{
	//SDL_Joystick *joystick = NULL;
	SDL_Init(SDL_INIT_VIDEO);
/*
	SDL_ShowCursor(0);
	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);*/
}


#endif

