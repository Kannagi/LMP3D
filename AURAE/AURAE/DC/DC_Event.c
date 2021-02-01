
#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"


#define PAD_R1 0x10000
#define PAD_L1 0x20000

void AURAE_Event_Update(AURAE_Event *event)
{
	static int padbuf;
	static int old_pad = 0;
	static int new_pad;
	static int end_pad;

	int i;

	for(i = 0;i < Button_Number;i++)
	{
		event->key[i] = 0;
	}

	maple_device_t *cont;
	cont_state_t *state;

	cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	state = (cont_state_t *)maple_dev_status(cont);

	int padr = state->rtrig;
	int padl = state->ltrig;

	if(padr != 0) padr = 0x10000;
	if(padl != 0) padl = 0x20000;

	padbuf = state->buttons | padr | padl;

	new_pad = padbuf & ~old_pad;
	end_pad = (padbuf^old_pad)^new_pad;
	old_pad = padbuf;

	//DOWNW
	if(padbuf & CONT_X)
		event->key[Button_X] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_Y)
		event->key[Button_Y] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_B)
		event->key[Button_B] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_A)
		event->key[Button_A] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_DPAD_LEFT)
		event->key[Button_Left] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_DPAD_RIGHT)
		event->key[Button_Right] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_DPAD_UP)
		event->key[Button_Up] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_DPAD_DOWN)
		event->key[Button_Down] = AURAE_KEY_DOWNW;

	if(padbuf & PAD_R1)
		event->key[Button_R1] = AURAE_KEY_DOWNW;

	if(padbuf & PAD_L1)
		event->key[Button_L1] = AURAE_KEY_DOWNW;

	if(padbuf & CONT_START)
		event->key[Button_Start] = AURAE_KEY_DOWNW;

	//DOWN
	if(new_pad & CONT_X)
		event->key[Button_X] = AURAE_KEY_DOWN;

	if(new_pad & CONT_Y)
		event->key[Button_Y] = AURAE_KEY_DOWN;

	if(new_pad & CONT_B)
		event->key[Button_B] = AURAE_KEY_DOWN;

	if(new_pad & CONT_A)
		event->key[Button_A] = AURAE_KEY_DOWN;

	if(new_pad & CONT_DPAD_LEFT)
		event->key[Button_Left] = AURAE_KEY_DOWN;

	if(new_pad & CONT_DPAD_RIGHT)
		event->key[Button_Right] = AURAE_KEY_DOWN;

	if(new_pad & CONT_DPAD_UP)
		event->key[Button_Up] = AURAE_KEY_DOWN;

	if(new_pad & CONT_DPAD_DOWN)
		event->key[Button_Down] = AURAE_KEY_DOWN;

	if(new_pad & PAD_R1)
		event->key[Button_R1] = AURAE_KEY_DOWN;

	if(new_pad & PAD_L1)
		event->key[Button_L1] = AURAE_KEY_DOWN;

	if(new_pad & CONT_START)
		event->key[Button_Start] = AURAE_KEY_DOWN;

	//UP
	if(end_pad & CONT_X)
		event->key[Button_X] = AURAE_KEY_UP;

	if(end_pad & CONT_Y)
		event->key[Button_Y] = AURAE_KEY_UP;

	if(end_pad & CONT_B)
		event->key[Button_B] = AURAE_KEY_UP;

	if(end_pad & CONT_A)
		event->key[Button_A] = AURAE_KEY_UP;

	if(end_pad & CONT_DPAD_LEFT)
		event->key[Button_Left] = AURAE_KEY_UP;

	if(end_pad & CONT_DPAD_RIGHT)
		event->key[Button_Right] = AURAE_KEY_UP;

	if(end_pad & CONT_DPAD_UP)
		event->key[Button_Up] = AURAE_KEY_UP;

	if(end_pad & CONT_DPAD_DOWN)
		event->key[Button_Down] = AURAE_KEY_UP;

	if(end_pad & PAD_R1)
		event->key[Button_R1] = AURAE_KEY_UP;

	if(end_pad & PAD_L1)
		event->key[Button_L1] = AURAE_KEY_UP;

	if(end_pad & CONT_START)
		event->key[Button_Start] = AURAE_KEY_UP;
}

#endif
