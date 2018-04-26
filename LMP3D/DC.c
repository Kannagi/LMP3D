/*
Test Dreamcast pvr lib




*/
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST

#include <kos.h>
#include <time.h>
#include <string.h>


#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

void DC_DrawSprite(float x,float y)
{
	void *currentBuffer = UNCACHED_P4(0);

	*((u32*)currentBuffer+0) = 0XF0000000;

	*((float*)currentBuffer+1) = x;
	*((float*)currentBuffer+2) = y;
	*((float*)currentBuffer+3) = 10000;

	*((float*)currentBuffer+4) = x+256;
	*((float*)currentBuffer+5) = y;
	*((float*)currentBuffer+6) = 10000;

	*((float*)currentBuffer+7) = x+256;
	*((float*)currentBuffer+8) = y+256;
	*((float*)currentBuffer+9) = 10000;

	*((float*)currentBuffer+10) = x;
	*((float*)currentBuffer+11) = y+256;

	float u = 0;
	float v = 0;
	u32 ufl2 = *((unsigned int*)&v);
	u32 ufl = *((unsigned int*)&u);
	ufl = (ufl2>>16) + (ufl&0xFFFF0000);
	*((u32*)currentBuffer+13) = ufl;


	u = 1;
	v = 0;
	ufl2 = *((unsigned int*)&v);
	ufl = *((unsigned int*)&u);
	ufl = (ufl2>>16) + (ufl&0xFFFF0000);
	*((u32*)currentBuffer+14) = ufl;

	u = 1;
	v = 1;
	ufl2 = *((unsigned int*)&v);
	ufl = *((unsigned int*)&u);
	ufl = (ufl2>>16) + (ufl&0xFFFF0000);
	*((u32*)currentBuffer+15) = ufl;

	asm("pref @%0" : : "r"(currentBuffer));
	asm("pref @%0" : : "r"(currentBuffer+32));

	//printf("%x %x %x %x\n	",*((u32*)currentBuffer+0),*((u32*)currentBuffer+1),*((u32*)currentBuffer+2),*((u32*)currentBuffer+3));
	//printf("%x %x %x %x\n	",*((u32*)currentBuffer+4),*((u32*)currentBuffer+5),*((u32*)currentBuffer+6),*((u32*)currentBuffer+7));

}




inline void DC_Matrix_Vertex(void *v,void *vbuf,int nv)
{
	int i=0;

	asm (

	"mov		%2,r0\n	" //MT
	"add		#16,%2\n	" //EX
	"fmov		@%1+,fr0\n	" //LS
	"fmov		@%1+,fr1\n	" //LS
	"fmov		@%1+,fr2\n	" //LS

	"loopvertex%=:\n	"

	"fldi1		fr3\n	" //LS
	"ftrv		xmtrx,fv0\n	" //FE

	"fmov		@%1+,fr15\n	" //LS

	//B-------------
	"fmov		@%1+,fr4\n	" //LS
	"fmov		@%1+,fr5\n	" //LS
	"fmov		@%1+,fr6\n	" //LS

	//A-------------
	"fldi1		fr12\n	"//LS
	"fdiv		fr3,fr12\n	" //FE
	//B-------------

	"fmov		@%1+,fr14\n	" //LS
	"fldi1		fr7\n	" //LS
	"ftrv		xmtrx,fv4\n	" //FE

	"fldi1		fr13\n	"//LS
	"fdiv		fr7,fr13\n	" //FE
	//A-------------
	"fmov		fr15,@-%2\n	" //LS
	"fmul		fr12,fr2\n	" //FE

	"fmul		fr12,fr1\n	" //FE

	"fmul		fr12,fr0\n	" //FE
	"fmov		fr2,@-%2\n	" //LS

	"fmov		fr1,@-%2\n	" //LS
	"add		#1,%0\n	" //EX

	"fmov		fr0,@-%2\n	" //LS
	"cmp/eq		%0,%3\n	" //MT

	"bt/s		loopvertexend%=\n	"//BR
	"add		#32,%2\n	" //EX

	//B-------------
	"fmov		fr14,@-%2\n	" //LS
	"fmul		fr13,fr6\n	" //FE

	"fmul		fr13,fr5\n	" //FE
	"fmov		@%1+,fr0\n	" //LS

	"fmul		fr13,fr4\n	" //FE
	"fmov		fr6,@-%2\n	" //LS

	"fmov		fr5,@-%2\n	" //LS
	"add		#1,%0\n	" //EX

	"fmov		fr4,@-%2\n	" //LS
	"add		#32,%2\n	" //EX

	"fmov		@%1+,fr1\n	" //LS
	"cmp/eq		%0,%3\n	" //MT

	"bf/s		loopvertex%=\n	"//BR
	"fmov		@%1+,fr2\n	" //LS

	"loopvertexend%=:\n	"
	"mov		r0,%2\n	" //MT

	::"r"(i),"r"(v) , "r"(vbuf),"r"(nv)  : "memory");
}

unsigned int BufferVertex[0x4000];

int typetest = 0;


inline void DC_Index_Draw(unsigned short *index,unsigned int *buffer,unsigned int *v,int nf)
{
	int i;

	asm (

	"mov		#0,%0\n	" //LS
	"loopindex%=:\n	"

	//-------------
	"mov.w		@%1+,r0\n	" //LS
	"add		#4*5,%2\n	" //EX

	"add		%3,r0\n	" //EX

	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%5,@-%2\n	" //LS

	//-------------
	"mov.w		@%1+,r0\n	" //LS

	"add		%3,r0\n	" //EX
	"pref 		@%2\n	" //LS

	"add		#4*5,%2\n	" //EX
	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%5,@-%2\n	" //LS

	//-------------
	"mov.w		@%1+,r0\n	" //LS

	"add		%3,r0\n	" //EX
	"pref 		@%2\n	" //LS

	"add		#4*5,%2\n	" //EX
	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"add		#1,%0\n	" //EX
	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%6,@-%2\n	" //LS

	"cmp/eq		%0,%4\n	" //MT
	"bf/s		loopindex%=\n	"//BR
	"pref 		@%2\n	" //LS

	::"r"(i),"r"(index) ,"r"(buffer) ,"r"(v) ,"r"(nf) ,"r"(PVR_CMD_VERTEX),"r"(PVR_CMD_VERTEX_EOL): "memory");

}


void __attribute__((optimize("-O0"), noinline)) DC_Index_Buffer(unsigned short *index,unsigned int *buffer,unsigned int *v,int nf)
{
	int i;

	asm (

	"mov		#0,%0\n	" //LS
	"loopindex%=:\n	"

	//-------------
	"mov.w		@%1+,r0\n	" //LS
	"add		#4*5,%2\n	" //EX

	"add		%3,r0\n	" //EX

	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%5,@-%2\n	" //LS
	"add		#4*8,%2\n	" //EX

	//-------------
	"mov.w		@%1+,r0\n	" //LS
	"add		%3,r0\n	" //EX

	"add		#4*5,%2\n	" //EX
	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%5,@-%2\n	" //LS
	"add		#4*8,%2\n	" //EX

	//-------------
	"mov.w		@%1+,r0\n	" //LS
	"add		%3,r0\n	" //EX

	"add		#4*5,%2\n	" //EX
	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr1\n	" //LS
	"fmov		@r0+,fr2\n	" //LS
	"fmov		@r0+,fr3\n	" //LS

	"add		#1,%0\n	" //EX
	"fmov		fr3,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"fmov		fr1,@-%2\n	" //LS
	"fmov		fr0,@-%2\n	" //LS
	"mov.l		%6,@-%2\n	" //LS

	"cmp/eq		%0,%4\n	" //MT
	"bf/s		loopindex%=\n	"//BR
	"add		#4*8,%2\n	" //EX
	"nop		\n	" //LS

	::"r"(i),"r"(index) ,"r"(buffer) ,"r"(v) ,"r"(nf) ,"r"(PVR_CMD_VERTEX),"r"(PVR_CMD_VERTEX_EOL): "memory");
}

//float BufferDMA[0x20000] __attribute__((aligned(32)));
void DC_DrawTriArray2(LMP3D_Model *model)
{



	DC_Matrix_Vertex(model->v,BufferVertex,model->nv);

	unsigned short *index = model->index;
	unsigned int *d = UNCACHED_P4(0);

	DC_Index_Draw(index,d,BufferVertex,model->nf);

/*
	unsigned int *buffer = UNCACHED_P2(BufferDMA);
	int nf = (model->nf>>1)*3;

	DC_Index_Buffer(index,buffer,BufferVertex,model->nf>>1);
	while(!pvr_dma_ready());
	pvr_dma_load_ta(BufferDMA,nf<<5,0,NULL,0);

	DC_Index_Buffer(index+nf,buffer+0x10000,BufferVertex,model->nf>>1);
	while(!pvr_dma_ready());
	pvr_dma_load_ta(BufferDMA+0x10000,nf<<5,0,NULL,0);*/
}

int testd1 = 0,testd2 = 0;
int test1 = 1;
int test2 = 5;

void DC_InitB(int ctextadr)
{
	u32 *tpoly = UNCACHED_P4(0);

	tpoly[0] = TA_PARA_POLYGON | TA_GROUP_ENABLE | TA_GROUP_USERTILECLIP_DISABLE | TA_GROUP_STRIPLEN1 | TA_OBJ_16BITUV |TA_OBJ_TEXTURE | TA_PARA_OPOLY;
	tpoly[1] = TA_ISP_DEPTH_GREATER | TA_ISP_CULLING_POSITIVE;
	tpoly[2] = TA_TSP_SRCALPHA(1) | TA_TSP_DSTALPHA(0) | TA_TSP_FOG_DISABLE | TA_TSP_USIZE(5) | TA_TSP_VSIZE(5) | TA_TSP_TEXTURE_DECAL;
	tpoly[3] = TEXTURE_CONTROL_WORD_RGB(0,0,TA_TEXTURE_RGB1555,1,0,ctextadr>>3);

	tpoly[4] = 0x0;
	tpoly[5] = 0x0;
	tpoly[6] = 0x0;
	tpoly[7] = 0xFFFFFFFF;

	asm("pref @%0" : : "r" (tpoly));
}

void DC_Init_Sprite(int ctextadr)
{
	u32 *tpoly = UNCACHED_P4(0);

	tpoly[0] = TA_PARA_SPRITE | TA_GROUP_ENABLE | TA_GROUP_USERTILECLIP_DISABLE | TA_OBJ_16BITUV | TA_OBJ_TEXTURE | TA_PARA_TPOLY;
	tpoly[1] = TA_ISP_ZWRITE_DISABLE;
	tpoly[2] = TA_TSP_SRCALPHA(test1) | TA_TSP_DSTALPHA(test2) | TA_TSP_FOG_DISABLE | TA_TSP_USIZE(5) | TA_TSP_VSIZE(5) | TA_TSP_TEXTURE_DECAL;
	tpoly[3] = TEXTURE_CONTROL_WORD_RGB(0,0,TA_TEXTURE_RGB1555,1,0,ctextadr>>3);

	tpoly[4] = 0x0;
	tpoly[5] = 0x0;
	tpoly[6] = 0x0;
	tpoly[7] = 0xFFFFFFFF;

	asm("pref @%0" : : "r" (tpoly));
}

void InitMatrix(LMP3D_Model *model)
{
	float rotateMatrix[16],rotateMatrix2[16];
	float projectionMatrix[16],cameraMatrix[16];

	int rotate = 1;

	if(model->rotate.y != 0)
	{
		LMP3D_MatrixRotateY(rotateMatrix, model->rotate.y);

		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}

		if(model->rotate.z != 0)
		{
			LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}
	}else
	{
		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix, model->rotate.x);

			if(model->rotate.z != 0)
			{
				LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
				LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
			}
		}else
		{
			if(model->rotate.z != 0)
				LMP3D_MatrixRotateZ(rotateMatrix, model->rotate.z);
			else rotate = 0;
		}
	}

	//T+S
	LMP3D_MatrixTS(cameraMatrix, model->position.x, model->position.y, model->position.z, model->scale.x, model->scale.y, model->scale.z);

	if(rotate == 1)
	{
		//T+S+R
		LMP3D_MatrixMultiply(cameraMatrix,rotateMatrix, cameraMatrix);
	}

	//final
	LMP3D_MatrixProjection(projectionMatrix);

	LMP3D_MatrixMultiply(projectionMatrix,cameraMatrix, projectionMatrix);

	DC_Matrix_Load(projectionMatrix);
}


void Draw_Model(LMP3D_Model *model)
{
	InitMatrix(model);
	DC_DrawTriArray2(model);
}

/*
void convertTwiddle(LMP3D_Texture* texture)
{
	unsigned short *pixel = malloc(texture->size);
	unsigned short *tpixel = texture->pixel;

	int x,y,i = 0;


	for(y = 0;y < texture->h;y++)
	{
		for(x = 0;x < texture->w;x++)
		{
			pixel[i+x] = tpixel[i+x];
		}

		i += x;
	}


	texture->pixel = pixel;



}*/

void Draw_Text(float px,float py,char *text)
{
	void *currentBuffer = UNCACHED_P4(0);

	int i = 0;
	u32 ufl,ufl2;

	float y,x;
	float tx,ty;
	char c;

	x = px;
	y = py;

	while(1)
	{
		c = text[i++];
		if(c == '\n')
		{
			y += 16.0f;
			x = px;
			continue;
		}

		if(c == 0) break;

		tx = c&0x0F;
		ty = (c&0xF0)>>4;

		*((u32*)currentBuffer+0) = 0XF0000000;

		*((float*)currentBuffer+1) = x;
		*((float*)currentBuffer+2) = y;
		*((float*)currentBuffer+3) = 10000;

		*((float*)currentBuffer+4) = x+16.0f;
		*((float*)currentBuffer+5) = y;
		*((float*)currentBuffer+6) = 10000;

		*((float*)currentBuffer+7) = x+16.0f;

		asm("pref @%0" : : "r"(currentBuffer));

		*((float*)currentBuffer+0) = y+16.0f;
		*((float*)currentBuffer+1) = 10000;

		*((float*)currentBuffer+2) = x;
		*((float*)currentBuffer+3) = y+16.0f;

		float u,v;
		u = (1.0f/16.0f)*tx;
		v = (1.0f/16.0f)*ty;
		ufl2 = *((unsigned int*)&v);
		ufl = *((unsigned int*)&u);
		ufl = (ufl2>>16) + (ufl&0xFFFF0000);
		*((u32*)currentBuffer+5) = ufl;

		u = (1.0f/16.0f)*(tx+1);
		v = (1.0f/16.0f)*ty;
		ufl2 = *((unsigned int*)&v);
		ufl = *((unsigned int*)&u);
		ufl = (ufl2>>16) + (ufl&0xFFFF0000);
		*((u32*)currentBuffer+6) = ufl;

		u = (1.0f/16.0f)*(tx+1);
		v = (1.0f/16.0f)*(ty+1);
		ufl2 = *((unsigned int*)&v);
		ufl = *((unsigned int*)&u);
		ufl = (ufl2>>16) + (ufl&0xFFFF0000);
		*((u32*)currentBuffer+7) = ufl;

		x += 16.0f;

		asm("pref @%0" : : "r"(currentBuffer));
	}
}





#endif
