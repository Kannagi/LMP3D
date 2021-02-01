/*
Test Dreamcast pvr lib




*/
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST

#include <kos.h>
#include <time.h>
#include <string.h>
#include "DATA_ROM.inc"

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

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
	AURAE_TAR tar;
	AURAE_Model *model;
	AURAE_Texture* texture,*texture3;

	AURAE_Tar(&tar,NULL,"zack.bcm",AURAE_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	model = AURAE_Load_Model(NULL,tar.offset,DATA_ROM,tar.size);

	AURAE_Tar(&tar,NULL,"font.png",AURAE_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	texture = AURAE_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	AURAE_Texture_Upload(texture);
	AURAE_Texture_Free_Pixel(texture);
	//----------

	AURAE_Tar(&tar,NULL,"tile.pcx",AURAE_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	texture3 = AURAE_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	AURAE_Texture_Upload(texture3);
	AURAE_Texture_Free_Pixel(texture3);

	//-------------
	AURAE_Tar(&tar,NULL,"adell.pcx",AURAE_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	AURAE_Texture *texture4 =  AURAE_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);
	AURAE_Texture_Upload(texture4);
	AURAE_Texture_Free_Pixel(texture4);

	//---------

	AURAE_Tar(&tar,NULL,"out.ktm",AURAE_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	AURAE_TileMap *tilemap = AURAE_Load_TileMap(NULL,tar.offset,DATA_ROM,tar.size);
	tilemap->texture = texture3;

	AURAE_Anim anim;

	AURAE_Anim_Init(&anim);

	model->position.z = 400;
	model->position.y = 100;

	char strinfo[100];

	AURAE_Camera camera;
	camera = AURAE_Camera_Init();

	int number = 1;
	int vblank = 0;

	AURAE_Event event;
	AURAE_Sprite sprite[2];

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

	AURAE_Texture tex = *texture;
	tex.psm = TA_TEXTURE_RGB4444;

	int mode = 0;

	float mid = model->Ymax/2;

	//model->scale.x = model->scale.y = model->scale.z = 0.01;
	mid = mid*model->scale.x;

	model->position = AURAE_Type_Vector3(0,-mid,-400);
	Vector3 position = model->position;
	position.y = 0;

	while(1)
	{
		//model->rotate.y += 0.01;
		AURAE_Event_Update(&event);

		AURAE_Clear();


		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x7)<<5;
			tmpy = (i&0xF8)<<4;

			model->position.x = tmpx;
			model->position.y = tmpy-100;

			AURAE_Model_Draw(model);
		}

		AURAE_Camera_Ortho2D();

		vblank = AURAE_VBlank();

		AURAE_Camera_Ortho2D();

		AURAE_Texture_Setup(texture);
		AURAE_Draw_Text(16,16,strinfo);
/*
		AURAE_Draw_Config(1);

		AURAE_Draw_TileMap(tilemap);

		AURAE_Draw_Sprite(model->texture[0],AURAE_Type_Vector2i(0,0),NULL,1);

		AURAE_Texture_Setup(texture);
		AURAE_Draw_Text(16,16,strinfo);

		sprite[1].rect.x = 32*anim.i;
		AURAE_Anim_Play(&anim,2,6);

		AURAE_Draw_Sprite_Array(sprite,2);
*/
		AURAE_FlipBuffer(NULL);

		camera.key[0] = event.key[Button_Left];
		camera.key[1] = event.key[Button_Right];
		camera.key[2] = event.key[Button_Down];
		camera.key[3] = event.key[Button_Up];

		camera.key[4] = event.key[Button_A];
		camera.key[5] = event.key[Button_B];

		if(event.key[Button_Start] == AURAE_KEY_DOWN)
		{
			mode = !mode;
		}

		//camera
		if(mode == 0)
			AURAE_Camera_ViewSub(&camera);
		else
			AURAE_Camera_ViewObj(&camera,&position);

		AURAE_Camera_Perspective(camera);

		if(event.key[Button_Left]  == AURAE_KEY_DOWNW) tilemap->position.x -= 4;
		if(event.key[Button_Right] == AURAE_KEY_DOWNW) tilemap->position.x += 4;
		if(event.key[Button_Up]	== AURAE_KEY_DOWNW) tilemap->position.y -= 4;
		if(event.key[Button_Down]  == AURAE_KEY_DOWNW) tilemap->position.y += 4;


		if(event.key[Button_R1] == AURAE_KEY_DOWN) number++;
		if(event.key[Button_L1] == AURAE_KEY_DOWN) number--;

		if(event.key[Button_X] == AURAE_KEY_DOWN) number+=10;
		if(event.key[Button_Y] == AURAE_KEY_DOWN) number-=10;


		if(event.key[Button_A] == AURAE_KEY_DOWN) typetest++;
		typetest &=3;
/*
		if(event.key[Button_Up] == AURAE_KEY_DOWNW) tilemap->position.y -= 8;
		if(event.key[Button_Down] == AURAE_KEY_DOWNW) tilemap->position.y += 8;
		if(event.key[Button_Right] == AURAE_KEY_DOWNW) tilemap->position.x += 8;
		if(event.key[Button_Left] == AURAE_KEY_DOWNW) tilemap->position.x -= 8;

		if(event.key[Button_X] == AURAE_KEY_DOWNW) model->position.z += 8;
		if(event.key[Button_Y] == AURAE_KEY_DOWNW) model->position.z -= 8;




		if(event.key[Button_R1] == AURAE_KEY_DOWNW) tex.address += 0x8000;
		if(event.key[Button_L1] == AURAE_KEY_DOWNW) tex.address -= 0x8000;

		if(event.key[Button_A] == AURAE_KEY_DOWN) typetest++;
		typetest &=3;

	*/

		sprintf(strinfo,"Vblank :%d\nnumber %d\nTri %d ,VRAM : %x ,type %x\n",vblank,number,model->nv*number,pvr_offset,typetest);

		AURAE_VBlank();
	}

	return 0;
}





#endif
