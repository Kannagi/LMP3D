#include <stdio.h>
#include <stdlib.h>

#include "AURAE/AURAE.h"

void game(AURAE_Buffer *buffer);
void game2(AURAE_Buffer *buffer);
int	game4(void);
void mainsoft(AURAE_Buffer *buffer);
void main2();




#define _SHIFTL(v, s, w)	\
	((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#include <stdio.h>
#include <math.h>




int main(void)
{

	printf("Begin\n----------------\n");

	AURAE_Init();

	AURAE_Buffer buffer;

	buffer = AURAE_Window("Demo");

#ifdef WINDOWS95
	mainsoft(&buffer);
	return 0;
#endif

#ifdef DREAMCAST
	main2();
	return 0;
#endif

#ifdef PLAYSTATION1
	game2(&buffer);
	return 0;
#endif

#ifdef Wii
	game4();
	return 0;
#endif

#ifdef GC
	game4();
	return 0;
#endif
	game(&buffer);

	return 0;
}

