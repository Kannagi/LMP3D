#include <stdio.h>
#include <stdlib.h>

#ifndef PLAYSTATION1
#include <time.h>
#endif

#ifdef EMSCRIPTEN
#include <emscripten.h>
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
#include "LMP3D/LMP3D.h"
#include "LMP3D/PS2/PS2.h"
//#include <GL/gl.h>
/*
extern unsigned char *DATA_ROM;
extern unsigned int size_DATA_ROM;
*/
int varg_debug = 0;

LMP3D_Anim3D MNS_load_3D_anim(char *nom_du_fichier);


static LMP3D_Event event;
static LMP3D_Camera camera;
static int mode = 1;
static int mode2 = 0;
static Vector3 position;
static int number;
static LMP3D_TileMap* tilemap;
static LMP3D_Model *model;
static char string[100],strfps[50];
static int vblank = 0;
static LMP3D_Texture* texture;
static unsigned int vram;
static LMP3D_Sprite sprite[2];
static LMP3D_Anim anim;
static LMP3D_Buffer *buffer;

void update(){

//model->rotate.y += 0.01;
	LMP3D_Event_Update(&event);

	camera.key[0] = event.key[Button_Left];
	camera.key[1] = event.key[Button_Right];
	camera.key[2] = event.key[Button_Down];
	camera.key[3] = event.key[Button_Up];

	camera.key[4] = event.key[Button_A];
	camera.key[5] = event.key[Button_B];

	if(event.key[Button_L3] == LMP3D_KEY_DOWN)
	{
		mode2 = !mode2;
	}

	if(event.key[Button_Start] == LMP3D_KEY_DOWN)
	{
		mode = !mode;
	}
	/*
	if(event.key[Button_X] == LMP3D_KEY_DOWN)
		varg_debug++;

	if(event.key[Button_Y] == LMP3D_KEY_DOWN)
		varg_debug--;

	varg_debug = varg_debug&3;
*/
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
	if(event.key[Button_Up]	== LMP3D_KEY_DOWNW) tilemap->position.y -= 4;
	if(event.key[Button_Down]  == LMP3D_KEY_DOWNW) tilemap->position.y += 4;

/*
	if(event.key[Button_Select] == LMP3D_KEY_DOWN)
	{
		mode2++;
		mode2 = mode2&7;

	}*/
	LMP3D_Clear();


	LMP3D_Camera_Perspective(camera);

/*
	if(event.key[Button_Select] == LMP3D_KEY_DOWN)
	{
		mode2 = !mode2;
		if(mode2)
			SetGsCrt(0, 2, 0);
		else
			SetGsCrt(1, 2, 1);
	}
*/
	if(mode2 == 0)
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	for(int i = 0;i <  number;i++)
	{
		int tmpx,tmpy;
		tmpx = (i&0x0F)<<5;
		tmpy = (i&0xF0)<<3;

		model->position.x = tmpx;
		model->position.y = tmpy-100;


		LMP3D_Model_Draw(model);
	}

	if(mode2 == 0)
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	LMP3D_Camera_Ortho2D();


	//LMP3D_Draw_Config(1);



	//LMP3D_Draw_TileMap(tilemap);

	//LMP3D_Draw_Sprite(model->texture[0],LMP3D_Type_Vector2i(0,0),NULL,1);
	//LMP3D_Draw_Sprite(texture3,LMP3D_Type_Vector2i(0,0),NULL,1);




	//sprintf(string,"ms :%d\nnumber %d\nTri %d\n",vblank,number,model->nf*number);
	//sprintf(string,"Time %s\nVblank :%d\nnumber %d\nTri %d\nperf %x\n%d\n",strfps,vblank,number,model->nf*number,model->texture[0]->addresspal,mode);

	//sprintf(string,"Vblank :%d/54770\nnumber %d\nTri %d\nVertex %d",vblank,number,model->nf*number,model->nv*number);

	sprintf(string,"Tri %d\nVertex %d\nvblank: %d\nvram : %x",model->nf*number,model->nv*number,vblank,vram);
	LMP3D_Texture_Setup(texture);
	LMP3D_Draw_Text(8,8,string);

	sprintf(string,"Cycle :%d",(54770-vblank)*91);
	LMP3D_Texture_Setup(texture);
	//LMP3D_Draw_Text(8,200,string);


	LMP3D_VRAM_Info(string);
	LMP3D_Draw_Text(8,200,string);

	sprite[1].rect.x = 32*anim.i;
	LMP3D_Anim_Play(&anim,5,6);

	LMP3D_Draw_Sprite_Array(sprite,2);




	//printf("p: %f\n",model->position.z);
	//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);


	//LMP3D_Camera_Perspective(camera);

	//LMP3D_VBlank();
	//model->position.z = -100;
/*
	if(mode2 == 0)
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	i = 0;
	for(i = 0;i <  number;i++)
	{
		tmpx = (i&0x0F)<<5;
		tmpy = (i&0xF0)<<3;

		model->position.x = tmpx;
		model->position.y = tmpy-100;


		LMP3D_Model_Draw(model);
	}
*/
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	LMP3D_FlipBuffer(buffer);

	vblank = LMP3D_VBlank();
}

void game(LMP3D_Buffer *_buffer)
{
	buffer=_buffer;
	
	LMP3D_TAR tar;
	printf("Begin game\n----------------\n");


	camera = LMP3D_Camera_Init();

	LMP3D_Event_Init(&event);




	vram = LMP3D_VRAM_Get();

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

/*
#ifndef EMSCRIPTEN

	model = LMP3D_Load_Model("zack.bcm",NULL,NULL,0);
	if(!model){
		printf("\nFail loading model\n");
	}
#else
*/
	LMP3D_Tar(&tar,"DATA","zack.bcm",LMP3D_TAR_OFFSET,NULL,0);

	//LMP3D_Tar(&tar,"DATA","Tidus/Tidus.bcm",LMP3D_TAR_OFFSET,NULL,0);

	model = LMP3D_Load_Model("DATA",tar.offset,NULL,0);

	//model = LMP3D_Load_Model("boy.bcm",NULL,NULL,0);
	//model = LMP3D_Load_Model("teapot.bcm",NULL,NULL,0);
	//model = LMP3D_Load_Model("lightning.bcm",NULL,NULL,0);
	//model = LMP3D_Load_Model("zack.bcm",0,NULL,0);
	//model = LMP3D_Load_Model("untitled.bcm",NULL,NULL,0);
//#endif
	
/*
	LMP3D_Model_Free(model);
	model = LMP3D_Load_Model("DATA",tar.offset,NULL,0);*/
	//model = LMP3D_Load_Model("PC/Tidus.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("DC/zack.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("sophia.bcm",0,NULL,0);

	//model = LMP3D_Load_Model("Lightning/lightning.bcm",0,NULL,0);
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

	LMP3D_Tar(&tar,"DATA","font.png",LMP3D_TAR_OFFSET,NULL,0);
	texture = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);

if(!texture){
	printf("\nError loading texture\n");
}else{
	printf("#nTexture sucess\n");
}

	//int t_end,t_begin,fps = 0,total=0;

	number = 1;

	LMP3D_Tar(&tar,"DATA","adell.pcx",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture2 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture2);
	LMP3D_Texture_Free_Pixel(texture2);



	LMP3D_Tar(&tar,"DATA","tile.pcx",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture3 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Upload(texture3);
	LMP3D_Texture_Free_Pixel(texture3);

	LMP3D_Tar(&tar,"DATA","out.ktm",LMP3D_TAR_OFFSET,NULL,0);
	tilemap = LMP3D_Load_TileMap("DATA",tar.offset,NULL,tar.size);
	tilemap->texture = texture3;


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

	//float scale = 32.0f;
	//model->scale = LMP3D_Type_Vector3(scale,scale,scale);
	position = model->position;
	position.y = 0;

	camera.angle.z = position.z;
	camera.speed = 0.04f;

	//printf("%f\n",camera.angle.z );
	//model->rotate.x += -PI/2;



#ifdef EMSCRIPTEN
	// register update as callback
	emscripten_set_main_loop(update, 0, 1);
#else
	while(event.exit == 0)
	{
		update();
	}
#endif

	LMP3D_VRAM_Set(vram);
}


