#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>


#include "LMP3D/LMP3D.h"

//Event update
void LMP3D_Event_Update(LMP3D_Event *event)
{
	unsigned short padbuf = 0;
	static unsigned short old_pad = 0;
	static unsigned short new_pad;
	static unsigned short end_pad;
	int i;

	for(i = 0;i < Button_Number;i++)
	{
		event->key[i] = 0;
	}

	PSX_ReadPad(&padbuf, NULL);
	//printf("%x\n",padbuf);

	new_pad = padbuf & ~old_pad;
	end_pad = (padbuf^old_pad)^new_pad;
	old_pad = padbuf;


	//DOWNW
	if(padbuf & PAD_SQUARE)
		event->key[Button_Square] = 2;

	if(padbuf & PAD_TRIANGLE)
		event->key[Button_Triangle] = 2;

	if(padbuf & PAD_CIRCLE)
		event->key[Button_Circle] = 2;

	if(padbuf & PAD_CROSS)
		event->key[Button_Cross] = 2;

	if(padbuf & PAD_LEFT)
		event->key[Button_Left] = 2;

	if(padbuf & PAD_RIGHT)
		event->key[Button_Right] = 2;

	if(padbuf & PAD_UP)
		event->key[Button_Up] = 2;

	if(padbuf & PAD_DOWN)
		event->key[Button_Down] = 2;

	if(padbuf & PAD_R1)
		event->key[Button_R1] = 2;

	if(padbuf & PAD_R2)
		event->key[Button_R2] = 2;

	if(padbuf & PAD_L1)
		event->key[Button_L1] = 2;

	if(padbuf & PAD_L2)
		event->key[Button_L2] = 2;

	if(padbuf & PAD_SELECT)
		event->key[Button_Select] = 2;

	if(padbuf & PAD_START)
		event->key[Button_Start] = 2;

	//DOWN
	if(new_pad & PAD_SQUARE)
		event->key[Button_Square] = 1;

	if(new_pad & PAD_TRIANGLE)
		event->key[Button_Triangle] = 1;

	if(new_pad & PAD_CIRCLE)
		event->key[Button_Circle] = 1;

	if(new_pad & PAD_CROSS)
		event->key[Button_Cross] = 1;

	if(new_pad & PAD_LEFT)
		event->key[Button_Left] = 1;

	if(new_pad & PAD_RIGHT)
		event->key[Button_Right] = 1;

	if(new_pad & PAD_UP)
		event->key[Button_Up] = 1;

	if(new_pad & PAD_DOWN)
		event->key[Button_Down] = 1;

	if(new_pad & PAD_R1)
		event->key[Button_R1] = 1;

	if(new_pad & PAD_R2)
		event->key[Button_R2] = 1;

	if(new_pad & PAD_L1)
		event->key[Button_L1] = 1;

	if(new_pad & PAD_L2)
		event->key[Button_L2] = 1;

	if(new_pad & PAD_SELECT)
		event->key[Button_Select] = 1;

	if(new_pad & PAD_START)
		event->key[Button_Start] = 1;

	//UP
	if(end_pad & PAD_SQUARE)
		event->key[Button_Square] = 3;

	if(end_pad & PAD_TRIANGLE)
		event->key[Button_Triangle] = 3;

	if(end_pad & PAD_CIRCLE)
		event->key[Button_Circle] = 3;

	if(end_pad & PAD_CROSS)
		event->key[Button_Cross] = 3;

	if(end_pad & PAD_LEFT)
		event->key[Button_Left] = 3;

	if(end_pad & PAD_RIGHT)
		event->key[Button_Right] = 3;

	if(end_pad & PAD_UP)
		event->key[Button_Up] = 3;

	if(end_pad & PAD_DOWN)
		event->key[Button_Down] = 3;

	if(end_pad & PAD_R1)
		event->key[Button_R1] = 3;

	if(end_pad & PAD_R2)
		event->key[Button_R2] = 3;

	if(end_pad & PAD_L1)
		event->key[Button_L1] = 3;

	if(end_pad & PAD_L2)
		event->key[Button_L2] = 3;

	if(end_pad & PAD_SELECT)
		event->key[Button_Select] = 3;

	if(end_pad & PAD_START)
		event->key[Button_Start] = 3;
}

#endif

