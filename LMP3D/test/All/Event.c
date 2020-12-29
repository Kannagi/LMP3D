//#include <stdlib.h>
//#include <stdio.h>


#include "LMP3D/LMP3D.h"

void LMP3D_Event_Init(LMP3D_Event *event)
{
	int i;
	for(i = 0;i < LMP3D_EVENT_MAX;i++)
    {
        event->key[i] = 0;
    }

    event->mousex = 0;
    event->mousey = 0;

    event->clikright = 0;
    event->clikleft = 0;

	event->exit = 0;
}
