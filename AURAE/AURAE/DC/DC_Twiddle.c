/* Texture twiddler/detwiddler for PowerVR
   (c)2000 Dan Potter

   Greets to PlanetWeb ;-)
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"


/* Size of the texture */
static int imgsize;

/* Linear I/O pointer */
static int ptr = 0;

/* Twiddled texture */
static unsigned char *twiddled = NULL;

/* Normal texture */
static unsigned char *detwiddled = NULL;


static unsigned short *twiddleds = NULL;

/* Normal texture */
static unsigned short *detwiddleds = NULL;

void twsubdivide_and_move(int x1, int y1, int size)
{
	if(size == 1) twiddled[ptr++] = detwiddled[(y1*imgsize)+x1];
	else
	{
		int ns = size>>1;
		twsubdivide_and_move(x1, y1, ns);
		twsubdivide_and_move(x1, y1+ns, ns);
		twsubdivide_and_move(x1+ns, y1, ns);
		twsubdivide_and_move(x1+ns, y1+ns, ns);
	}
}

void twsubdivide_and_move_s(int x1, int y1, int size)
{
	if(size == 1) twiddleds[ptr++] = detwiddleds[(y1*imgsize)+x1];
	else
	{
		int ns = size>>1;
		twsubdivide_and_move_s(x1, y1, ns);
		twsubdivide_and_move_s(x1, y1+ns, ns);
		twsubdivide_and_move_s(x1+ns, y1, ns);
		twsubdivide_and_move_s(x1+ns, y1+ns, ns);
	}
}


void DC_twiddle_encode(AURAE_Texture *texture)
{
	ptr = 0;
	imgsize = texture->w;

/*
	twiddled = texture->pixel;
	detwiddled = malloc(imgsize*imgsize*2);
*/
	detwiddleds =detwiddled = texture->pixel;
	twiddleds = twiddled = malloc((imgsize*imgsize)<<1);

	/* Encode mode */
	if(texture->format == AURAE_FORMAT_8BPP)
		twsubdivide_and_move(0, 0, imgsize);
	else
		twsubdivide_and_move_s(0, 0, imgsize);

	free(texture->pixel);

	texture->pixel = twiddled;
	texture->psm |= TA_TEXTURE_TWILLED;
}

#endif
