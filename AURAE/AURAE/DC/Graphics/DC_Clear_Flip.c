#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

void AURAE_Clear()
{
	DC_InitClip();


	return;

/*

	u32 *tpoly = UNCACHED_P4(0);


	tpoly[0] = TA_PARA_POLYGON | TA_GROUP_ENABLE | TA_GROUP_STRIPLEN1  | TA_PARA_OPOLY | TA_OBJ_OFFSET;
	tpoly[1] = TA_ISP_DEPTH_GREATER | TA_ISP_CULLING_POSITIVE;
	tpoly[2] = TA_TSP_FOG_DISABLE;
	tpoly[3] = 0;

	tpoly[4] = 0xFF000000;
	tpoly[5] = 0xFF000000;
	tpoly[6] = 0x00000000;
	tpoly[7] = 0xFFFFFFFF;

	//asm("pref @%0" : : "r" (tpoly));

	float u,v;
	u32 *ufl2 = &v;
	u32 *ufl  = &u;

	void *currentBuffer = UNCACHED_P4(0);

	*((u32*)currentBuffer+0) = 0XE0000000;
	*((float*)currentBuffer+1) = 0;
	*((float*)currentBuffer+2) = 0;
	*((float*)currentBuffer+3) = 200;

	asm("pref @%0" : : "r"(currentBuffer));


	*((u32*)currentBuffer+0) = 0XE0000000;
	*((float*)currentBuffer+1) = 250;
	*((float*)currentBuffer+2) = 200;
	*((float*)currentBuffer+3) = 200;

	asm("pref @%0" : : "r"(currentBuffer));


	*((u32*)currentBuffer+0) = 0XF0000000;
	*((float*)currentBuffer+1) = 640;
	*((float*)currentBuffer+2) = 480;
	*((float*)currentBuffer+3) = 200;

	asm("pref @%0" : : "r"(currentBuffer));
	*/

}

void AURAE_FlipBuffer(AURAE_Buffer *buffer)
{

	u32 *tpoly = UNCACHED_P4(0);

	tpoly[0] = TA_PARA_SPRITE |  TA_PARA_TPOLY;
	tpoly[1] = 0;
	tpoly[2] = 0;
	tpoly[3] = 0;

	tpoly[6] = 0x00000000;
	tpoly[7] = 0xFFFFFFFF;

	asm("pref @%0" : : "r" (tpoly));

	DC_Finish();
	DC_SwapBuffers();
}

#endif


