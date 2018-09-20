#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

#ifdef PC


void LMP3D_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}

#endif

