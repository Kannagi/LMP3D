#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

static int AURAE_mode2D = 0;
void AURAE_Draw_Config(int flag)
{
	AURAE_mode2D = flag;
}

float DC_Zorder(float z,int flag)
{
	static float zinit = 10000.0f;

	if(flag == 0)
	{
		zinit = z;
	}else
	{
		zinit += 0.5f;
		return zinit;
	}
	return 0;
}

void AURAE_Draw_Sprite(AURAE_Texture *texture,Vector2i position,Recti *rectin,int flag)
{
	if(flag & AURAE_NODRAW) return;

	DC_Init_Poly(texture,TA_PARA_SPRITE);

	float z = DC_Zorder(0,1);

	float x = position.x;
	float y = position.y;

	Recti rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = texture->w;
	rect.h = texture->h;

	if(rectin != NULL)
		rect = *rectin;

	int irectw = 0;
	int irecth = 0;
	float zfrectw = 0;
	float zfrecth = 0;

	if(AURAE_mode2D & 1)
	{
		position.x = position.x<<1;
		position.y = position.y<<1;
		irectw = rect.w;
		irecth = rect.h;
		zfrectw = rect.w;
		zfrecth = rect.h;
	}

	if(position.x >= 640) return;
	if(position.y >= 480) return;
	if(position.x+rect.w+irectw <= 0) return;
	if(position.y+rect.h+irecth <= 0) return;

	float u,v;
	u32 *ufl2 = &v;
	u32 *ufl  = &u;

	void *currentBuffer = UNCACHED_P4(0);

	*((u32*)currentBuffer+0) = 0XF0000000;

	float frectw = rect.w;
	float frecth = rect.h;



	float flagw = 0,flagh = 0;
	if(flag & AURAE_FLIPH) flagw = frectw+zfrectw;
	if(flag & AURAE_FLIPV) flagh = frecth+zfrecth;

	*((float*)currentBuffer+1) = x+flagw;
	*((float*)currentBuffer+2) = (y+flagh);
	*((float*)currentBuffer+3) = z;

	*((float*)currentBuffer+4) = (x-flagw)+frectw+zfrectw;
	*((float*)currentBuffer+5) = (y+flagh);
	*((float*)currentBuffer+6) = z;

	*((float*)currentBuffer+7) = (x-flagw)+frectw+zfrectw;
	*((float*)currentBuffer+8) = (y-flagh)+frecth+zfrecth;
	*((float*)currentBuffer+9) = z;

	*((float*)currentBuffer+10) = x+flagw;
	*((float*)currentBuffer+11) = (y-flagh)+frecth+zfrecth;

	float tfpixelw = 1.0f/texture->w;
	float tfpixelh = 1.0f/texture->h;

	float rectx = tfpixelw*rect.x;
	float recty = tfpixelh*rect.y;
	float rectw = tfpixelw*frectw;
	float recth = tfpixelh*frecth;

	u = rectx;
	v = recty;
	*((u32*)currentBuffer+13) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

	u = rectx+rectw;
	v = recty;
	*((u32*)currentBuffer+14) = (*ufl2>>16) + (*ufl&0xFFFF0000);

	u = rectx+rectw;
	v = recty+recth;
	*((u32*)currentBuffer+15) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

	asm("pref @%0" : : "r"(currentBuffer));
	asm("pref @%0" : : "r"(currentBuffer+32));
}

void AURAE_Draw_Text(int ipx,int ipy,char *text)
{
	void *currentBuffer = UNCACHED_P4(0);

	float z = DC_Zorder(0,1);

	int i = 0;

	float y,x;
	float tx,ty;
	char c;

	float u,v;

	float tilef = 16.0f;
	if(AURAE_mode2D & 1)
	{
		tilef = 32.0f;
		ipx = ipx<<1;
		ipy = ipy<<1;
	}

	float px = ipx;
	float py = ipy;
	u32 *ufl2 = &v;
	u32 *ufl  = &u;

	x = px;
	y = py;



	*((u32*)currentBuffer+0) = 0XF0000000;
	*((float*)currentBuffer+3) = z;
	*((float*)currentBuffer+6) = z;
	*((float*)currentBuffer+9) = z;

	while(1)
	{
		c = text[i++];

		if(c == ' ')
		{
			x += tilef;
			continue;
		}

		if(c == '\n')
		{
			y += tilef;
			x = px;
			continue;
		}

		if(c == 0) break;

		tx = c&0x0F;
		ty = (c&0xF0)>>4;


		*((float*)currentBuffer+1) = x;
		*((float*)currentBuffer+2) = y;

		*((float*)currentBuffer+4) = x+tilef;
		*((float*)currentBuffer+5) = y;

		*((float*)currentBuffer+7) = x+tilef;
		*((float*)currentBuffer+8) = y+tilef;

		*((float*)currentBuffer+10) = x;
		*((float*)currentBuffer+11) = y+tilef;

		u = (1.0f/16.0f)*tx;
		v = (1.0f/16.0f)*ty;
		*((u32*)currentBuffer+13) = (*ufl2>>16) + (*ufl&0xFFFF0000);

		u = (1.0f/16.0f)*(tx+1);
		//v = (1.0f/16.0f)*ty;
		*((u32*)currentBuffer+14) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

		//u = (1.0f/16.0f)*(tx+1);
		v = (1.0f/16.0f)*(ty+1);
		*((u32*)currentBuffer+15) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

		x += tilef;

		asm("pref @%0" : : "r"(currentBuffer));
		asm("pref @%0" : : "r"(currentBuffer+32));
	}
}

void AURAE_Draw_Sprite_Array(AURAE_Sprite *sprite,int n)
{
	int i,flag;

	float u,v;
	u32 *ufl2 = &v;
	u32 *ufl  = &u;

	void *currentBuffer = UNCACHED_P4(0);

	float z = DC_Zorder(0,1);

	static void *oldtexture;
	oldtexture = NULL;

	for(i =0;i < n;i++)
	{
		flag = sprite[i].flag;
		if(flag & AURAE_NODRAW) continue;

		Recti rect = sprite[i].rect;
		Vector2i position = sprite[i].position;

		int irectw = 0;
		int irecth = 0;
		float zfrectw = 0;
		float zfrecth = 0;

		if(AURAE_mode2D & 1)
		{
			position.x = position.x<<1;
			position.y = position.y<<1;
			irectw = rect.w;
			irecth = rect.h;
			zfrectw = rect.w;
			zfrecth = rect.h;
		}

		if(position.x >= 640) continue;
		if(position.y >= 480) continue;
		if(position.x+rect.w+irectw <= 0) continue;
		if(position.y+rect.h+irecth <= 0) continue;

		AURAE_Texture *texture = sprite[i].texture;

		float frectw = rect.w;
		float frecth = rect.h;
		float x = position.x;
		float y = position.y;

		if(oldtexture != texture)
		{
			DC_Init_Poly(texture,TA_PARA_SPRITE);
			oldtexture = texture;
		}

		*((u32*)currentBuffer+0) = 0XF0000000;

		float flagw = 0,flagh = 0;
		if(flag & AURAE_FLIPH) flagw = frectw+zfrectw;
		if(flag & AURAE_FLIPV) flagh = frecth+zfrecth;

		*((float*)currentBuffer+1) = x+flagw;
		*((float*)currentBuffer+2) = (y+flagh);
		*((float*)currentBuffer+3) = z;

		*((float*)currentBuffer+4) = (x-flagw)+frectw+zfrectw;
		*((float*)currentBuffer+5) = (y+flagh);
		*((float*)currentBuffer+6) = z;

		*((float*)currentBuffer+7) = (x-flagw)+frectw+zfrectw;
		*((float*)currentBuffer+8) = (y-flagh)+frecth+zfrecth;
		*((float*)currentBuffer+9) = z;

		*((float*)currentBuffer+10) = x+flagw;
		*((float*)currentBuffer+11) = (y-flagh)+frecth+zfrecth;

		float tfpixelw = 1.0f/texture->w;
		float tfpixelh = 1.0f/texture->h;

		asm("pref @%0" : : "r"(currentBuffer));

		float rectx = tfpixelw*rect.x;
		float recty = tfpixelh*rect.y;
		float rectw = tfpixelw*frectw;
		float recth = tfpixelh*frecth;

		u = rectx;
		v = recty;
		*((u32*)currentBuffer+13) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

		u = rectx+rectw;
		//v = recty;
		*((u32*)currentBuffer+14) = (*ufl2>>16) + (*ufl&0xFFFF0000);

		//u = rectx+rectw;
		v = recty+recth;
		*((u32*)currentBuffer+15) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

		asm("pref @%0" : : "r"(currentBuffer+32));
	}

}

void __attribute__((optimize("-O1"), noinline)) AURAE_Draw_TileMap(AURAE_TileMap *tilemap)
{
	int i=0,flag;

	AURAE_Texture *texture = tilemap->texture;
	float z = DC_Zorder(0,1);

	DC_Init_Poly(texture,TA_PARA_SPRITE);

	int x,y;

	int h = tilemap->h;
	int w = tilemap->w;
	int windowW = 640;
	int windowH = 480;

	if(AURAE_mode2D & 1)
	{
		windowW = 320;
		windowH = 240;
	}

	if(tilemap->position.x < 0) tilemap->position.x = 0;
	if(tilemap->position.y < 0) tilemap->position.y = 0;

	if(tilemap->position.x > (w<<4)-windowW) tilemap->position.x = (w<<4)-windowW;
	if(tilemap->position.y > (h<<4)-windowH) tilemap->position.y = (h<<4)-windowH;


	int px = tilemap->position.x;
	int py = tilemap->position.y;
	i = (px>>4)+ ((py>>4)*w);

	int org = -(px&0xF);

	px = org;
	py = -(py&0xF);


	float tilef = 16.0f;
	int ntilex = 41;
	int ntiley = 31;

	if(AURAE_mode2D & 1)
	{
		tilef = 32.0f;
		px = px<<1;
		py = py<<1;
		ntilex = 21;
		ntiley = 16;
	}

	float fpx = px;
	float fpy = py;
	float forg = fpx;

	float u,v;
	u32 *ufl2 = &v;
	u32 *ufl  = &u;

	float tfpixelw = 1.0f/texture->w;
	float tfpixelh = 1.0f/texture->h;

	void *currentBuffer = UNCACHED_P4(0);
	int tile;

	float rectw = tfpixelw*16.0f;
	float recth = tfpixelh*16.0f;

	*((u32*)currentBuffer+0) = 0XF0000000;
	*((float*)currentBuffer+3) = z;
	*((float*)currentBuffer+6) = z;
	*((float*)currentBuffer+9) = z;

	for(y = 0;y < ntiley;y++)
	{
		for(x = 0;x < ntilex;x++)
		{
			tile = tilemap->tiles[i];
			flag = tile>>8;

			if(flag & AURAE_NODRAW)
			{
				i++;
				fpx += tilef;
				continue;
			}

			flag = flag&0x03;

			float frectx = (tile&0x0F)<<4;
			float frecty = (tile&0xF0);

			float flagw = 0,flagh = 0;
			if(flag & AURAE_FLIPH) flagw = tilef;
			if(flag & AURAE_FLIPV) flagh = tilef;

			*((float*)currentBuffer+1) = fpx+flagw;
			*((float*)currentBuffer+2) = (fpy+flagh);

			*((float*)currentBuffer+4) = (fpx-flagw)+tilef;
			*((float*)currentBuffer+5) = (fpy+flagh);

			*((float*)currentBuffer+7) = (fpx-flagw)+tilef;
			*((float*)currentBuffer+8) = (fpy-flagh)+tilef;

			*((float*)currentBuffer+10) = fpx+flagw;
			*((float*)currentBuffer+11) = (fpy-flagh)+tilef;

			float rectx = tfpixelw*frectx;
			float recty = tfpixelh*frecty;
			asm("pref @%0" : : "r"(currentBuffer));

			u = rectx;
			v = recty;
			*((u32*)currentBuffer+13) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

			u = rectx+rectw;
			//v = recty;
			*((u32*)currentBuffer+14) = (*ufl2>>16) + (*ufl&0xFFFF0000);

			//u = rectx+rectw;
			v = recty+recth;
			*((u32*)currentBuffer+15) =  (*ufl2>>16) + (*ufl&0xFFFF0000);

			fpx += tilef;
			i++;


			asm("pref @%0" : : "r"(currentBuffer+32));
		}
		fpy += tilef;
		fpx = forg;
		i += w-x;
	}

}


#endif

