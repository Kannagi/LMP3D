#include <stdio.h>
#include <stdlib.h>



#include "LMP3D/LMP3D.h"
void GsDrawSpriteTest(int posx,int posy);
int memory();
void *LMP3D_FrameBuffer_WIN95();

/*
extern unsigned char *DATA_ROM;
extern unsigned int size_DATA_ROM;
*/





void LMP3D_Draw_Triangles_Fill(int *tri,int ntri,int color);
void LMP3D_Draw_Triangles_Texture(int *tri,int ntri,LMP3D_Texture *texture,int *trivt);

void LMP3D_SelectTexture_WIN95(LMP3D_Texture *texture);
void mainsoft(LMP3D_Buffer *buffer)
{
	LMP3D_TAR tar;

	LMP3D_Event event;

	LMP3D_Event_Init(&event);


	char string[100],strfps[50];
	int vblank = 0;

	int t_end,t_begin,fps = 0,total=0;

	Vector3 p;

	p.x = 0;
	p.y = 0;
	p.z = 0;

	int i,number = 1,tmpx,tmpy;

	LMP3D_Tar(&tar,"DATA","font.png",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Convert(texture,LMP3D_FORMAT_BGR565);

	LMP3D_Tar(&tar,"DATA","ffccdif_1.png",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture2 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Convert(texture2,LMP3D_FORMAT_BGR565);


	LMP3D_SelectTexture_WIN95(texture);

	int tri[2*3];

	tri[0] = 16;
	tri[1] = 42;

	tri[2] = 240;
	tri[3] = 220;

	tri[4] = 16;
	tri[5] = 220;


	/**/
	tri[0] = 100;
	tri[1] = 100;

	tri[2] = 132;
	tri[3] = 100;

	tri[4] = 100;
	tri[5] = 132;

	int vt[2*3];

	int chg = 0;


	int ytmp = 100;
	int xtmp = 132;

    while(event.exit == 0)
    {
		LMP3D_Event_Update(&event);

		LMP3D_Clear();


		if(event.key[Button_Cross] == LMP3D_KEY_DOWN) number += 20;
		if(event.key[Button_Circle] == LMP3D_KEY_DOWN) number -= 20;
/*
		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) number +=5;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) number -=5;
*/
		if(event.key[Button_Left] == LMP3D_KEY_DOWNW) xtmp -=2;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) xtmp +=2;

		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) ytmp -=2;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) ytmp +=2;

		tri[2] = xtmp;
		tri[3] = ytmp;


		if(event.key['a'] == LMP3D_KEY_DOWN) chg =!chg;
		//if(event.key[Button_Start] == LMP3D_KEY_DOWN) model->test = 0;

		//GsDrawSpriteTest(0,0);

		if(chg == 0)
		{
			for(i = 0;i < number;i++)
				LMP3D_Draw_Triangles_Fill(tri,12,0);
		}else
		{
			for(i = 0;i < number;i++)
				LMP3D_Draw_Triangles_Texture(tri,12,texture2,vt);
		}


		LMP3D_Camera_Ortho2D();

		//LMP3D_Texture_Setup(texture);

		sprintf(string,"ms :%.2d\nnum : %.3d",vblank,number*12);
		bitmap_font2(string,8,8);

		//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);

		LMP3D_FlipBuffer(buffer);
		vblank = LMP3D_VBlank();

	}


}
