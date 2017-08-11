#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"


/*
#ifdef PC
		LMP3D_Texture_Delete_GL(id);
	#endif

	#ifdef PLAYSTATION2

	#endif

	#ifdef DREAMCAST

	#endif

	#ifdef PLAYSTATIONPORTABLE

	#endif


*/
void LMP3D_Texture_Free(LMP3D_Texture *image)
{
    if(image->pixel != NULL) free(image->pixel);
    if(image->palette != NULL) free(image->palette);
    free(image);

    image->pixel = NULL;
    image->palette = NULL;
    image = NULL;
}
