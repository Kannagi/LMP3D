#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

void AURAE_Init()
{
	int flag[2];
	flag[0] = 0x00000;

	asm __volatile__(

	"mov.l		@%0,r0\n	"
	"lds		r0,FPSCR\n	" //GBR VBR SSR SPC SGR DBR

	//"sts		FPSCR,r0\n	" //GBR VBR SSR SPC SGR DBR
	//"mov.l		r0,@%0\n	"

	:: "r"(flag) : "memory");

	//printf("result :%x\n	",flag[0]);


	RW_REGISTER_U32(RENDER_PALETTE+DC_P2) = 0;
	DC_Init();
}

#endif

