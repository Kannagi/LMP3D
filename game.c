#include <stdio.h>
#include <stdlib.h>

#ifndef PLAYSTATION1
#include <time.h>
#endif

#include "LMP3D/LMP3D.h"


/*
extern unsigned char *DATA_ROM;
extern unsigned int size_DATA_ROM;
*/

void game(LMP3D_Buffer *buffer)
{
	LMP3D_TAR tar;

	LMP3D_Event event;
	LMP3D_Camera camera;
	camera = LMP3D_Camera_Init();

	LMP3D_Event_Init(&event);
	LMP3D_Model *model;

	unsigned int vram = LMP3D_VRAM_Get();

/*
	int size;
	FILE *file;

	file = fopen("DATA", "rb");

		fseek(file, 0, SEEK_END);
		size = ftell(file);
	//rewind(file);
	fseek(file, 0, SEEK_SET);
	DATA_ROM = malloc(size);

	fread(DATA_ROM, 1, size, file);
	fclose(file);



LMP3D_Tar(&tar,"","zack.bcm",LMP3D_TAR_OFFSET,DATA_ROM,size);

	model = LMP3D_Load_Model("",tar.offset,DATA_ROM,size);

	LMP3D_Tar(&tar,"","font.png",LMP3D_TAR_OFFSET,DATA_ROM,size);

	LMP3D_Texture *texture = LMP3D_Load_Texture("",tar.offset,DATA_ROM,tar.size);


*/

	LMP3D_Tar(&tar,"DATA","zack.bcm",LMP3D_TAR_OFFSET,NULL,0);

	model = LMP3D_Load_Model("DATA",tar.offset,NULL,0);
/*
	LMP3D_Model_Free(model);
	model = LMP3D_Load_Model("DATA",tar.offset,NULL,0);*/
	//model = LMP3D_Load_Model("PC/Tidus.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("DC/zack.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("sophia.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("Lightning/lightning.bcm",0,NULL,0);
	//model->scale.x = model->scale.y = model->scale.z = 164.0f/model->Ymax;

	LMP3D_Tar(&tar,"DATA","font.png",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);

	char string[100],strfps[50];
	int vblank = 0;

	int t_end,t_begin,fps = 0,total=0;

	int i,number = 1,tmpx,tmpy;

	LMP3D_Tar(&tar,"DATA","adell.pcx",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture2 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture2);
	LMP3D_Texture_Free_Pixel(texture2);

	LMP3D_Sprite sprite[2];

	LMP3D_Tar(&tar,"DATA","tile.pcx",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture3 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture3);
	LMP3D_Texture_Free_Pixel(texture3);

	LMP3D_Tar(&tar,"DATA","out.ktm",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_TileMap *tilemap = LMP3D_Load_TileMap("DATA",tar.offset,NULL,tar.size);
	tilemap->texture = texture3;

	LMP3D_Anim anim;

	LMP3D_Anim_Init(&anim);

	for(i = 0; i <2;i++)
	{
		sprite[i].texture = texture2;
		sprite[i].position.x = 16 + 64*i;
		sprite[i].position.y = 128;

		sprite[i].rect.x = 0;
		sprite[i].rect.y = 56*i;
		sprite[i].rect.w = 32;
		sprite[i].rect.h = 56;

		sprite[i].color = 0x808080;
		sprite[i].flag = 0x01;
	}

	//LMP3D_Texture tex = *model->texture[0];
	//tex.psm = 0x2;

	float mid = model->Ymax/2;

	mid = mid*model->scale.x;

	model->position = LMP3D_Type_Vector3(0,-mid,-400);
	Vector3 position = model->position;
	position.y = 0;

	//model->rotate.x += -PI/2;

	int mode = 0;
	while(event.exit == 0)
    {
    	//model->rotate.y += 0.01;
		LMP3D_Event_Update(&event);

		camera.key[0] = event.key[Button_Left];
		camera.key[1] = event.key[Button_Right];
		camera.key[2] = event.key[Button_Down];
		camera.key[3] = event.key[Button_Up];

		camera.key[4] = event.key[Button_A];
		camera.key[5] = event.key[Button_B];

		if(event.key[Button_Start] == LMP3D_KEY_DOWN)
		{
			printf("ok\n");
			mode = !mode;
		}

		//camera
		if(mode == 0)
			LMP3D_Camera_ViewSub(&camera);
		else
			LMP3D_Camera_ViewObj(&camera,&position);

		if(event.key[Button_R2] == LMP3D_KEY_DOWN) number +=10;
		if(event.key[Button_L2] == LMP3D_KEY_DOWN) number -=10;
		if(event.key[Button_R1] == LMP3D_KEY_DOWN) number++;
		if(event.key[Button_L1] == LMP3D_KEY_DOWN) number--;


		if(event.key[Button_Left]  == LMP3D_KEY_DOWNW) tilemap->position.x -= 4;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) tilemap->position.x += 4;
		if(event.key[Button_Up]    == LMP3D_KEY_DOWNW) tilemap->position.y -= 4;
		if(event.key[Button_Down]  == LMP3D_KEY_DOWNW) tilemap->position.y += 4;

		LMP3D_Camera_Perspective(camera);

		LMP3D_Clear();


		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x7)<<5;
			tmpy = (i&0xF8)<<4;

			model->position.x = tmpx;
			model->position.y = tmpy-100;
			model->position.z = -400;

			LMP3D_Model_Draw(model);
		}

/*

		for(i = 0;i <  number;i++)
		{
			tmpx = (i%20)*30;
			//tmpy = (i/30)*140;
			tmpy = 0;
			model->position.x = tmpx+p.x;
			model->position.y = mid-tmpy+p.y;
			model->position.z = 400+p.z;


		}
*/

		LMP3D_Camera_Ortho2D();

/*
		LMP3D_Draw_Config(1);



		LMP3D_Draw_TileMap(tilemap);

		LMP3D_Draw_Sprite(model->texture[0],LMP3D_Type_Vector2i(0,0),NULL,1);
*/



		//sprintf(string,"ms :%d\nnumber %d\nTri %d\n",vblank,number,model->nf*number);
		//sprintf(string,"Time %s\nVblank :%d\nnumber %d\nTri %d\nperf %x\n%d\n",strfps,vblank,number,model->nf*number,model->texture[0]->addresspal,mode);

		sprintf(string,"Time %s\nVblank :%d\nnumber %d\nTri %d\nVertex %d\n\n",strfps,vblank,number,model->nf*number,model->nv*number);
		LMP3D_Texture_Setup(texture);
		LMP3D_Draw_Text(8,8,string);

		sprite[1].rect.x = 32*anim.i;
		LMP3D_Anim_Play(&anim,5,6);

		LMP3D_Draw_Sprite_Array(sprite,2);




		//printf("p: %f\n",model->position.z);
		//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);


#ifndef PLAYSTATION1

		t_end = clock();
		if(fps >= 60)
		{

			total += t_end-t_begin;
			total = t_end-t_begin;
			sprintf(strfps,"%d/%d",total,(int)(CLOCKS_PER_SEC)/60);
			total = 0;
			fps = 0;
		}
		fps++;
		t_begin = clock();


		LMP3D_FlipBuffer(buffer);
		vblank = LMP3D_VBlank();

#endif

	}

	LMP3D_VRAM_Set(vram);
}
