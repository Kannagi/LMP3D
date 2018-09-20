#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"

void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{
	PS1_GTE_Perspective_Set();
}



void LMP3D_Camera_Ortho2D()
{

}
#endif


