#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#include <SDL/SDL.h>
#include "LMP3D/LMP3D.h"

void LMP3D_Init()
{
	//SDL_Joystick *joystick = NULL;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_ShowCursor(0);
	//SDL_JoystickEventState(SDL_ENABLE);
	//joystick = SDL_JoystickOpen(0);
}


#endif

