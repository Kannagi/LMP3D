#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

AURAE_Buffer AURAE_Window(char *name)
{
	AURAE_Buffer buffer;

	return buffer;
}


int AURAE_VBlank()
{
	DC_Zorder(10000.0f,0);
	int vcnt = 0;
	RW_REGISTER_U32(0xA05f6900) = 0x08;
	while (!(RW_REGISTER_U32(0xA05f6900) & 0x08)) vcnt++;

	return vcnt;
}

void AURAE_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}

#endif

