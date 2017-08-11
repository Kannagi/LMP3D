#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2
#include <kernel.h>
#include "LMP3D/Window.h"
#include "LMP3D/Platform/PS2_Register.h"


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
int _port,_slot;
void LMP3D_Init()
{
	SifInitRpc(0);
	PS2_LoadModules();

	GsDmaInit();


	int ret;
	static char padBuf[256] __attribute__((aligned(64)));
	padInit(0);

    _port = 0; // 0 -> Connector 1, 1 -> Connector 2
    _slot = 0; // Always zero if not using multitap

    printf("PortMax: %d\n", padGetPortMax());
    printf("SlotMax: %d\n", padGetSlotMax(_port));

	if((ret = padPortOpen(_port, _slot, padBuf)) == 0) {
        printf("padOpenPort failed: %d\n", ret);
        SleepThread();
    }

    if(!initializePad(_port, _slot)) {
        printf("pad initalization failed!\n");
        SleepThread();
    }

	//Init PAD
	//Init DMA
}

LMP3D_Buffer LMP3D_Init_Graphic(int width, int height);

LMP3D_Buffer LMP3D_Window(char *name)
{
	return LMP3D_Init_Graphic(0,0);
}

void LMP3D_Fps(int fps)
{

}

int LMP3D_VBlank()
{
	int VBlankCnt = 0;

	//VBlank interrupt enable
	GS_SET_CSR(0,0,0,1,0,0,0,0,0,0,0,0);

	//wait VBlank

	while( !( ( RW_REGISTER_U32(GS_CSR) ) & 0x08) ) VBlankCnt++;

	return VBlankCnt;
}


#endif
