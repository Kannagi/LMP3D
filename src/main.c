#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __MINGW32__
#undef main
#endif

#include "LMP3D/LMP3D.h"

void game(LMP3D_Buffer *buffer);



int main(int argc, char** argv)
{
	LMP3D_Init();

	LMP3D_Buffer buffer;

    buffer = LMP3D_Window("Demo");

    srand(time(NULL));

    game(&buffer);

    return 0;
}
