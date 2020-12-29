

#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2

#include <kernel.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/PS2/PS2.h"

void GsDmaInit(void)
{
	/*Reset/init dma(gif channel only)*/
	__asm__(
	"li	$2,0x1000A000	\n"
	"nop				\n"
	"sw	$0,0x80($2)		\n"
	"sw	$0,0($2)		\n"
	"sw	$0,0x30($2)		\n"
	"sw	$0,0x10($2)		\n"
	"sw	$0,0x50($2)		\n"
	"sw	$0,0x40($2)		\n"
	"li	$2,0xFF1F		\n"//	0xFF1F
	"sw	$2,0x1000E010	\n"
	"lw	$2,0x1000E010	\n"
	"li	$3,0xFF1F		\n"//0xFF1F
	"and	$2,$3		\n"
	"sw	$2,0x1000E010	\n"
	"sync.p				\n"
	"sw	$0,0x1000E000	\n"
	"sw	$0,0x1000E020	\n"
	"sw	$0,0x1000E030	\n"
	"sw	$0,0x1000E050	\n"
	"sw	$0,0x1000E040	\n"
	"li	$3,1			\n"
	"lw	$2,0x1000E000	\n"
	"ori	$3,$2,1		\n"
	"nop				\n"
	"sw	$3,0x1000E000	\n"
	"nop				\n"
	);
}

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
	GsDmaInit();

	PS2_LoadModules();


	PS2_Init_Pad();
	//Init DMA
}

void LMP3D_Init2()
{

	SifInitRpc(0);

	GsDmaInit();

	/*
	SifInitRpc(0);
	while(!SifIopReset("", 0)){};
	while(!SifIopSync()){};
	SifInitRpc(0);
	*/
	fioInit();

	PS2_LoadModules();
/*
	FILE *file;
	if((file = fopen("FILE.EXT", "rb")) != NULL)
	{

	}

	fclose(file);
*/

	PS2_Init_Pad();
	//Init DMA
}

#endif
