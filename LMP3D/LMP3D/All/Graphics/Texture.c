#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"



void LMP3D_Texture_Free_RAM(LMP3D_Texture *image)
{
    if(image->pixel != NULL) free(image->pixel);
    if(image->palette != NULL) free(image->palette);
    if(image != NULL) free(image);

    image->pixel = NULL;
    image->palette = NULL;
    image = NULL;
}

void LMP3D_Texture_Free_Pixel(LMP3D_Texture *image)
{
    if(image->pixel != NULL) free(image->pixel);
    if(image->palette != NULL) free(image->palette);

    image->pixel = NULL;
    image->palette = NULL;
}

