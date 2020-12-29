#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

LMP3D_Buffer LMP3D_Window(char *name)
{
	LMP3D_Buffer buffer;

	return buffer;
}


int LMP3D_VBlank()
{
	DC_Zorder(10000.0f,0);
	int vcnt = 0;
	RW_REGISTER_U32(0xA05f6900) = 0x08;
	while (!(RW_REGISTER_U32(0xA05f6900) & 0x08)) vcnt++;

	return vcnt;
}

void LMP3D_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}

#endif

