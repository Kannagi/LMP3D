#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"

void LMP3D_Draw_Config(int flag)
{

}

void LMP3D_Draw_Sprite(LMP3D_Texture *texture,Vector2i position,Recti *rect,int flag)
{
	unsigned int linked_list[0x10];

	int i = 1;

	if(position.x > 320) return;
	if(position.y > 240) return;

	int tx = (texture->address&0x3F);
	int ty = 0;
	if(texture->address&0x40) ty = 1;

	int destY = 240 + (texture->addresspal&0x0F);
	int destX = (texture->addresspal&0x30)<<7;
	int dest = destX+(destY<<6);

	flag = flag&0x03;

	int flagw = 0,flagh = 0;


	linked_list[i++] = GP0_TEXPAGE(tx,ty,0,texture->psm,0,1,0,flag,0);
	linked_list[i++] = GP0_CMD_COLOR(GP0_RECTANGLE_TOR,0,0,0);
	linked_list[i++] = GP0_VERTEX(position.x&0xFFFF,position.y);

	if(rect == NULL)
	{
		if(position.x+texture->w <= 0) return;
		if(position.y+texture->h <= 0) return;

		if(flag & LMP3D_FLIPH) flagw = texture->w-1;
		if(flag & LMP3D_FLIPV) flagh = texture->h-1;
		linked_list[i++] = GP0_TEXCOORD(flagw,flagh,dest);
		linked_list[i++] = GP0_VERTEX(texture->w,texture->h);
	}else
	{
		if(position.x+rect->w <= 0) return;
		if(position.y+rect->h <= 0) return;

		if(flag & LMP3D_FLIPH) flagw = rect->w-1;
		if(flag & LMP3D_FLIPV) flagh = rect->h-1;
		linked_list[i++] = GP0_TEXCOORD((rect->x+flagw),(rect->y+flagh),dest);
		linked_list[i++] = GP0_VERTEX(rect->w,rect->h);
	}

	linked_list[0] = (i-1)<<24;
	linked_list[i] = 0x00ffffff;

	gpu_ctrl(4, 2); // DMA CPU->GPU mode
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); /* Wait for the GPU to finish
						 * drawing primitives. */
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); /* Wait for the GPU to be free */


	RW_REGISTER_U32(D2_MADR) = (unsigned int)linked_list;
	RW_REGISTER_U32(D2_BCR) = 0;
	RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);
	while(GsIsDrawing());
}

void LMP3D_Draw_Text(int px,int py,char *text)
{
	unsigned int linked_list[0x100];

	int i = 1,l = 0;
	char c;
	int x,y;
	int tx,ty;
	x = px;
	y = py;



	int dest = LMP3D_TexturePal_Setup_Get();

	int text_all = 1;
	linked_list[i++] = LMP3D_Texture_Setup_Get();
	while(text_all == 1)
	{
		while(1)
		{
			c = text[l++];
			if(c == ' ')
			{
				x += 16;
				continue;
			}
			if(c == '\n')
			{
				y += 16;
				x = px;
				continue;
			}

			if(c == 0)
			{
				text_all = 0;
				break;
			}

			tx = (c&0x0F)<<4;
			ty = (c&0xF0);

			linked_list[i++] = GP0_CMD_COLOR(GP0_RECTANGLE_TOR16,0,0,0);
			linked_list[i++] = GP0_VERTEX(x,y);
			linked_list[i++] = GP0_TEXCOORD(tx,ty,dest);

			if(i >= 0xFF-2) break;

			x += 16;
		}

		linked_list[0] = (i-1)<<24;
		linked_list[i] = 0x00ffffff;

		gpu_ctrl(4, 2); // DMA CPU->GPU mode
		while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); /* Wait for the GPU to finish drawing primitives. */
		while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); /* Wait for the GPU to be free */

		RW_REGISTER_U32(D2_MADR) = (unsigned int)linked_list;
		RW_REGISTER_U32(D2_BCR) = 0;
		RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);

		i = 1;
		while(GsIsDrawing());
	}

}


void LMP3D_Draw_Sprite_Array(LMP3D_Sprite *sprite,int n)
{
	unsigned int linked_list[0x100];
	int i = 1,l = 0,sprite_all = 1,flag;

	while(sprite_all == 1)
	{
		for(;l < n;l++)
		{
			flag = sprite[l].flag;
			if(flag & LMP3D_NODRAW) continue;

			Recti rect = sprite[l].rect;
			Vector2i position = sprite[l].position;
			LMP3D_Texture *texture = sprite[l].texture;

			if(position.x >= 320) continue;
			if(position.y >= 240) continue;
			if(position.x+rect.w <= 0) continue;
			if(position.y+rect.h <= 0) continue;

			flag = flag&0x03;
			int tx = (texture->address&0x3F);
			int ty = 0;
			if(texture->address&0x40) ty = 1;
			int destY = 240 + (texture->addresspal&0x0F);
			int destX = (texture->addresspal&0x30)<<7;

			int flagw = 0,flagh = 0;
			if(flag & LMP3D_FLIPH) flagw = rect.w-1;
			if(flag & LMP3D_FLIPV) flagh = rect.h-1;

			linked_list[i++] = GP0_TEXPAGE(tx,ty,0,texture->psm,0,1,0,flag,0);
			linked_list[i++] = GP0_CMD_COLOR(GP0_RECTANGLE_TOB,sprite[l].color,0,0);
			linked_list[i++] = GP0_VERTEX(position.x&0xFFFF,position.y);
			linked_list[i++] = GP0_TEXCOORD((rect.x+flagw),(rect.y+flagh),(destX+(destY<<6)));
			linked_list[i++] = GP0_VERTEX(rect.w,rect.h);

			if(i >= 0xFF-4)
				break;
		}

		if(l >= n)
			sprite_all = 0;

		linked_list[0] = (i-1)<<24;
		linked_list[i] = 0x00ffffff;

		gpu_ctrl(4, 2); // DMA CPU->GPU mode
		while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); /* Wait for the GPU to finish drawing primitives. */
		while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); /* Wait for the GPU to be free */

		RW_REGISTER_U32(D2_MADR) = (unsigned int)linked_list;
		RW_REGISTER_U32(D2_BCR) = 0;
		RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);
		while(GsIsDrawing());
	}
}

void LMP3D_Draw_TileMap(LMP3D_TileMap *tilemap)
{
	unsigned int linked_list[0x100];
	int i,l = 0,flag;

	LMP3D_Texture *texture = tilemap->texture;
	int tx = (texture->address&0x3F);
	int ty = 0;
	if(texture->address&0x40) ty = 1;

	int destY = 240 + (texture->addresspal&0x0F);
	int destX = (texture->addresspal&0x30)<<7;
	int dest = destX+(destY<<6);

	int gpo_texpage = GP0_TEXPAGE(tx,ty,0,texture->psm,0,1,0,0,0);
	Recti rect;
	int h = tilemap->h;
	int w = tilemap->w;
	int tile;
	int x,y,z;

	if(tilemap->position.x < 0) tilemap->position.x = 0;
	if(tilemap->position.y < 0) tilemap->position.y = 0;

	if(tilemap->position.x > (w<<4)-320) tilemap->position.x = (w<<4)-320;
	if(tilemap->position.y > (h<<4)-240) tilemap->position.y = (h<<4)-240;

	int px = tilemap->position.x;
	int py = tilemap->position.y;

	l += (px>>4)+ ((py>>4)*w);
	int org = -(px&0xF);
	px = org;
	py = -(py&0xF);


	for(x = 2;x < 0x100;x+=4)
		linked_list[x] = GP0_CMD_COLOR(GP0_RECTANGLE_TOR16,0,0,0);

	for(z = 0;z < 6;z++)
	{
		i = 1;
		int county = 3;
		if(z == 5) county = 1;
		for(y = 0;y < county;y++)
		{
			for(x = 0;x < 21;x++)
			{
				tile = tilemap->tiles[l];
				flag = tile>>8;

				if(flag & LMP3D_NODRAW)
				{
					l++;
					px += 16;
					continue;
				}

				flag = flag&0x03;

				rect.x = (tile&0x0F)<<4;
				rect.y = (tile&0xF0);

				int flagw = 0,flagh = 0;
				if(flag & LMP3D_FLIPH) flagw = 15;
				if(flag & LMP3D_FLIPV) flagh = 15;

				linked_list[i+0] = gpo_texpage + (flag<<12);
				//linked_list[i+1] = GP0_CMD_COLOR(GP0_RECTANGLE_TOR16,0,0,0);
				linked_list[i+2] = GP0_VERTEX(px&0xFFFF,py);
				linked_list[i+3] = GP0_TEXCOORD((rect.x+flagw),(rect.y+flagh),dest);
				i+=4;


				px += 16;
				l++;
			}
			py += 16;
			px = org;
			l += w-x;
		}

		if(i > 1)
		{
			linked_list[0] = (i-1)<<24;
			linked_list[i] = 0x00ffffff;

			//gpu_ctrl(4, 2); // DMA CPU->GPU mode
			while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); // Wait for the GPU to finish drawing primitives.
			while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); // Wait for the GPU to be free

			RW_REGISTER_U32(D2_MADR) = (unsigned int)linked_list;
			RW_REGISTER_U32(D2_BCR) = 0;
			RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);
			while(GsIsDrawing());

		}
	}

}

#endif
