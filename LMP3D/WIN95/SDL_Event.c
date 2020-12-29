#include <stdlib.h>
#include <stdio.h>

#ifdef WINDOWS95

#include <SDL/SDL.h>

#include "LMP3D/LMP3D.h"

void LMP3D_Event_Update(LMP3D_Event *event)
{
	SDL_Event sdlevent;
	int i;

	event->clikright = 0;
	event->clikleft = 0;

	event->exit = 0;

	for(i = 0;i < LMP3D_EVENT_MAX;i++)
	{
		if(event->key[i] == 1)
			event->key[i] = 2;

		if(event->key[i] == 3)
			event->key[i] = 0;
	}




	while(SDL_PollEvent(&sdlevent) == 1)
	{

		switch(sdlevent.type)
		{
			case SDL_MOUSEMOTION :
				event->mousex = sdlevent.motion.x;
				event->mousey = sdlevent.motion.y;
			break;

			case SDL_QUIT:
				event->exit = 1;
			break;

			case SDL_MOUSEBUTTONDOWN:
				 switch(sdlevent.button.button)
				 {
					 case SDL_BUTTON_RIGHT:
						  event->clikright = 1;
					 break;

					 case SDL_BUTTON_LEFT:
						  event->clikleft = 1;
					 break;
				 }
			break;

			case SDL_MOUSEBUTTONUP:
				 switch(sdlevent.button.button)
				 {
					 case SDL_BUTTON_RIGHT:
						  event->clikright = 2;
					 break;

					 case SDL_BUTTON_LEFT:
						  event->clikleft = 2;
					 break;
				 }
			break;

			case SDL_KEYDOWN:

				for(i=0;i<SDLK_LAST;i++)
				{
					if(sdlevent.key.keysym.sym == i)
					{
						event->key[i] = 1;
/*
						event->key[MNSK_KEYA] = i;
						event->key_ext[MNSK_KEYA] = i;
*/
					}

				}


			break;

			case SDL_KEYUP:

				for(i=0;i<SDLK_LAST;i++)
				{
					if(sdlevent.key.keysym.sym == i)
					{
						event->key[i] = 3;
					}

				}

				//event->key_ext[MNSK_KEYA] = -1;

			break;
/*
			case SDL_JOYBUTTONDOWN:

				for(i=0;i<20;i++)
				{
					if (sdlevent.jbutton.button == i)
					{
						event->touche[MNSJ + i] = 1;
						event->touche_ext[MNSJ + i] = 1;
					}
				}

			break;

			case SDL_JOYBUTTONUP:

				for(i=0;i<20;i++)
				{
					if (sdlevent.jbutton.button == i)
					{
						event->touche[MNSJ + i] = 2;
						event->touche_ext[MNSJ + i] = 0;
					}
				}

			break;

			case SDL_JOYAXISMOTION:


				if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value < -20000)
				{
					event->touche[MNSJ_LEFT] = 1;
					event->touche_ext[MNSJ_LEFT] = 1;
				}

				if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value >= -20000)
				{
					event->touche[MNSJ_LEFT] = 2;
					event->touche_ext[MNSJ_LEFT] = 0;
				}

				if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value > 20000)
				{
					event->touche[MNSJ_RIGHT] = 1;
					event->touche_ext[MNSJ_RIGHT] = 1;
				}

				if (sdlevent.jaxis.axis == 0 && sdlevent.jaxis.value <= 20000)
				{
					event->touche[MNSJ_RIGHT] = 2;
					event->touche_ext[MNSJ_RIGHT] = 0;
				}

				if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value < -20000)
				{
					event->touche[MNSJ_UP] = 1;
					event->touche_ext[MNSJ_UP] = 1;
				}

				if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value >= -20000)
				{
					event->touche[MNSJ_UP] = 2;
					event->touche_ext[MNSJ_UP] = 0;
				}

				if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value > 20000)
				{
					event->touche[MNSJ_DOWN] = 1;
					event->touche_ext[MNSJ_DOWN] = 1;
				}

				if (sdlevent.jaxis.axis == 1 && sdlevent.jaxis.value <= 20000)
				{
					event->touche[MNSJ_DOWN] = 2;
					event->touche_ext[MNSJ_DOWN] = 0;
				}


 */
			break;

		}



	}


	event->key[Button_Start] = event->key['h'];
	event->key[Button_Select] = event->key['j'];

	event->key[Button_A] = event->key['s'];
	event->key[Button_B] = event->key['d'];
	event->key[Button_X] = event->key['f'];
	event->key[Button_Y] = event->key['g'];

	event->key[Button_Up]	= event->key[SDLK_UP];
	event->key[Button_Down]  = event->key[SDLK_DOWN];
	event->key[Button_Right] = event->key[SDLK_RIGHT];
	event->key[Button_Left]  = event->key[SDLK_LEFT];

	event->key[Button_R1] = event->key['x'];
	event->key[Button_L1] = event->key['c'];
	event->key[Button_R2] = event->key['v'];
	event->key[Button_L2] = event->key['b'];

	event->key[Button_R3] = event->key['e'];
	event->key[Button_L3] = event->key['r'];
}

#endif

