#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#ifdef EMSCRIPTEN
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "LMP3D/LMP3D.h"

SDL_Joystick *LMP3D_joystick = NULL;
void LMP3D_Init()
{

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_ShowCursor(0);
	SDL_JoystickEventState(SDL_ENABLE);
	LMP3D_joystick = SDL_JoystickOpen(0);
}


#endif

