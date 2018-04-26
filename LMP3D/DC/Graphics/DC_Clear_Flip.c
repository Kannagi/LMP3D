#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

void LMP3D_Clear()
{

}

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{
	DC_SwapBuffers();
}

#endif


