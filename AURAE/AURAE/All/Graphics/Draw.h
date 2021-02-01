#ifndef __AURAE_Draw__
#define __AURAE_Draw__

typedef struct
{
	AURAE_Texture *texture;
	Vector2i position;
	Recti rect;
	int flag;
	int color;
}AURAE_Sprite;

typedef struct
{
	AURAE_Texture *texture;
	Vector2i position;
	int w,h;
	unsigned short *tiles;
}AURAE_TileMap;

typedef struct
{
	int act,old,i,itmp,end;
}AURAE_Anim;

void AURAE_Anim_Init(AURAE_Anim *anim);
int AURAE_Anim_Play(AURAE_Anim *anim,int cadence,int n);
void AURAE_Draw_Sprite(AURAE_Texture *texture,Vector2i position,Recti *rectin,int flag);
void AURAE_Draw_Sprite_Array(AURAE_Sprite *sprite,int n);
void AURAE_Draw_TileMap(AURAE_TileMap *tilemap);
void AURAE_Draw_Text(int x,int y,char *text);

AURAE_TileMap *AURAE_Load_TileMap(char *filename,int offset,void *buffer,int size);

void AURAE_Draw_Config(int flag);

#define  AURAE_FLIPH  0x01
#define  AURAE_FLIPV  0x02
#define  AURAE_NODRAW 0x04

#define  AURAE_DRAW2D_DEFAULT 0x00
#define  AURAE_DRAW2D_320x240 0x01

#endif
