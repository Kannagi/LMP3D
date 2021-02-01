#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>

#include "AURAE/AURAE.h"

static int AURAE_mode2D = 0;
void AURAE_Draw_Config(int flag)
{
	AURAE_mode2D = flag;
}

void AURAE_Draw_Sprite(AURAE_Texture *texture,Vector2i position,Recti *rectin,int flag)
{
	const float fpixelw = 1.0f/320.0f;
	const float fpixelh = 1.0f/240.0f;

	float va[8];
	float vt[8];

	if(flag & AURAE_NODRAW) return;

	Recti rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = texture->w;
	rect.h = texture->h;

	if(rectin != NULL)
		rect = *rectin;

	if(AURAE_mode2D & 1)
	{
		position.x = position.x<<1;
		position.y = position.y<<1;

		rect.x = rect.x<<1;
		rect.y = rect.y<<1;
		rect.w = rect.w<<1;
		rect.h = rect.h<<1;
	}

	if(position.x >= 640) return;
	if(position.y >= 480) return;
	if(position.x+rect.w <= 0) return;
	if(position.y+rect.h <= 0) return;

	glBindTexture(GL_TEXTURE_2D,texture->address);

	float px = -1.0f+(fpixelw*position.x);
	float py = +1.0f-(fpixelh*position.y);

	float w = rect.w;
	float h = rect.h;

	float tfpixelw = 1.0f/texture->w;
	float tfpixelh = 1.0f/texture->h;

	if(AURAE_mode2D & 1)
	{
		tfpixelw = tfpixelw*0.5;
		tfpixelh = tfpixelh*0.5;
	}

	float rectx = tfpixelw*rect.x;
	float recty = tfpixelh*rect.y;

	vt[0] = rectx;
	vt[1] = recty;

	vt[2] = rectx;
	vt[3] = recty+(tfpixelh*h);

	vt[4] = rectx+(tfpixelw*w);
	vt[5] = recty+(tfpixelh*h);

	vt[6] = rectx+(tfpixelw*w);
	vt[7] = recty;

	w = fpixelw*w;
	h = fpixelh*h;

	float flagw = 0,flagh = 0;
	if(flag & AURAE_FLIPH) flagw = w;
	if(flag & AURAE_FLIPV) flagh = h;

	va[0] = px+flagw;
	va[1] = py-flagh;

	va[2] = px+flagw;
	va[3] = py-h+flagh;

	va[4] = px+w-flagw;
	va[5] = py-h+flagh;

	va[6] = px+w-flagw;
	va[7] = py-flagh;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2,GL_FLOAT,0,va);
	glTexCoordPointer(2,GL_FLOAT,0,vt);

	glDrawArrays( GL_QUADS, 0,4);

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}



void AURAE_Draw_Text(int ipx,int ipy,char *text)
{
	float fpixelw = 1.0f/320.0f;
	float fpixelh = 1.0f/240.0f;

	int i=0,l = 0;
	float va[8000];
	float vt[8000];

	char c;
	int tx,ty;


	const float tfpixelw = 1.0f/256.0f;
	const float tfpixelh = 1.0f/256.0f;

	const float fw = 16.0f;
	const float fh = 16.0f;

	if(AURAE_mode2D & 1)
	{
		fpixelw = fpixelw*2.0f;
		fpixelh = fpixelh*2.0f;
		ipx *= 2.0f;
		ipy *= 2.0f;
	}

	float x = ipx;
	float y = ipy;

	while(1)
	{
		c = text[i++];
		if(c == ' ')
		{
			x += fw;
			continue;
		}
		if(c == '\n')
		{
			y += fh;
			x = ipx;
			continue;
		}

		if(c == 0)
			break;

		if(l >= 8000) break;

		tx = (c&0x0F)<<4;
		ty = (c&0xF0);

		float px = -1.0f+(fpixelw*x);
		float py = +1.0f-(fpixelh*y);

		float rectx = tfpixelw*tx;
		float recty = tfpixelh*ty;

		vt[l+0] = rectx;
		vt[l+1] = recty;

		vt[l+2] = rectx;
		vt[l+3] = recty+(tfpixelh*fh);

		vt[l+4] = rectx+(tfpixelw*fw);
		vt[l+5] = recty+(tfpixelh*fh);

		vt[l+6] = rectx+(tfpixelw*fw);
		vt[l+7] = recty;

		float w = fpixelw*fw;
		float h = fpixelh*fh;

		va[l+0] = px;
		va[l+1] = py;

		va[l+2] = px;
		va[l+3] = py-h;

		va[l+4] = px+w;
		va[l+5] = py-h;

		va[l+6] = px+w;
		va[l+7] = py;
		l+= 8;

		x += fw;
	}

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2,GL_FLOAT,0,va);
	glTexCoordPointer(2,GL_FLOAT,0,vt);

	glDrawArrays( GL_QUADS, 0,l>>1);

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

}

void AURAE_Draw_Sprite_Array(AURAE_Sprite *sprite,int n)
{
	const float fpixelw = 1.0f/320.0f;
	const float fpixelh = 1.0f/240.0f;

	int i,flag,l = 0;
	float va[16];
	float vt[16];


	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2,GL_FLOAT,0,va);
	glTexCoordPointer(2,GL_FLOAT,0,vt);

	for(i =0;i < n;i++)
	{
		sprite[i].flag &= 0x7F;
		flag = sprite[i].flag;
		if(flag & AURAE_NODRAW) continue;

		Recti rect = sprite[i].rect;
		Vector2i position = sprite[i].position;

		if(AURAE_mode2D & 1)
		{
			position.x = position.x<<1;
			position.y = position.y<<1;
			rect.x = rect.x<<1;
			rect.y = rect.y<<1;
			rect.w = rect.w<<1;
			rect.h = rect.h<<1;
		}

		if(position.x >= 640) continue;
		if(position.y >= 480) continue;
		if(position.x+rect.w <= 0) continue;
		if(position.y+rect.h <= 0) continue;

		sprite[i].flag |= 0x80;

		float px = -1.0f+(fpixelw*position.x);
		float py = +1.0f-(fpixelh*position.y);

		float w = rect.w;
		float h = rect.h;
		float tfpixelw = 0;
		float tfpixelh = 0;

		AURAE_Texture *texture = sprite[i].texture;
		if(texture != NULL)
		{
			glBindTexture(GL_TEXTURE_2D,texture->address);
			tfpixelw = 1.0f/texture->w;
			tfpixelh = 1.0f/texture->h;
		}

		if(AURAE_mode2D & 1)
		{
			tfpixelw = tfpixelw*0.5;
			tfpixelh = tfpixelh*0.5;
		}

		float rectx = tfpixelw*rect.x;
		float recty = tfpixelh*rect.y;

		float rectw = tfpixelw*w;
		float recth = tfpixelh*h;

		vt[0] = rectx;
		vt[1] = recty;

		vt[2] = rectx;
		vt[3] = recty+recth;

		vt[4] = rectx+rectw;
		vt[5] = recty+recth;

		vt[6] = rectx+rectw;
		vt[7] = recty;

		w = fpixelw*w;
		h = fpixelh*h;

		float flagw = 0,flagh = 0;
		if(flag & AURAE_FLIPH) flagw = w;
		if(flag & AURAE_FLIPV) flagh = h;

		va[0] = px+flagw;
		va[1] = py-flagh;

		va[2] = px+flagw;
		va[3] = py-h+flagh;

		va[4] = px+w-flagw;
		va[5] = py-h+flagh;

		va[6] = px+w-flagw;
		va[7] = py-flagh;

		glDrawArrays(GL_QUADS,0,4);
	}


	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}




void AURAE_Draw_TileMap(AURAE_TileMap *tilemap)
{
	int i=0,l = 0,flag;
	float va[16];
	float vt[16];

	const float fpixelw = 1.0f/320.0f;
	const float fpixelh = 1.0f/240.0f;

	float tfpixelw = 1.0f/256.0f;
	float tfpixelh = 1.0f/256.0f;

	float fw = 16.0f;
	float fh = 16.0f;

	float tfw,tfh;

	AURAE_Texture *texture = tilemap->texture;

	glBindTexture(GL_TEXTURE_2D,texture->address);

	Recti rect;
	int h = tilemap->h;
	int w = tilemap->w;
	int id;
	int x,y;
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


	int ntilex = 41;
	int ntiley = 31;

	if(AURAE_mode2D & 1)
	{
		tfpixelw = tfpixelw*0.5;
		tfpixelh = tfpixelh*0.5;
		fw = 32.0f;
		fh = 32.0f;
		px = px<<1;
		py = py<<1;
		ntilex = 21;
		ntiley = 16;
	}

	float fpx = -1.0f+(fpixelw*px);
	float fpy = +1.0f-(fpixelh*py);
	float forg = fpx;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2,GL_FLOAT,0,va);
	glTexCoordPointer(2,GL_FLOAT,0,vt);

	for(y = 0;y < ntiley;y++)
	{
		for(x = 0;x < ntilex;x++)
		{
			flag = tilemap->tiles[i]>>8;

			if(!(flag & AURAE_NODRAW))
			{
				flag = flag&0x03;

				id  = tilemap->tiles[i];
				rect.x = (id&0x0F)<<4;
				rect.y = (id&0xF0);

				if(AURAE_mode2D & 1)
				{
					rect.x = rect.x<<1;
					rect.y = rect.y<<1;
					rect.w = rect.w<<1;
					rect.h = rect.h<<1;
				}

				float flagw = 0,flagh = 0;
				if(flag & AURAE_FLIPH) flagw = fpixelw*fw;
				if(flag & AURAE_FLIPV) flagh = fpixelh*fh;

				float rectx = tfpixelw*rect.x;
				float recty = tfpixelh*rect.y;

				vt[0] = rectx;
				vt[1] = recty;

				vt[2] = rectx;
				vt[3] = recty+(tfpixelh*fh);

				vt[4] = rectx+(tfpixelw*fw);
				vt[5] = recty+(tfpixelh*fh);

				vt[6] = rectx+(tfpixelw*fw);
				vt[7] = recty;

				tfw = fpixelw*fw;
				tfh = fpixelh*fh;

				va[0] = fpx+flagw;
				va[1] = fpy-flagh;

				va[2] = fpx+flagw;
				va[3] = fpy-tfh+flagh;

				va[4] = fpx+tfw-flagw;
				va[5] = fpy-tfh+flagh;

				va[6] = fpx+tfw-flagw;
				va[7] = fpy-flagh;

				glDrawArrays( GL_QUADS, 0,4);
			}

			fpx += fpixelw*fw;
			i++;
		}
		fpy -= fpixelh*fh;
		fpx = forg;
		i += w-x;
	}


	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

}


#endif
