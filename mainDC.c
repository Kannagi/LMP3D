/*
Test Dreamcast pvr lib




*/
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST

#include <kos.h>
#include <time.h>
#include <string.h>
#include "DATA_ROM.c"

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

extern int typetest;
extern int pvr_offset;

int __attribute__((optimize("-O0"), noinline)) main2()
{
/*
	FILE *file = fopen("/sd/DATA","rb");

	int fok =0;
	if(file != NULL)
	{
		fok =1;
		fclose(file);
	}
*/
	int tmpx,i,tmpy;
	LMP3D_TAR tar;
	LMP3D_Model *model;
	LMP3D_Texture* texture,*texture3;

	LMP3D_Tar(&tar,NULL,"zack.bcm",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	model = LMP3D_Load_Model(NULL,tar.offset,DATA_ROM,tar.size);

	LMP3D_Tar(&tar,NULL,"font.png",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	texture = LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);
	//----------

	LMP3D_Tar(&tar,NULL,"tile.pcx",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	texture3 = LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	LMP3D_Texture_Upload(texture3);
	LMP3D_Texture_Free_Pixel(texture3);

	//-------------
	LMP3D_Tar(&tar,NULL,"adell.pcx",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	LMP3D_Texture *texture4 =  LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	LMP3D_Texture_Upload(texture4);
	LMP3D_Texture_Free_Pixel(texture4);

	//---------

	LMP3D_Tar(&tar,NULL,"out.ktm",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	LMP3D_TileMap *tilemap = LMP3D_Load_TileMap(NULL,tar.offset,DATA_ROM,tar.size);
	tilemap->texture = texture3;

	LMP3D_Anim anim;

	LMP3D_Anim_Init(&anim);

	model->position.z = 400;
	model->position.y = 100;

	char strinfo[100];

	LMP3D_Camera camera;
	camera = LMP3D_Camera_Init();

	int number = 1;
	int vblank = 0;

	LMP3D_Event event;
	LMP3D_Sprite sprite[2];

	for(i = 0; i <2;i++)
	{
		sprite[i].texture = texture4;
		sprite[i].position.x = 16 + 64*i;
		sprite[i].position.y = 128;

		sprite[i].rect.x = 0;
		sprite[i].rect.y = 56*i;
		sprite[i].rect.w = 32;
		sprite[i].rect.h = 56;

		sprite[i].color = 0x808080;
		sprite[i].flag = 0x01;
	}

	LMP3D_Texture tex = *texture;
	tex.psm = TA_TEXTURE_RGB4444;

	int mode = 0;

	float mid = model->Ymax/2;

	//model->scale.x = model->scale.y = model->scale.z = 0.01;
	mid = mid*model->scale.x;

	model->position = LMP3D_Type_Vector3(0,-mid,-400);
	Vector3 position = model->position;
	position.y = 0;

	while(1)
	{
		//model->rotate.y += 0.01;
		LMP3D_Event_Update(&event);

		LMP3D_Clear();


		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x7)<<5;
			tmpy = (i&0xF8)<<4;

			model->position.x = tmpx;
			model->position.y = tmpy-100;

			LMP3D_Model_Draw(model);
		}

		vblank = LMP3D_VBlank();

		LMP3D_Camera_Ortho2D();
/*
		LMP3D_Draw_Config(1);

		LMP3D_Draw_TileMap(tilemap);

		LMP3D_Draw_Sprite(model->texture[0],LMP3D_Type_Vector2i(0,0),NULL,1);
*/
		LMP3D_Texture_Setup(texture);
		LMP3D_Draw_Text(16,16,strinfo);

		sprite[1].rect.x = 32*anim.i;
		LMP3D_Anim_Play(&anim,2,6);

		LMP3D_Draw_Sprite_Array(sprite,2);

		LMP3D_FlipBuffer(NULL);

		camera.key[0] = event.key[Button_Left];
		camera.key[1] = event.key[Button_Right];
		camera.key[2] = event.key[Button_Down];
		camera.key[3] = event.key[Button_Up];

		camera.key[4] = event.key[Button_A];
		camera.key[5] = event.key[Button_B];

		if(event.key[Button_Start] == LMP3D_KEY_DOWN)
		{
			mode = !mode;
		}

		//camera
		if(mode == 0)
			LMP3D_Camera_ViewSub(&camera);
		else
			LMP3D_Camera_ViewObj(&camera,&position);

		LMP3D_Camera_Perspective(camera);

		if(event.key[Button_Left]  == LMP3D_KEY_DOWNW) tilemap->position.x -= 4;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) tilemap->position.x += 4;
		if(event.key[Button_Up]    == LMP3D_KEY_DOWNW) tilemap->position.y -= 4;
		if(event.key[Button_Down]  == LMP3D_KEY_DOWNW) tilemap->position.y += 4;


		if(event.key[Button_R1] == LMP3D_KEY_DOWN) number++;
		if(event.key[Button_L1] == LMP3D_KEY_DOWN) number--;

		if(event.key[Button_X] == LMP3D_KEY_DOWN) number+=10;
		if(event.key[Button_Y] == LMP3D_KEY_DOWN) number-=10;


		if(event.key[Button_A] == LMP3D_KEY_DOWN) typetest++;
		typetest &=3;
/*
		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) tilemap->position.y -= 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) tilemap->position.y += 8;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) tilemap->position.x += 8;
		if(event.key[Button_Left] == LMP3D_KEY_DOWNW) tilemap->position.x -= 8;

		if(event.key[Button_X] == LMP3D_KEY_DOWNW) model->position.z += 8;
		if(event.key[Button_Y] == LMP3D_KEY_DOWNW) model->position.z -= 8;




		if(event.key[Button_R1] == LMP3D_KEY_DOWNW) tex.address += 0x8000;
		if(event.key[Button_L1] == LMP3D_KEY_DOWNW) tex.address -= 0x8000;

		if(event.key[Button_A] == LMP3D_KEY_DOWN) typetest++;
		typetest &=3;

	*/

		sprintf(strinfo,"Vblank :%d\nnumber %d\nTri %d ,VRAM : %x ,type %x\n",vblank,number,model->nf*number,pvr_offset,typetest);

		LMP3D_VBlank();
	}

	return 0;
}





#endif
