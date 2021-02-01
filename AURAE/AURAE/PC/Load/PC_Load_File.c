#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AURAE/AURAE.h"

#ifdef PC


void AURAE_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}

#endif

