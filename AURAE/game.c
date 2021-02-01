#include <stdio.h>
#include <stdlib.h>

#ifndef PLAYSTATION1
#include <time.h>
#endif
/*
#include <stdint.h>
uint64_t rdtsc()
{
	register uint64_t a = 0, d = 0;
	asm volatile("mfence");

	asm volatile("rdtsc" : "=a"(a), "=d"(d));

	a = (d << 32) | a;
	asm volatile("mfence");
	return a;
}
*/
#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"
//#include <GL/gl.h>
/*
extern unsigned char *DATA_ROM;
extern unsigned int size_DATA_ROM;
*/
int varg_debug = 0;

AURAE_Anim3D MNS_load_3D_anim(char *nom_du_fichier);


int __attribute__((optimize("-O0"), noinline))free_ram()
{
	//char *adr2 = malloc(0x100000);
	int *adr = malloc(0x10);
	int var;
	int fr = (&var) - (int)adr;
	free(adr);
	//free(adr2);
	return fr;
}

void game(AURAE_Buffer *buffer)
{
	AURAE_TAR tar;
	printf("Begin game\n----------------\n");

	AURAE_Event event;
	AURAE_Camera camera;
	camera = AURAE_Camera_Init();

	AURAE_Event_Init(&event);
	AURAE_Model *model;



	unsigned int vram = AURAE_VRAM_Get();

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



AURAE_Tar(&tar,"","zack.bcm",AURAE_TAR_OFFSET,DATA_ROM,size);

	model = AURAE_Load_Model("",tar.offset,DATA_ROM,size);

	AURAE_Tar(&tar,"","font.png",AURAE_TAR_OFFSET,DATA_ROM,size);

	AURAE_Texture *texture = AURAE_Load_Texture("",tar.offset,DATA_ROM,tar.size);


*/


	int fr = free_ram();

	AURAE_Tar(&tar,"DATA","zack.bcm",AURAE_TAR_OFFSET,NULL,0);

	//AURAE_Tar(&tar,"DATA","Tidus/Tidus.bcm",AURAE_TAR_OFFSET,NULL,0);

	model = AURAE_Load_Model("DATA",tar.offset,NULL,0);
	//model = AURAE_Load_Model("boy.bcm",NULL,NULL,0);
	//model = AURAE_Load_Model("teapot.bcm",NULL,NULL,0);
	//model = AURAE_Load_Model("lightning.bcm",NULL,NULL,0);
	//model = AURAE_Load_Model("zack.bcm",0,NULL,0);
	//model = AURAE_Load_Model("untitled.bcm",NULL,NULL,0);
/*
	AURAE_Model_Free(model);
	model = AURAE_Load_Model("DATA",tar.offset,NULL,0);*/
	//model = AURAE_Load_Model("PC/Tidus.bcm",0,NULL,0);

	//model = AURAE_Load_Model("DC/zack.bcm",0,NULL,0);

	//model = AURAE_Load_Model("sophia.bcm",0,NULL,0);

	//model = AURAE_Load_Model("Lightning/lightning.bcm",0,NULL,0);
	//model->scale.x = model->scale.y = model->scale.z = 164.0f/model->Ymax;


	//model->anim = MNS_load_3D_anim("run.smd");

	int i,l,j;

/*

	for( i = 0;i < model->nbones;i++)
	{
		l = i*16;

		model->anim.skeleton[i].matrix[0+12] += model->matrix_bones[l+12+0];
		model->anim.skeleton[i].matrix[1+12] -= model->matrix_bones[l+12+1];
		model->anim.skeleton[i].matrix[2+12] -= model->matrix_bones[l+12+2];

		model->anim.skeleton[i].matrix[1+12] = -model->anim.skeleton[i].matrix[1+12];
		model->anim.skeleton[i].matrix[2+12] = -model->anim.skeleton[i].matrix[2+12];
	}*/

	AURAE_Tar(&tar,"DATA","font.png",AURAE_TAR_OFFSET,NULL,0);
	AURAE_Texture *texture = AURAE_Load_Texture("DATA",tar.offset,NULL,tar.size);
	AURAE_Texture_Upload(texture);
	AURAE_Texture_Free_Pixel(texture);

	char string[100],strfps[50];
	int vblank = 0;

	//int t_end,t_begin,fps = 0,total=0;

	int number = 1,tmpx,tmpy;

	AURAE_Tar(&tar,"DATA","adell.pcx",AURAE_TAR_OFFSET,NULL,0);
	AURAE_Texture *texture2 = AURAE_Load_Texture("DATA",tar.offset,NULL,tar.size);
	AURAE_Texture_Upload(texture2);
	AURAE_Texture_Free_Pixel(texture2);

	AURAE_Sprite sprite[2];

	AURAE_Tar(&tar,"DATA","tile.pcx",AURAE_TAR_OFFSET,NULL,0);
	AURAE_Texture *texture3 = AURAE_Load_Texture("DATA",tar.offset,NULL,tar.size);
	AURAE_Texture_Upload(texture3);
	AURAE_Texture_Free_Pixel(texture3);

	AURAE_Tar(&tar,"DATA","out.ktm",AURAE_TAR_OFFSET,NULL,0);
	AURAE_TileMap *tilemap = AURAE_Load_TileMap("DATA",tar.offset,NULL,tar.size);
	tilemap->texture = texture3;

	AURAE_Anim anim;

	AURAE_Anim_Init(&anim);

	for(i = 0; i <2;i++)
	{
		sprite[i].texture = texture2;
		sprite[i].position.x = 16 + 64*i;
		sprite[i].position.y = 200-28;

		sprite[i].rect.x = 0;
		sprite[i].rect.y = 56*i;
		sprite[i].rect.w = 32;
		sprite[i].rect.h = 56;

		sprite[i].color = 0x808080;
		sprite[i].flag = 0x01;
	}

	//AURAE_Texture tex = *model->texture[0];
	//tex.psm = 0x2;

	float mid = model->Ymax/2;

	mid = mid*model->scale.x;

	model->position = AURAE_Type_Vector3(0,-mid,-200);

	//float scale = 32.0f;
	//model->scale = AURAE_Type_Vector3(scale,scale,scale);
	Vector3 position = model->position;
	position.y = 0;

	camera.angle.z = position.z;
	camera.speed = 0.04f;

	//printf("%f\n",camera.angle.z );
	//model->rotate.x += -PI/2;


	int mode = 1;
	int mode2 = 0;
	int fr2 = free_ram();

	while(event.exit == 0)
	{
		//model->rotate.y += 0.01;
		AURAE_Event_Update(&event);

		camera.key[0] = event.key[Button_Left];
		camera.key[1] = event.key[Button_Right];
		camera.key[2] = event.key[Button_Down];
		camera.key[3] = event.key[Button_Up];

		camera.key[4] = event.key[Button_A];
		camera.key[5] = event.key[Button_B];

		if(event.key[Button_L3] == AURAE_KEY_DOWN)
		{
			mode2 = !mode2;
		}

		if(event.key[Button_Start] == AURAE_KEY_DOWN)
		{
			mode = !mode;
		}

		if(event.key[Button_R3] == AURAE_KEY_DOWN)
		{
		}



		if(event.key[Button_Triangle] == AURAE_KEY_DOWN)
		{

		}


		/*
		if(event.key[Button_X] == AURAE_KEY_DOWN)
			varg_debug++;

		if(event.key[Button_Y] == AURAE_KEY_DOWN)
			varg_debug--;

		varg_debug = varg_debug&3;
*/
		//camera
		if(mode == 0)
			AURAE_Camera_ViewSub(&camera);
		else
			AURAE_Camera_ViewObj(&camera,&position);

		if(event.key[Button_R2] == AURAE_KEY_DOWN) number +=10;
		if(event.key[Button_L2] == AURAE_KEY_DOWN) number -=10;
		if(event.key[Button_R1] == AURAE_KEY_DOWN) number++;
		if(event.key[Button_L1] == AURAE_KEY_DOWN) number--;


		if(event.key[Button_Left]  == AURAE_KEY_DOWNW) tilemap->position.x -= 4;
		if(event.key[Button_Right] == AURAE_KEY_DOWNW) tilemap->position.x += 4;
		if(event.key[Button_Up]	== AURAE_KEY_DOWNW) tilemap->position.y -= 4;
		if(event.key[Button_Down]  == AURAE_KEY_DOWNW) tilemap->position.y += 4;

/*
		if(event.key[Button_Select] == AURAE_KEY_DOWN)
		{
			mode2++;
			mode2 = mode2&7;

		}*/
		AURAE_Clear();


		AURAE_Camera_Perspective(camera);

/*
		if(event.key[Button_Select] == AURAE_KEY_DOWN)
		{
			mode2 = !mode2;
			if(mode2)
				SetGsCrt(0, 2, 0);
			else
				SetGsCrt(1, 2, 1);
		}

		if(mode2 == 0)
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x0F)<<5;
			tmpy = (i&0xF0)<<3;

			model->position.x = tmpx;
			model->position.y = tmpy-100;


			AURAE_Model_Draw(model);
		}

		if(mode2 == 0)
		//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
*/
		AURAE_Camera_Ortho2D();


		//AURAE_Draw_Config(1);



		//AURAE_Draw_TileMap(tilemap);

		//AURAE_Draw_Sprite(model->texture[0],AURAE_Type_Vector2i(0,0),NULL,1);
		//AURAE_Draw_Sprite(texture3,AURAE_Type_Vector2i(0,0),NULL,1);




		//sprintf(string,"ms :%d\nnumber %d\nTri %d\n",vblank,number,model->nf*number);
		//sprintf(string,"Time %s\nVblank :%d\nnumber %d\nTri %d\nperf %x\n%d\n",strfps,vblank,number,model->nf*number,model->texture[0]->addresspal,mode);

		//sprintf(string,"Vblank :%d/54770\nnumber %d\nTri %d\nVertex %d",vblank,number,model->nf*number,model->nv*number);

		sprintf(string,"Tri %d\nVertex %d\nvblank: %d\nvram : %x",model->nf*number,model->nv*number,vblank,AURAE_VRAM_Get());
		AURAE_Texture_Setup(texture);
		AURAE_Draw_Text(8,8,string);


		sprintf(string,"CPU :%d\%",100-(int)((54660-vblank)*0.00183f));
		AURAE_Texture_Setup(texture);
		AURAE_Draw_Text(8,100,string);

/*
		sprintf(string,"Cycle :%d",(54770-vblank)*91);
		AURAE_Texture_Setup(texture);
		//AURAE_Draw_Text(8,200,string);


		AURAE_VRAM_Info(string);
		AURAE_Draw_Text(8,200,string);

		sprite[1].rect.x = 32*anim.i;
		AURAE_Anim_Play(&anim,5,6);
*/
		AURAE_Draw_Sprite_Array(sprite,2);


		sprintf(string,"RAM :%d\nuse :%d",fr,fr-fr2);
		AURAE_Texture_Setup(texture);
		AURAE_Draw_Text(8,120,string);


		//printf("p: %f\n",model->position.z);
		//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);


		AURAE_Camera_Perspective(camera);

		AURAE_VBlank();


		i = 0;
		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x0F)<<5;
			tmpy = (i&0xF0)<<3;

			model->position.x = tmpx;
			model->position.y = tmpy-100;


			AURAE_Model_Draw(model);
		}


		AURAE_FlipBuffer(buffer);

		vblank = AURAE_VBlank();


	}

	AURAE_VRAM_Set(vram);
}


