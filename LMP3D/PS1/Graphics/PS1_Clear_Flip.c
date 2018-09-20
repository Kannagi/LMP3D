#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"

void LMP3D_Clear()
{
	unsigned int list[10];

	list[0] = 3<<24;
	list[1] = GP0_CMD_COLOR(GP0_RECTANGLE_MO,128,128,128);
	list[2] = GP0_VERTEX(0,0);
	list[3] = GP0_VERTEX(320,240);
	list[4] = 0x00ffffff;

	gpu_ctrl(4, 2); // DMA CPU->GPU mode
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); /* Wait for the GPU to finish
						 * drawing primitives. */
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); /* Wait for the GPU to be free */


	RW_REGISTER_U32(D2_MADR) = (unsigned int)list;
	RW_REGISTER_U32(D2_BCR) = 0;
	RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);


	while(!(RW_REGISTER_U32(GP1) & (1<<0x1a)));

}

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{
	PS1_BufferDraw();
	if(buffer->switchBuffer == 0)
	{
		PS1_GsSetDispEnv(0,0);
		PS1_GsSetDrawEnv(0,0,buffer->width ,buffer->height);

	}else
	{
		PS1_GsSetDispEnv(320,0);
		PS1_GsSetDrawEnv(320,0,buffer->width ,buffer->height);
	}
	buffer->switchBuffer = !buffer->switchBuffer;
}

#endif

