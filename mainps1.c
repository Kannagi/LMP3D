#include <stdio.h>

#ifdef PLAYSTATION1

#include <psx.h>


#include "LMP3D/LMP3D.h"
#include "LMP3D/PS1/PS1.h"

//#include "DATA_ROM2.c"

void game2(LMP3D_Buffer *buffer)
{

	unsigned int vram = LMP3D_VRAM_Get();

	//--------------------------------
	int fps = 0;
	int i,vb;
	LMP3D_Event event;
	LMP3D_Event_Init(&event);

	LMP3D_Camera camera;
	camera = LMP3D_Camera_Init();

	LMP3D_Model *model;
	LMP3D_Texture* texture,*texture2,*texture3;

#if 0
	LMP3D_TAR tar;

	LMP3D_Tar(&tar,NULL,"zack.bcm",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	model = LMP3D_Load_Model(NULL,tar.offset,DATA_ROM,tar.size);

/*
	LMP3D_Tar(&tar,NULL,"ffccdif_1.pcx",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	texture = LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);*/

#else

	model = LMP3D_Load_Model("ZACK.BCM",0,NULL,0);

	//---------------------
	texture = LMP3D_Load_Texture("adell.PCX",0,NULL,0);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);

	//---------------------
	texture2 = LMP3D_Load_Texture("FONT.PCX",0,NULL,0);
	LMP3D_Texture_Upload(texture2);
	LMP3D_Texture_Free_Pixel(texture2);

	//---------------------
	texture3 = LMP3D_Load_Texture("TILE.PCX",0,NULL,0);
	LMP3D_Texture_Upload(texture3);
	LMP3D_Texture_Free_Pixel(texture3);

#endif

	LMP3D_TileMap *tilemap = LMP3D_Load_TileMap("OUT.KTM",0,NULL,0);
	tilemap->texture = texture3;

	char info[100];
	char str[100];


	model->iposition = LMP3D_Type_Vector3i(0,80,400);
	model->irotate = LMP3D_Type_Vector3s(0x400,0,0);
	model->iscale = LMP3D_Type_Vector3s(1<<12,1<<12,1<<12);

	LMP3D_Sprite sprite[2];

	for(i = 0; i <2;i++)
	{
		sprite[i].texture = texture;
		sprite[i].position.x = 16;
		sprite[i].position.y = 64+(i<<6);

		sprite[i].rect.x = 0;
		sprite[i].rect.y = 56*i;
		sprite[i].rect.w = 32;
		sprite[i].rect.h = 56;

		sprite[i].color = 0x808080;
		sprite[i].flag = 0;
	}

	LMP3D_Anim anim;

	LMP3D_Anim_Init(&anim);


	while(1)
	{
		model->irotate.y += 0x18;
		LMP3D_Event_Update(&event);
		LMP3D_Camera_Perspective(camera);
/*
		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) sposition.y -= 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) sposition.y += 8;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) sposition.x += 8;
		if(event.key[Button_Left] == LMP3D_KEY_DOWNW) sposition.x -= 8;
*/
		if(event.key[Button_Cross] == LMP3D_KEY_DOWNW) model->iposition.z += 16;
		if(event.key[Button_Circle] == LMP3D_KEY_DOWNW) model->iposition.z -= 16;

		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) tilemap->position.y -= 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) tilemap->position.y += 8;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) tilemap->position.x += 8;
		if(event.key[Button_Left] == LMP3D_KEY_DOWNW) tilemap->position.x -= 8;

		if(fps >= 60)
		{
			LMP3D_VRAM_Info(info);
			sprintf(str,"VBlank %d/56251\n%s",vb,info); //0X10800 max vb

			fps = 0;
		}
		fps++;

		LMP3D_VBlank();

		LMP3D_Clear();
/*
		LMP3D_Draw_TileMap(tilemap);
*/
		sprite[1].rect.x = 32*anim.i;
		LMP3D_Anim_Play(&anim,3,6);

		LMP3D_Draw_Sprite_Array(sprite,2);

		LMP3D_Model_Draw(model);

		LMP3D_Texture_Setup(texture2);
		LMP3D_Draw_Text(8,8,str);

		LMP3D_FlipBuffer(buffer);

		vb = LMP3D_VBlank();
	}

	LMP3D_VRAM_Set(vram);
}
#endif


