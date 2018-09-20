#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LMP3D/LMP3D.h"

#ifdef PLAYSTATION1

#include "LMP3D/PS1/PS1.h"

void LMP3D_File_Path(char *inpath,char *outpath)
{

	int i,n = strlen(inpath);

	for(i =0;i < n;i++)
	{
		if(inpath[i] >= 'a' && inpath[i] <= 'z')
			inpath[i] += 'A' -'a';
	}

	sprintf(outpath,"cdrom:%s;1",inpath);
}

#endif

