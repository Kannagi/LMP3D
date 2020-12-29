#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>


#include "LMP3D/LMP3D.h"

#include "LMP3D/PS2/PS2.h"

static u64 dmaBuffer[16] __attribute__((aligned(16)));
static int buffer_sprite[16];
static u64 giftag[2];

#define PS2_VU_Func_Sprite	 		(0x30>>3)

void PS2_VU_Init2D()
{

	//-------------------
	void *currentBuffer;
	int i;

	u64 primv = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT,GS_TME_TEXTURE_ON, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_UV,
							GS_CTXT_0, GS_FIX_0);

	giftag[0] = GS_SET_GIFTAG(2, 1, 1, primv,0, 2);
	giftag[1] = 0x53;
	//5 : XYZ2
	//4 : XYZF2
	//3 : UV
	//2 : ST
	//1 : RGBAQ

	for(i = 0;i < 2;i++)
	{
		currentBuffer = (dmaBuffer);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)buffer_sprite , 4);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,3,0);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)giftag, 1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,1,6);

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,PS2_VU_Func_Sprite);

		*((u64*)currentBuffer)++ = DMA_END_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHA,0,0);
	}


	FlushCache(0);
}

static int LMP3D_mode2D = 0;
void LMP3D_Draw_Config(int flag)
{
	LMP3D_mode2D = flag;
}

//-------------------------------------------------------------

void LMP3D_Draw_Sprite(LMP3D_Texture *texture,Vector2i position,Recti *rectin,int flag)
{
	if(flag & LMP3D_NODRAW) return;

	Recti rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = texture->w;
	rect.h = texture->h;

	if(rectin != NULL)
		rect = *rectin;

	int w = 0;
	if(LMP3D_mode2D & 1)
	{
		position.x = position.x<<1;
		w = rect.w;
	}

	if(position.x >= 640) return;
	if(position.y >= 256) return;
	if(position.x+rect.w+w <= 0) return;
	if(position.y+rect.h <= 0) return;

	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[16] __attribute__((aligned(64)));

	LMP3D_Texture_Setup(texture);

	int posx = (2048-320+position.x)<<4;
	int posy = (2048-128+position.y)<<4;

	rect.x = rect.x<<4;
	rect.y = rect.y<<4;
	rect.w = rect.w<<4;
	rect.h = rect.h<<4;
	w = w<<4;

	int flagw = 0,flagh = 0;
	if(flag & LMP3D_FLIPH) flagw = rect.w;
	if(flag & LMP3D_FLIPV) flagh = rect.h;

	gif = UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(1,0,0,0,0,1);
	gif[i++] = GS_REG_AD;


	gif[i++] = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT,GS_TME_TEXTURE_ON,GS_FGE_FOGGING_OFF,
							GS_ABE_ALPHA_OFF,GS_AA1_ANTIALIASING_OFF, GS_FST_UV, GS_CTXT_0, GS_FIX_0);
	gif[i++] = GS_REG_PRIM;

	gif[i++] = GS_SET_GIFTAG(2,1,0,0,1,2);
	gif[i++] = 0x0053;

	gif[i++] = GS_SET_UV((rect.x+flagw),(rect.y+flagh));
	gif[i++] = GS_SET_XYZ(posx, posy, 4048<<4);

	gif[i++] = GS_SET_UV((rect.x+rect.w-flagw),(rect.y+rect.h-flagh));
	gif[i++] = GS_SET_XYZ((posx+rect.w+w),(posy+rect.h), 4048<<4);

/*
	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = 1;
	gif[i++] = 0X61;

*/
	//FlushCache(0);
	//asm __volatile__("sync.l");
	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);



	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_TADR) = 0;
	RW_REGISTER_U32(D2_QWC ) = i>>1;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);


}

void LMP3D_Draw_Text(int px,int py,char *text)
{
	int i=0;
	u32 *buffer_sp = UNCACHED_SEG(buffer_sprite);
	void *kickBuffer = dmaBuffer;

	char c;
	int tx,ty;
	int x = px;
	int y = py;

	int tilew = 16;

	if(LMP3D_mode2D & 1)
	{
		x = x<<1;
		px = px<<1;
		tilew = 32;
	}

	buffer_sp[2] = 0x100;
	buffer_sp[3] = 0x100;
	buffer_sp[6] = 0;
	buffer_sp[7] = 0;
	buffer_sp[8] = tilew;

	buffer_sp[9]  = (2048-320)<<4;
	buffer_sp[10] = (2048-128)<<4;

	while(1)
	{
		c = text[i++];

		if(c == ' ')
		{
			x += tilew;
			continue;
		}
		if(c == '\n')
		{
			y += 16;
			x = px;
			continue;
		}

		if(c == 0)
			break;

		tx = (c&0x0F)<<8;
		ty = (c&0xF0)<<4;

		while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

		buffer_sp[0] = tx;
		buffer_sp[1] = ty;

		buffer_sp[4] = (x)<<4;
		buffer_sp[5] = (y)<<4;

		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);


		x += tilew;
	}

	buffer_sp[9]  = 0;(2048-320)<<4;
	buffer_sp[10] = 0;(2048-128)<<4;


	while( (RW_REGISTER_U32(D1_CHCR)) & 0x100);

}


void LMP3D_Draw_Sprite_Array(LMP3D_Sprite *sprite,int n)
{
	int i=0,flag;
	int w = 0;

	u32 *buffer_sp = UNCACHED_SEG(buffer_sprite);
	void *kickBuffer = dmaBuffer;

	buffer_sp[9]  = (2048-320)<<4;
	buffer_sp[10] = (2048-128)<<4;

	for(i = 0;i < n;i++)
	{
		flag = sprite[i].flag;
		if(flag & LMP3D_NODRAW) continue;

		Recti rect = sprite[i].rect;
		Vector2i position = sprite[i].position;
		LMP3D_Texture *texture = sprite[i].texture;

		if(LMP3D_mode2D & 1)
		{
			position.x = position.x<<1;
			w = rect.w<<4;
		}

		if(position.x >= 640) continue;
		if(position.y >= 256) continue;
		if(position.x+rect.w+w <= 0) continue;
		if(position.y+rect.h <= 0) continue;

		LMP3D_Texture_Setup(texture);

		rect.x = rect.x<<4;
		rect.y = rect.y<<4;
		rect.w = rect.w<<4;
		rect.h = rect.h<<4;

		int posx = (position.x)<<4;
		int posy = (position.y)<<4;

		int flagw = 0,flagh = 0;
		if(flag & LMP3D_FLIPH) flagw = rect.w;
		if(flag & LMP3D_FLIPV) flagh = rect.h;


		while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

		buffer_sp[0] = rect.x;
		buffer_sp[1] = rect.y;
		buffer_sp[2] = rect.w;
		buffer_sp[3] = rect.h;

		buffer_sp[4] = posx;
		buffer_sp[5] = posy;
		buffer_sp[6] = flagw;
		buffer_sp[7] = flagh;

		buffer_sp[8] = w;

		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

	}


	while( (RW_REGISTER_U32(D1_CHCR)) & 0x100);

}

void LMP3D_Draw_TileMap(LMP3D_TileMap *tilemap)
{
	int i=0,flag;

	u32 *buffer_sp = UNCACHED_SEG(buffer_sprite);
	void *kickBuffer = dmaBuffer;

	LMP3D_Texture *texture = tilemap->texture;

	int x,y;

	int h = tilemap->h;
	int w = tilemap->w;
	int ntilex = 41;

	int windowW = 640;
	buffer_sp[9]  = 0;
	buffer_sp[10] = 0;

	buffer_sp[2] = 0x100;
	buffer_sp[3] = 0x100;

	if(LMP3D_mode2D & 1)
	{
		windowW = 320;
		ntilex = 21;
	}

	if(tilemap->position.x < 0) tilemap->position.x = 0;
	if(tilemap->position.y < 0) tilemap->position.y = 0;

	if(tilemap->position.x > (w<<4)-windowW) tilemap->position.x = (w<<4)-windowW;
	if(tilemap->position.y > (h<<4)-256) tilemap->position.y = (h<<4)-256;

	LMP3D_Texture_Setup(texture);

	int px = tilemap->position.x;
	int py = tilemap->position.y;
	i = (px>>4)+ ((py>>4)*w);

	int org = -(px&0xF);

	int tilew = 0;

	if(LMP3D_mode2D & 1)
	{
		org = org<<1;
		tilew = 0x100;
	}

	org = (2048-320+org)<<4;

	px = org;
	py = -(py&0xF);

	py = (2048-128+py)<<4;

	int tile;

	for(y = 0;y < 17;y++)
	{
		for(x = 0;x < ntilex;x++)
		{
			tile = tilemap->tiles[i];
			flag = tile>>8;

			if(flag & LMP3D_NODRAW)
			{
				i++;
				px += 0x100+tilew;
				continue;
			}

			flag = flag&0x03;
			int flagw = 0,flagh = 0;

			int rectx = (tile&0x0F)<<8;
			int recty = (tile&0xF0)<<4;

			if(flag & LMP3D_FLIPH) flagw = 0X100;
			if(flag & LMP3D_FLIPV) flagh = 0X100;


			while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

			buffer_sp[0] = rectx;
			buffer_sp[1] = recty;

			buffer_sp[4] = px;
			buffer_sp[5] = py;
			buffer_sp[6] = flagw;
			buffer_sp[7] = flagh;

			buffer_sp[8] = tilew;

			RW_REGISTER_U32(D1_MADR) = 0;
			RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
			RW_REGISTER_U32(D1_QWC ) = 0;
			RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);


			i++;
			px += 0x100+tilew;

		}
		py += 0x100;
		px = org;
		i += w-x;
	}

	while( (RW_REGISTER_U32(D1_CHCR)) & 0x100);
}

#endif

