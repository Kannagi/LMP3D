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
void Draw_Text(float px,float py,char *text);
void InitMatrix(LMP3D_Model *model);
void DC_Init_Sprite(int ctextadr);
void DC_InitB(int ctextadr);
void DC_DrawTriArray2(LMP3D_Model *model);
void DC_DrawSprite(float x,float y);

void Draw_Model(LMP3D_Model *model);

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

	LMP3D_TAR tar;
	LMP3D_Model *model;

	LMP3D_Tar(&tar,NULL,"zack.bcm",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);
	model = LMP3D_Load_Model(NULL,tar.offset,DATA_ROM,tar.size);

	LMP3D_Tar(&tar,NULL,"font.png",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);

	LMP3D_Texture* texture,*texture2;

	texture = LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);

	LMP3D_Texture_Convert(texture,LMP3D_FORMAT_BGRA1555);

	//Upload VRAM
	texture->address = 0x3D0000;
	LMP3D_Texture_Upload_VRAM(texture);
	LMP3D_Texture_Free_Pixel(texture);
	//----------

	LMP3D_Tar(&tar,NULL,"ffccdif_1.png",LMP3D_TAR_OFFSET,DATA_ROM,size_DATA_ROM);

	texture2 = LMP3D_Load_Texture(NULL,tar.offset,DATA_ROM,tar.size);

	LMP3D_Texture_Convert(texture2,LMP3D_FORMAT_BGRA1555);

	//convertTwiddle(texture);

	texture2->address = 0x400000;
	LMP3D_Texture_Upload_VRAM(texture2);
	LMP3D_Texture_Free_Pixel(texture2);

	model->position.z = 400;
	model->position.y = 100;
	model->rotate.x = PI/2;

	char strinfo[100],strfps[50];

	int tmpx,i,tmpy;
	int number = 1;
	int vblank = 0;

	LMP3D_Event event;

	while(1)
	{
		LMP3D_Event_Update(&event);

		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) model->position.z += 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) model->position.z -= 8;

		if(event.key[Button_R1] == LMP3D_KEY_DOWN) number++;
		if(event.key[Button_L1] == LMP3D_KEY_DOWN) number--;

		if(event.key[Button_A] == LMP3D_KEY_DOWN) typetest++;
		typetest &=3;

		model->rotate.y += 0.04f;


		DC_InitB(texture2->address);

		for(i = 0;i <  number;i++)
		{
			tmpx = (i&0x7)<<5;
			tmpy = (i&0xF8)<<4;

			model->position.x = tmpx;
			model->position.y = tmpy+100;

			Draw_Model(model);
		}

		DC_Finish();

		DC_Init_Sprite(texture2->address);

		DC_DrawSprite(16,64);

		DC_Init_Sprite(texture->address);

		Draw_Text(16,16,strinfo);

		DC_Finish();

		sprintf(strinfo,"Vblank :%d\nnumber %d\nTri %d ,VRAM : %x ,type %d\n",vblank,number,model->nf*number,pvr_offset,typetest);



		vblank = LMP3D_VBlank();

		LMP3D_FlipBuffer(NULL);



		LMP3D_VBlank();
	}

	return 0;
}





#endif
