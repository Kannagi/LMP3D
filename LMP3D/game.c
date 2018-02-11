#include <stdio.h>
#include <stdlib.h>

#ifndef PLAYSTATION1
#include <time.h>
#endif

#include "LMP3D/LMP3D.h"
void GsDrawSpriteTest(int posx,int posy);
int memory();
void game(LMP3D_Buffer *buffer)
{
	LMP3D_TAR tar;

	LMP3D_Event event;
	LMP3D_Camera camera;
	camera = LMP3D_Camera_Init();


	LMP3D_Event_Init(&event);
	LMP3D_Model *model;

	LMP3D_Tar(&tar,"DATA","zack.bcm",LMP3D_TAR_OFFSET);
	model = LMP3D_Load_Model("DATA",tar.offset,NULL);

	LMP3D_Tar(&tar,"DATA","font.png",LMP3D_TAR_OFFSET);
	LMP3D_Texture *texture = LMP3D_Load_Texture("DATA",tar.offset,NULL);
	LMP3D_Texture_Upload(texture);
	LMP3D_Texture_Free_Pixel(texture);

	char string[100],strfps[50];
	int vblank = 0;

	int t_end,t_begin,fps = 0,total=0;

	Vector3 p;

	p.x = 0;
	p.y = 0;
	p.z = 0;

	int i,number = 1,tmpx,tmpy;

    while(event.exit == 0)
    {
    	model->rotate.y += 0.01;
		LMP3D_Event_Update(&event);

		//camera
		LMP3D_Camera_Perspective(camera);

		LMP3D_Clear();

/*
		if(event.key[Button_Cross] == LMP3D_KEY_DOWN) addv += 2;
		if(event.key[Button_Circle] == LMP3D_KEY_DOWN) addv -= 2;
*/
		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) p.z += 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) p.z -= 8;


		if(event.key[Button_R1] == LMP3D_KEY_DOWN) number++;
		if(event.key[Button_L1] == LMP3D_KEY_DOWN) number--;


		if(event.key[Button_R2] == LMP3D_KEY_DOWN) number +=10;
		if(event.key[Button_L2] == LMP3D_KEY_DOWN) number -=10;

		if(event.key[Button_Start] == LMP3D_KEY_DOWN) model->test = 0;

		LMP3D_Texture_Setup(model->texture[0]);
		//GsDrawSpriteTest(0,0);

		for(i = 0;i <  number;i++)
		{

			tmpx = (i%20)*30;
			//tmpy = (i/30)*140;
			tmpy = 0;
			model->position.x = tmpx-0;
			model->position.y = 100-tmpy;
			model->position.z = 400+p.z;

			model->scale.x = 1;

			LMP3D_Model_Draw(model);
		}


		LMP3D_Camera_Ortho2D();

		LMP3D_Texture_Setup(texture);

		sprintf(string,"Time %s\nVblank :%d\nnumber %d\nTri %d\nperf %d\n",strfps,vblank,number,model->nf*number,model->test);
		bitmap_font2(string,8,8);

		//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);


#ifndef PLAYSTATION1

		t_end = clock();
		if(fps >= 60)
		{
			total += t_end-t_begin;
			sprintf(strfps,"%d/%d",total,CLOCKS_PER_SEC/60);
			total = 0;
			fps = 0;
		}
		fps++;


		LMP3D_FlipBuffer(buffer);
		vblank = LMP3D_VBlank();
		t_begin = clock();
#endif

	}


}
