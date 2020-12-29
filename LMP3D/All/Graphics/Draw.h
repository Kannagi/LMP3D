#ifndef __LMP3D_Draw__
#define __LMP3D_Draw__

typedef struct
{
	LMP3D_Texture *texture;
	Vector2i position;
	Recti rect;
	int flag;
	int color;
}LMP3D_Sprite;

typedef struct
{
	LMP3D_Texture *texture;
	Vector2i position;
	int w,h;
	unsigned short *tiles;
}LMP3D_TileMap;

typedef struct
{
	int act,old,i,itmp,end;
}LMP3D_Anim;

void LMP3D_Anim_Init(LMP3D_Anim *anim);
int LMP3D_Anim_Play(LMP3D_Anim *anim,int cadence,int n);
void LMP3D_Draw_Sprite(LMP3D_Texture *texture,Vector2i position,Recti *rectin,int flag);
void LMP3D_Draw_Sprite_Array(LMP3D_Sprite *sprite,int n);
void LMP3D_Draw_TileMap(LMP3D_TileMap *tilemap);
void LMP3D_Draw_Text(int x,int y,char *text);

LMP3D_TileMap *LMP3D_Load_TileMap(char *filename,int offset,void *buffer,int size);

void LMP3D_Draw_Config(int flag);

#define  LMP3D_FLIPH  0x01
#define  LMP3D_FLIPV  0x02
#define  LMP3D_NODRAW 0x04

#define  LMP3D_DRAW2D_DEFAULT 0x00
#define  LMP3D_DRAW2D_320x240 0x01

#endif
