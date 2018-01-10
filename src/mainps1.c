#include <stdio.h>


#ifdef PLAYSTATION1

#include <psx.h>
#include <psxgpu.h>


#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"


void PS1BufferDraw();

extern short sincostbl[4096];
#define fastsin(v) sincostbl[v&0xFFF];
#define fastcos(v) sincostbl[(v+1024)&0xFFF];

void GsDrawList3()
{

	unsigned int linked_list2[0x100];

	int i = 1;


	//Texture 16 bits
	linked_list2[i++] = GP0_TEXPAGE(0,1,0,2,0,1,0,0,0);
	linked_list2[i++] = GP0_CMD_COLOR(GP0_RECTANGLE_TOB,128,128,128);
	linked_list2[i++] = GP0_VERTEX(150,0);
	linked_list2[i++] = GP0_TEXCOORD(0,0,0);
	linked_list2[i++] = GP0_VERTEX(128,256);


	//8bpp
	linked_list2[i++] = GP0_TEXPAGE(7,0,0,1,0,1,0,0,0);
	linked_list2[i++] = GP0_CMD_COLOR(GP0_RECTANGLE_TOB,128,128,128);
	linked_list2[i++] = GP0_VERTEX(0,0);
	linked_list2[i++] = GP0_TEXCOORD(0,0,0x0001);
	linked_list2[i++] = GP0_VERTEX(128,64);







	linked_list2[0] = (i-1)<<24;

	linked_list2[i] = 0x00ffffff;

gpu_ctrl(4, 2); // DMA CPU->GPU mode
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1a))); /* Wait for the GPU to finish
						 * drawing primitives. */
	while(!(RW_REGISTER_U32(GP1) & (1<<0x1c))); /* Wait for the GPU to be free */


	RW_REGISTER_U32(D2_MADR) = (unsigned int)linked_list2;
	RW_REGISTER_U32(D2_BCR) = 0;
	RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);
	while(GsIsDrawing());

}




void PS1_TRS(IVector3 position,IVector3 rotate,IVector3 scale)
{
	short cosAngle,sinAngle;
	short m1[10];
	short m2[10];
	short r[10];


	position.z +=(1<<9);

	cosAngle = fastcos(rotate.x);
	sinAngle = fastsin(rotate.x);
	m1[0+0] = 1<<12;
	m1[0+1] = 0<<12;
	m1[0+2] = 0<<12;

	m1[3+0] = 0<<12;
	m1[3+1] = cosAngle;
	m1[3+2] = sinAngle;

	m1[6+0] = 0<<12;
	m1[6+1] = -sinAngle;
	m1[6+2] = cosAngle;

	m1[6+3] = 0;


	cosAngle = fastcos(rotate.y);
	sinAngle = fastsin(rotate.y);
	m2[0+0] = cosAngle;
	m2[0+1] = 0<<12;
	m2[0+2] = -sinAngle;

	m2[3+0] = 0<<12;
	m2[3+1] = 1<<12;
	m2[3+2] = 0<<12;

	m2[6+0] = sinAngle;
	m2[6+1] = 0<<12;
	m2[6+2] = cosAngle;

	m2[6+3] = 0;

	PS1_GTE_MulMat(m1,m2,r);

	cosAngle = fastcos(rotate.z);
	sinAngle = fastsin(rotate.z);
	m1[0+0] = cosAngle;
	m1[0+1] = sinAngle;
	m1[0+2] = 0<<12;

	m1[3+0] = -sinAngle;
	m1[3+1] = cosAngle;
	m1[3+2] = 0<<12;

	m1[6+0] = 0<<12;
	m1[6+1] = 0<<12;
	m1[6+2] = 1<<12;

	m1[6+3] = 0;

	PS1_GTE_MulMat(r,m1,r);



	m1[0+0] = scale.x;
	m1[0+1] = 0<<12;
	m1[0+2] = 0<<12;

	m1[3+0] = 0<<12;
	m1[3+1] = scale.y;
	m1[3+2] = 0<<12;

	m1[6+0] = 0<<12;
	m1[6+1] = 0<<12;
	m1[6+2] = scale.z;

	m1[6+3] = 0;

	PS1_GTE_MulMat(r,m1,r);


	PS1_GTE_ModelView_Set(r,&position);
}


void game2(LMP3D_Buffer *buffer)
{

	LMP3D_Texture* texture = LMP3D_Load_Texture("cdrom:FFCCDIF.PNG;1");

	LMP3D_Convert_Pixel(texture,LMP3D_FORMAT_RGB15);
	LMP3D_Texture_Upload(texture);

	LMP3D_Texture_Free_Pixel(texture);

	//--------------------------------
	int nvec = 12,fps = 0;
	int l = 0;
	int i;


	LMP3D_Event event;
	LMP3D_Camera camera;
	camera = LMP3D_Camera_Init();

	int vb,n = 1;

	IVector3 position,scale,rotate;

	position.x = 100;
	position.y = 0;
	position.z = 0;

	rotate.x = 0;
	rotate.y = 0;
	rotate.z = 0;

	scale.x = 1<<12;
	scale.y = 1<<12;
	scale.z = 1<<12;


	int x = 0;


	LMP3D_Model *model = LMP3D_Load_Model_bcm("cdrom:ZACK.BCM;1");

	while(1)
	{

		LMP3D_Event_Update(&event);
		LMP3D_Camera_Perspective(camera);

		if(event.key[Button_Up] == LMP3D_KEY_DOWNW) position.y -= 8;
		if(event.key[Button_Down] == LMP3D_KEY_DOWNW) position.y += 8;
		if(event.key[Button_Right] == LMP3D_KEY_DOWNW) position.x += 8;
		if(event.key[Button_Left] == LMP3D_KEY_DOWNW) position.x -= 8;

		if(event.key[Button_Cross] == LMP3D_KEY_DOWNW) position.z += 16;
		if(event.key[Button_Circle] == LMP3D_KEY_DOWNW) position.z -= 16;

		if(event.key[Button_Triangle] == LMP3D_KEY_DOWNW) scale.x += 16;
		if(event.key[Button_Square] == LMP3D_KEY_DOWNW) scale.x -= 16;


		//if(event.key[Button_Start] == LMP3D_KEY_DOWN) buffer->switchBuffer = !buffer->switchBuffer;


		if(event.key[Button_R1] == LMP3D_KEY_DOWNW) rotate.x += 16;
		if(event.key[Button_L1] == LMP3D_KEY_DOWNW) rotate.x -= 16;

		if(event.key[Button_R2] == LMP3D_KEY_DOWNW) rotate.y += 16;
		if(event.key[Button_L2] == LMP3D_KEY_DOWNW) rotate.z += 16;



		PS1_TRS(position,rotate,scale);

		LMP3D_VBlank();

		LMP3D_Clear();

		//GsDrawList3();

		//for(i = 0;i < n;i++)
		LMP3D_Model_Draw(model);


		//TriArray(vec,model->vt,12);


		//printf("scale %d\n",_sc); //0X10800 max vb

		if(fps >= 60)
		{
			printf("time %d/%d   %d\n",0x10800-0x20-vb,0x10800-0x20 ,n*12); //0X10800 max vb

			fps = 0;
		}
		fps++;


		PS1BufferDraw();

		LMP3D_FlipBuffer(buffer);
		vb = LMP3D_VBlank();
	}
}
#endif


