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



void intersect_segment(float A1x,float A1y,float A2x,float A2y,float B1x,float B1y,float B2x,float B2y,float *itx,float *ity);
void LMP3D_Draw_Triangles_Fill(int *tri,int ntri,int color);
void LMP3D_Draw_Triangles_Texture(int *tri,int ntri,LMP3D_Texture *texture,int *trivt);

void LMP3D_Draw_Line(int *tri,int color);
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
/*
	LMP3D_Tar(&tar,"DATA","font.png",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Convert(texture,LMP3D_FORMAT_BGR565);

	LMP3D_Tar(&tar,"DATA","ffccdif_1.png",LMP3D_TAR_OFFSET,NULL,0);
	LMP3D_Texture *texture2 = LMP3D_Load_Texture("DATA",tar.offset,NULL,tar.size);
	LMP3D_Texture_Convert(texture2,LMP3D_FORMAT_BGR565);


	LMP3D_SelectTexture_WIN95(texture);
*/
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
	tri[3] = 120;

	tri[4] = 100;
	tri[5] = 132;

	int vt[2*3];

	int chg = 0;


	int ytmp = 120;
	int xtmp = 100;


	int line[4];
	line[0] = 80;
	line[1] = 100;

	line[2] = 170;
	line[3] = 150;

	int line2[4];

	line2[0] = 0;
	line2[1] = 0;

	line2[2] = 10;
	line2[3] = 10;

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


		if(event.key['k'] == LMP3D_KEY_DOWNW) tri[0] -=2;
		if(event.key['m'] == LMP3D_KEY_DOWNW) tri[0] +=2;

		if(event.key['o'] == LMP3D_KEY_DOWNW) tri[1] -=2;
		if(event.key['l'] == LMP3D_KEY_DOWNW) tri[1] +=2;

		tri[2] = xtmp;
		tri[3] = ytmp;


		if(event.key['a'] == LMP3D_KEY_DOWN) chg =!chg;
		//if(event.key[Button_Start] == LMP3D_KEY_DOWN) model->test = 0;


		LMP3D_Draw_Triangles_Fill(tri,12,0);

		//GsDrawSpriteTest(0,0);
/*
		if(chg == 0)
		{
			for(i = 0;i < number;i++)
				LMP3D_Draw_Triangles_Fill(tri,12,0);
		}else
		{
			for(i = 0;i < number;i++)
				LMP3D_Draw_Triangles_Texture(tri,12,texture2,vt);
		}
*/



/*
		float fx=0,fy=0;
		intersect_segment(line[0],line[1],line[2],line[3],tri[0],tri[1],tri[2],tri[3],&fx,&fy);

		LMP3D_Draw_Line(line,0xFF);
		LMP3D_Draw_Line(tri,0);

		//LMP3D_Draw_Line(line2,0xFF00FF00);




		unsigned int *pixels = LMP3D_FrameBuffer_WIN95();
		int tx = fx;
		int ty = fy;

		//printf("%d %d\n",tx,ty);

		i = tx + (ty<<8);
		if(i < 0) i = 0;
		if(i > 0xEFFE) i = 0;

		//pixels[i>>1]  = 0xFF00FF00;


		float px,py,p,c,d,pz,z;
*/
		/*
		tri
		n try

		quad
		nquad

		int quatree 16

		struct:
		x,y,z
		seg



		*/

		/*
		px = line[2]-line[0];
		py = line[3]-line[1];
		pz = 5-2;
		p = px/py;
		z = pz/py;

		d = ytmp-line[1];
		c = p*d;

		tx = c+line[0];

		ty = ytmp;

		float tz = (float)(d*z)+2;
		//printf("tz %f %f\n",tz,p);




		if(ytmp >= line[1] && ytmp <= line[3])
		{
			i = tx + (ty<<8);
			if(i < 0) i = 0;
			if(i > 0xEFFE) i = 0;

			pixels[i>>1]  = 0xF800F800;


			line2[0] = tx;
			line2[1] = ty;

			p = py/px;

			line2[2] = line2[0] - (p*py);
			line2[3] = line2[1] + (p*px);



			float a,b,e;
			a = line2[2]-line2[0];
			b = line2[3]-line2[1];

			c = sqrtf(a*a + b*b);

			a = line2[0]-line[0];
			b = line2[1]-line[1];

			d = sqrtf(a*a + b*b);

			a = line2[2]-line[0];
			b = line2[3]-line[1];

			e = sqrtf(a*a + b*b);




			//printf("%f %f %f , %f = %f\n",c,d,e,e*e,c*c + d*d);
		}



		tx = xtmp;
		ty = ytmp;
		i = tx + (ty<<8);
		if(i < 0) i = 0;
		if(i > 0xEFFE) i = 0;

		pixels[i>>1]  = 0xFFFFFFFF;
*/
		LMP3D_Camera_Ortho2D();

		//LMP3D_Texture_Setup(texture);

		//sprintf(string,"ms :%.2d\nnum : %.3d",vblank,number*12);
		//bitmap_font2(string,8,8);

		//printf("buf %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);

		LMP3D_FlipBuffer(buffer);
		vblank = LMP3D_VBlank();

	}


}

void intersect_segment(float A1x,float A1y,float A2x,float A2y,float B1x,float B1y,float B2x,float B2y,float *itx,float *ity)
{
	float tmp;
/*
	if(B2y < B1y)
	{
		tmp = B1x;
		B1x = B2x;
		B2x = tmp;

		tmp = B1y;
		B1y = B2y;
		B2y = tmp;
	}
*/

	float dif1x = A2x-A1x;
	float dif2x = B2x-B1x;

	float dif1y = A2y-A1y;
	float dif2y = (B2y-B1y);

	float A1v = dif1x/dif1y;
	float B1v = dif2x/dif2y;

	float B2v = B1x-(B1v*B1y);
	float A2v = A1x-(A1v*A1y);

	//-------------------------------

	Vector2 AO,AP;
    AP.x = B1x - A1x;
    AP.y = B1y - A1y;
    AO.x = B2x - A1x;
    AO.y = B2y - A1y;

	if ((dif1x*AP.y - dif1y*AP.x)*(dif1x*AO.y - dif1y*AO.x) > 0) return;

	//AP.x = -AP.x;
    //AP.y = -AP.y;
    AO.x = A2x - B1x;
    AO.y = A2y - B1y;

	if ((dif2x*-AP.y - dif2y*-AP.x)*(dif2x*AO.y - dif2y*AO.x) > 0) return;

	//-----------------

	float Sy = (B2v-A2v)/(A1v-B1v);
	float Sx = (B1v*Sy)+B2v;


	*itx = Sx;
	*ity = Sy;
/*
	float coef;

	coef = B1v*(AP.y);

	B1x = coef+B1x;

	B1v = (B2x-B1x)/(ao);

	float intersect = (B1x-A1x)/(A1v-B1v);



	*itx = A1x+(A1v*intersect);
	*ity = A1y+(intersect);*/
}

