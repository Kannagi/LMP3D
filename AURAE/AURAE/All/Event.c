//#include <stdlib.h>
//#include <stdio.h>

#include "AURAE/AURAE.h"

void AURAE_Event_Init(AURAE_Event *event)
{
	int i;
	for(i = 0;i < AURAE_EVENT_MAX;i++)
	{
		event->key[i] = 0;
	}

	event->mousex = 0;
	event->mousey = 0;

	event->clikright = 0;
	event->clikleft = 0;

	event->exit = 0;
}


