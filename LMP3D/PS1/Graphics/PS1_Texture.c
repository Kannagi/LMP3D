#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>
#include <psxgpu.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"


void gpuSendSync0(unsigned int v)
{
  while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_CMD_READY)); /* is this correct? */
  RW_REGISTER_U32(GP0) = v;
}

void PS1_Upload_VRAM(unsigned int *buffer,unsigned short w,unsigned short h,int destX,int destY)
{
	printf("dest : %d %d\n",destX,destY);
	unsigned int hwords_to_transfer = w*h;
	unsigned int words_to_transfer = hwords_to_transfer >> 1;
	unsigned int blocks_to_transfer = words_to_transfer >> 4; /* full blocks */

	if(blocks_to_transfer > 0xFFFF)
	{
		printf("ERR: too many blocks to transfer: %#x\n", blocks_to_transfer);
		return;
	}

	RW_REGISTER_U32(GP1) = (0x04<<24)| 2; /* dma: on, cpu to gp0 */

	gpuSendSync0(GP0_COPY_CPU_TO_VRAM<< 24);
	RW_REGISTER_U32(GP0) = (destX | (destY << 16));
	RW_REGISTER_U32(GP0) = (w | (h << 16));

	RW_REGISTER_U32(DICR) &= ~(1 << (16+2)); /* disable dma interrupts for DMA2 */
	RW_REGISTER_U32(DPCR) |= 0x800; /* enable DMA2 */

	printf("VRAM : %d   %d,%d\n",blocks_to_transfer,destX,destY);
	if(blocks_to_transfer)
	{
		RW_REGISTER_U32(D2_MADR) = (unsigned int)buffer;
		RW_REGISTER_U32(D2_BCR)  = 0x10 | (blocks_to_transfer << 16);
		RW_REGISTER_U32(D2_CHCR) = D_CHCR_FROM_RAM | D_CHCR_FORWARD | D_CHCR_SYNC_DEV | D_CHCR_START;
		while( RW_REGISTER_U32(D2_CHCR) & D_CHCR_BUSY);
	}

	/* transfer remaining words in one block */
	words_to_transfer &= 0xF;
	if(words_to_transfer)
	{
		RW_REGISTER_U32(D2_MADR) = (unsigned int)buffer + (blocks_to_transfer * 0x10)*4;
		RW_REGISTER_U32(D2_BCR)  = words_to_transfer | (1 << 16);
		RW_REGISTER_U32(D2_CHCR) = D_CHCR_FROM_RAM | D_CHCR_FORWARD | D_CHCR_SYNC_DEV | D_CHCR_START;
		while( RW_REGISTER_U32(D2_CHCR) & D_CHCR_BUSY); /* wait while transfer in progress */
	}

	/* transfer remaining half word if necessary */
	hwords_to_transfer &= 1;
	if(hwords_to_transfer)
	{
		while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_DMA_READY));
		RW_REGISTER_U32(GP0) = ((unsigned int)*(unsigned short*)(buffer + (blocks_to_transfer * 0x10)*4 + words_to_transfer));
	}
}

void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{
	unsigned int *buffer = texture->pixel;
	int destX = (texture->address&0x0F)<<6, destY = (texture->address&0x70)<<2;
	if(texture->address&0x80)
	{
		return;
	}
	unsigned short w = texture->w;
	unsigned short h = texture->h;

	if(texture->psm == PS1_PSM8) w = w>>1;
	if(texture->psm == PS1_PSM4) w = w>>2;

	PS1_Upload_VRAM(buffer,w,h,destX,destY);

}

void LMP3D_Texture_Upload_VRAM_Pal(LMP3D_Texture *texture)
{
	unsigned int *buffer = texture->palette;
	int destX=0, destY=0;
	if(texture->addresspal&0x80)
	{
		destX = (texture->addresspal&0x3C)>>2;
		destY = 240 + destX;
		destX = (texture->addresspal&0x03)<<8;
	}else
		return;

	PS1_Upload_VRAM(buffer,256,1,destX,destY);

}
//malloc VRAM + LMP3D_Texture_Upload_VRAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{
	if(texture == NULL) return;

	texture->address = PS1_vram_allocate(texture->w,texture->h, texture->psm);
	LMP3D_Texture_Upload_VRAM(texture);

	if(texture->palette == NULL) return;

	texture->addresspal = PS1_vram_allocate(256,1, 0);
	LMP3D_Texture_Upload_VRAM_Pal(texture);

}

//Select Texture
void LMP3D_Texture_Setup(LMP3D_Texture *texture)
{

}

//Delete Texture
void LMP3D_Texture_Delete(LMP3D_Texture *texture)
{

}

#endif


