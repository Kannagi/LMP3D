#include <stdlib.h>
#include <stdio.h>

#ifdef PC

#ifdef EMSCRIPTEN
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "AURAE/AURAE.h"

SDL_Joystick *AURAE_joystick = NULL;
void AURAE_Init()
{

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_ShowCursor(0);
	SDL_JoystickEventState(SDL_ENABLE);
	AURAE_joystick = SDL_JoystickOpen(0);
}


#endif

