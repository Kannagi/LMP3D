

#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "LMP3D/LMP3D.h"


void PS2_LoadModules(void)
{
	static char *ModulesList[5] = {
   "rom0:SIO2MAN",
   "rom0:MCMAN",
   "rom0:MCSERV",
   "rom0:PADMAN",
   "rom0:LIBSD"
	};

	int ret = 0;
	int i;
	for ( i = 0; i < 5; i++ )
	{
		ret = SifLoadModule(ModulesList[i], 0, NULL);
		if ( ret < 0 )
			printf("Failed to load module: %s\n", ModulesList[i]);
	}
}


void LMP3D_Init()
{
	SifInitRpc(0);
	PS2_LoadModules();


	PS2_Init_Pad();
	//Init DMA
}

#endif
