
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST

#include <kos.h>
#include <time.h>
#include <string.h>


#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

//  Z =   X0*Y1 + X1*Y2 + X2*Y0 - X0*Y2 - X1*Y0 - X2*Y1
//X0*Y1

void __attribute__((optimize("-O0"), noinline)) DC_Matrix_Vertex(void *v,void *vbuf,int nv)
{
	int i;

	asm (

	"mov		%3,%0\n	" //LS
	"mov		%2,%3\n	" //MT

	"add		#-16,%3\n	" //EX
	"nop 		\n	" //MT

	"add		#-32,%2\n	" //EX
	"mov		#-16,r0\n	" //LS
	//"nop 		\n	" //MT


	"loopvertex%=:\n	"



	//A-------------

	"fschg\n	" //FE
	"fmov		@%1+,dr0\n	" //LS

	"fschg\n	" //FE
	"fmov		@%1+,fr2\n	" //LS

	"fldi1		fr3\n	" //LS
	"add		#48,%2\n	" //EX

	"fmov		@%1+,fr15\n	" //LS
	"add		#48,%3\n	" //EX

	"fldi1		fr12\n	"//LS
	"ftrv		xmtrx,fv0\n	" //FE

	//B-------------
	"fldi1		fr7\n	" //LS
	"nop 		\n	" //MT

	"fldi1		fr13\n	"//LS
	"nop 		\n	" //MT

	"fmov		@%1+,fr4\n	" //LS
	"nop 		\n	" //MT

	"fdiv		fr3,fr12\n	" //FE
	"nop 		\n	" //MT

	"fmov		@%1+,fr5\n	" //LS
	"nop 		\n	" //MT

	//B-------------
	"fmov		@%1+,fr6\n	" //LS
	"nop 		\n	" //MT

	"ftrv		xmtrx,fv4\n	" //FE
	"nop 		\n	" //MT

	"fmov		@%1+,fr14\n	" //LS
	"nop 		\n	" //MT


	"fdiv		fr7,fr13\n	" //FE
	"nop 		\n	" //MT

	//"nop 		\n	" //MT
	//A-------------
	"fmul		fr12,fr2\n	" //FE
	"fmov		fr15,@-%2\n	" //LS

	"fmul		fr12,fr1\n	" //FE
	"nop 		\n	" //MT

	"fmul		fr12,fr0\n	" //FE
	"fmov		fr2,@-%2\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr0,@-%2\n	" //LS
	"fschg\n	" //FE

	"bt/s		loopvertexend%=\n	"//BR
	"fmul		fr13,fr6\n	" //FE

	//B-------------
	//"fmov		@(r0,%3),fr8\n	" //LS
	"fmul		fr13,fr5\n	" //FE
	"fmov		fr14,@-%3\n	" //LS


	"fmul		fr13,fr4\n	" //FE
	"fmov		fr6,@-%3\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr4,@-%3\n	" //LS
	"bf/s		loopvertex%=\n	"//BR
	"fschg\n	" //FE

	"loopvertexend%=:\n	"

	::"r"(i),"r"(v) , "r"(vbuf),"r"(nv)  : "memory");
}

void __attribute__((optimize("-O0"), noinline)) DC_Matrix_Vertex2(void *v,void *vbuf,int nv)
{
	int i;

	asm (

	"mov		%3,%0\n	" //LS
	"mov		%2,%3\n	" //MT

	"add		#-16,%3\n	" //EX
	"nop 		\n	" //MT

	"add		#-32,%2\n	" //EX
	"mov		#-16,r0\n	" //LS
	//"nop 		\n	" //MT


	"loopvertex%=:\n	"



	//A-------------

	"fschg\n	" //FE
	"fmov		@%1+,dr0\n	" //LS

	"fschg\n	" //FE
	"fmov		@%1+,fr2\n	" //LS

	"fldi1		fr3\n	" //LS
	"add		#48,%2\n	" //EX

	"fmov		@%1+,fr15\n	" //LS
	"add		#48,%3\n	" //EX

	"fldi1		fr12\n	"//LS
	"ftrv		xmtrx,fv0\n	" //FE

	//B-------------
	"fldi1		fr7\n	" //LS
	"nop 		\n	" //MT

	"fldi1		fr13\n	"//LS
	"nop 		\n	" //MT

	"fmov		@%1+,fr4\n	" //LS
	"nop 		\n	" //MT

	"fdiv		fr3,fr12\n	" //FE
	"nop 		\n	" //MT

	"fmov		@%1+,fr5\n	" //LS
	"nop 		\n	" //MT

	//B-------------
	"fmov		@%1+,fr6\n	" //LS
	"nop 		\n	" //MT

	"ftrv		xmtrx,fv4\n	" //FE
	"nop 		\n	" //MT

	"fmov		@%1+,fr14\n	" //LS
	"fmov		@(r0,%2),fr8\n	" //LS


	"fdiv		fr7,fr13\n	" //FE
	"nop 		\n	" //MT

	//"nop 		\n	" //MT
	//A-------------
	"fmul		fr12,fr2\n	" //FE
	"fmov		fr15,@-%2\n	" //LS

	"fmul		fr12,fr1\n	" //FE
	"nop 		\n	" //MT

	"fmul		fr12,fr0\n	" //FE
	"fmov		fr2,@-%2\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr0,@-%2\n	" //LS
	"fschg\n	" //FE

	"bt/s		loopvertexend%=\n	"//BR
	"fmul		fr13,fr6\n	" //FE

	//B-------------
	//"fmov		@(r0,%3),fr8\n	" //LS
	"fmul		fr13,fr5\n	" //FE
	"fmov		fr14,@-%3\n	" //LS


	"fmul		fr13,fr4\n	" //FE
	"fmov		fr6,@-%3\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr4,@-%3\n	" //LS
	"bf/s		loopvertex%=\n	"//BR
	"fschg\n	" //FE

	"loopvertexend%=:\n	"

	::"r"(i),"r"(v) , "r"(vbuf),"r"(nv)  : "memory");
}

void __attribute__((optimize("-O0"), noinline)) DC_Matrix_Vertex3(void *v,void *vbuf,int nv)
{
	int i;

	asm (

	"mov		%3,%0\n	" //LS
	"mov		%2,%3\n	" //MT

	"add		#-16,%3\n	" //EX

	"add		#-32,%2\n	" //EX
	"mov		#-16,r0\n	" //LS


	"loopvertex%=:\n	"



	//A-------------

	"fschg\n	" //FE
	"fmov		@%1+,dr0\n	" //LS

	"fschg\n	" //FE
	"fmov		@%1+,fr2\n	" //LS

	"fldi1		fr3\n	" //LS
	"add		#48,%2\n	" //EX

	"fmov		@%1+,fr15\n	" //LS
	"add		#48,%3\n	" //EX

	"fldi1		fr12\n	"//LS
	"ftrv		xmtrx,fv0\n	" //FE

	//B-------------
	"fldi1		fr7\n	" //LS

	"fldi1		fr13\n	"//LS

	"fmov		@%1+,fr4\n	" //LS

	"fdiv		fr3,fr12\n	" //FE

	"fmov		@%1+,fr5\n	" //LS

	//B-------------
	"fmov		@%1+,fr6\n	" //LS

	"ftrv		xmtrx,fv4\n	" //FE

	"fmov		@%1+,fr14\n	" //LS
	"fmov		@(r0,%2),fr8\n	" //LS

	"fdiv		fr7,fr13\n	" //FE


	//A-------------
	"fmul		fr12,fr2\n	" //FE
	"fmov		fr15,@-%2\n	" //LS

	"fmul		fr12,fr1\n	" //FE

	"fmul		fr12,fr0\n	" //FE
	"fmov		fr2,@-%2\n	" //LS

	"fschg\n	" //FE

	"fmov		dr0,@-%2\n	" //LS
	"fschg\n	" //FE

	"fmul		fr13,fr6\n	" //FE

	//B-------------
	"fmul		fr13,fr5\n	" //FE
	"fmov		fr14,@-%3\n	" //LS


	"fmul		fr13,fr4\n	" //FE
	"fmov		fr6,@-%3\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr4,@-%3\n	" //LS
	"bf/s		loopvertex%=\n	"//BR
	"fschg\n	" //FE

	"loopvertexend%=:\n	"

	::"r"(i),"r"(v) , "r"(vbuf),"r"(nv)  : "memory");
}

void __attribute__((optimize("-O0"), noinline)) DC_Matrix_Vertex4(void *v,void *vbuf,int nv)
{
	int i;

	asm (

	"mov		%3,%0\n	" //LS
	"mov		%2,%3\n	" //MT

	"add		#-16,%3\n	" //EX

	"add		#-32,%2\n	" //EX
	"mov		#-16,r0\n	" //LS


	"loopvertex%=:\n	"



	//A-------------

	"fschg\n	" //FE
	"fmov		@%1+,dr0\n	" //LS

	"fschg\n	" //FE
	"fmov		@%1+,fr2\n	" //LS

	"fldi1		fr3\n	" //LS
	"add		#48,%2\n	" //EX

	"fmov		@%1+,fr15\n	" //LS
	"add		#48,%3\n	" //EX

	"fldi1		fr12\n	"//LS
	"ftrv		xmtrx,fv0\n	" //FE

	//B-------------
	"fldi1		fr7\n	" //LS

	"fldi1		fr13\n	"//LS

	"fmov		@%1+,fr4\n	" //LS

	"fdiv		fr3,fr12\n	" //FE

	"fmov		@%1+,fr5\n	" //LS

	//B-------------
	"fmov		@%1+,fr6\n	" //LS

	"ftrv		xmtrx,fv4\n	" //FE

	"fmov		@%1+,fr14\n	" //LS
	"fmov		@(r0,%2),fr8\n	" //LS

	"fdiv		fr7,fr13\n	" //FE

	//A-------------
	"fmul		fr12,fr2\n	" //FE
	"fmov		fr15,@-%2\n	" //LS

	"fmul		fr12,fr1\n	" //FE

	"fmul		fr12,fr0\n	" //FE
	"fmov		fr2,@-%2\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr0,@-%2\n	" //LS
	"fschg\n	" //FE

	"bt/s		loopvertexend%=\n	"//BR
	"fmul		fr13,fr6\n	" //FE

	//B-------------
	"fmul		fr13,fr5\n	" //FE
	"fmov		fr14,@-%3\n	" //LS


	"fmul		fr13,fr4\n	" //FE
	"fmov		fr6,@-%3\n	" //LS

	"dt			%0\n	" //EX
	"fschg\n	" //FE

	"fmov		dr4,@-%3\n	" //LS
	"bf/s		loopvertex%=\n	"//BR
	"fschg\n	" //FE

	"loopvertexend%=:\n	"

	::"r"(i),"r"(v) , "r"(vbuf),"r"(nv)  : "memory");
}


void __attribute__((optimize("-O0"), noinline)) DC_Index_Draw_NClip(unsigned short *index,unsigned int *buffer,unsigned int *v,int nf)
{
	int i;

	asm (

	"mov.w		@%1+,r0\n	" //LS
	"mov		%4,%0\n	" //MT

	"mov.w		@%1+,r8\n	" //LS
	"mov		%2,%4\n	" //MT

	"mov.w		@%1+,r9\n	" //LS
	"add		#4*5,%4\n	" //EX

	"fldi0		fr3\n	" //LS
	"add		%3,r0\n	" //EX

	"fldi0		fr15\n	" //LS
	"add		%3,r8\n	" //EX

	"add		%3,r9\n	" //EX

	"loopindex%=:\n	"

	//------------ fv0 : x0,x1,x2,0 / fv4 : y1,y2,y0,0 / fv12 : y2,y0,y1,0

	"fmov		@r0+,fr0\n	" //LS
	"fmov		@r0+,fr6\n	" //LS
	"fmov		@r8+,fr1\n	" //LS
	"fmov		@r8+,fr4\n	" //LS
	"fmov		@r9+,fr2\n	" //LS
	"fmov		@r9+,fr5\n	" //LS

	//-----
	"fmov		fr6,fr13\n	" //MT
	"fmov		fr4,fr14\n	" //MT
	"fmov		fr5,fr12\n	" //MT
	"mov		r0,r10\n	" //MT
	//-----
	"fipr		fv0,fv4\n	" //FE
	"mov		r8,r11\n	" //MT

	"mov.w		@%1+,r0\n	" //LS
	"mov		r9,r12\n	" //MT

	"fipr		fv0,fv12\n	" //FE
	"mov.w		@%1+,r8\n	" //LS

	"fsub		fr15,fr7\n	" //FE
	"mov.w		@%1+,r9\n	" //LS

	"fcmp/gt	fr7,fr3\n	" //FE
	"add		%3,r0\n	" //EX

	"bf/s		loopindexend%=\n	"//BR
	"add		%3,r8\n	" //EX

	//---------------------------

	//1------------

	"fldi1		fr14\n	" //LS
	"mov		%4,%2\n	" //FE

	"fschg\n	" //MT
	"fmov		@r10+,dr8\n	" //LS

	"fmov		@r11+,dr10\n	" //LS

	"fmov		@r12+,dr12\n	" //LS
	"fschg\n	" //FE

	"fcmp/gt	fr14,fr8\n	" //FE
	"fmov		fr9,@-%2\n	" //LS

	"bf/s		loopindexend%=\n	"//BR
	"fmov		fr8,@-%2\n	" //LS

	"fcmp/gt	fr14,fr10\n	" //FE
	"fmov		fr6,@-%2\n	" //LS

	"bf/s		loopindexend%=\n	"//BR
	"fmov		fr0,@-%2\n	" //LS

	"fcmp/gt	fr14,fr12\n	" //FE
	"mov.l		%5,@-%2\n	" //LS

	"bf			loopindexend%=\n	"//BR
	"pref 		@%2\n	" //LS

	//2-------------
	"fmov		fr11,@-%4\n	" //LS
	"fmov		fr10,@-%4\n	" //LS
	"fmov		fr4,@-%4\n	" //LS
	"fmov		fr1,@-%4\n	" //LS

	"pref 		@%4\n	" //LS
	"add		#4*5,%2\n	" //EX

	//3-------------
	"fmov		fr13,@-%2\n	" //LS
	"fmov		fr12,@-%2\n	" //LS
	"fmov		fr5,@-%2\n	" //LS
	"fmov		fr2,@-%2\n	" //LS
	"mov.l		%6,@-%2\n	" //LS
	"pref 		@%2\n	" //LS
	"add		#4*4,%4\n	" //EX

	"loopindexend%=:\n	"

	"dt			%0\n	" //MT
	"add		%3,r9\n	" //EX

	"bf/s		loopindex%=\n	"//BR
	"fldi0		fr15\n	" //LS

	::"r"(i),"r"(index) ,"r"(buffer) ,"r"(v) ,"r"(nf) ,"r"(PVR_CMD_VERTEX),"r"(PVR_CMD_VERTEX_EOL): "memory");

}


unsigned int fBufferVertex[0x10];
unsigned int BufferVertex[0x8000];

int typetest = 0;

void DC_DrawModel(AURAE_Model *model)
{
	int nv = model->nv;
	if(model->nv >= 0x2000) nv = 0x2000;
	AURAE_Texture_Setup(model->texture[0]);

	unsigned short *index = model->index;
	unsigned int *d = UNCACHED_P4(0);

	/*
	typetest = 3;

	if(typetest & 1)
	*/

	if(typetest == 0)
		DC_Matrix_Vertex(model->v,BufferVertex,nv);

	if(typetest == 1)
		DC_Matrix_Vertex2(model->v,BufferVertex,nv);

	if(typetest == 2)
		DC_Matrix_Vertex3(model->v,BufferVertex,(nv>>1)+1);

	if(typetest == 3)
		DC_Matrix_Vertex4(model->v,BufferVertex,nv);

	DC_Index_Draw_NClip(index,d,BufferVertex,model->nf);
	//if(typetest & 2)
	//DC_Index_Draw_NClip(index,d,BufferVertex,model->nf);

	//printf("%f %f %f\n",BufferVertex[0],BufferVertex[1],BufferVertex[2]);
}

#endif
