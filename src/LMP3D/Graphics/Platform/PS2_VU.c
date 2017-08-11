#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include <kernel.h>
#include "LMP3D/LMP3D.h"
#include <tamtypes.h>

#include "LMP3D/Platform/PS2_Register.h"
#include "PS2_vu1Triangle.c"


int pdkVu1Size(u32* start, u32* end)
{
  int size = (end-start)/2;

	// if size is odd we have make it even in order for the transfer to work
	// (quadwords, because of that its VERY important to have an extra nop nop
	// at the end of each vuprogram

	if( size&1 )
		size++;

	return size;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pdkVu1UploadProg(int Dest, void* start, void* end)
{
  int   count = 0;
  u8 tempDma[1024] ALIGNED(16);
  void* chain = (u64*)&tempDma; // uncached

  // get the size of the code as we can only send 256 instructions in each MPGtag

  count = pdkVu1Size( start, end );

  while( count > 0 )
  {
    u32 current_count = count > 256 ? 256 : count;

    *((u64*) chain)++ = DMA_REF_TAG( (u32)start, current_count/2 );
    *((u32*) chain)++ = VIF_CODE( VIF_NOP,0,0 );
    *((u32*) chain)++ = VIF_CODE( VIF_MPG,current_count&0xff,Dest );

    start += current_count*2;
    count -= current_count;
    Dest += current_count;
  }

  *((u64*) chain)++ = DMA_END_TAG( 0 );
  *((u32*) chain)++ = VIF_CODE(VIF_NOP,0,0);
  *((u32*) chain)++ = VIF_CODE(VIF_NOP,0,0);

  // Send it to vif1
  FlushCache(0);


	RW_REGISTER_U32(D1_MADR) = 0;
	RW_REGISTER_U32(D1_TADR) = (u32)tempDma;
	RW_REGISTER_U32(D1_QWC ) = 0;
	RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);
}

void PS2_VU_Init()
{

	void *end = vu1Triangle + size_vu1Triangle;
	pdkVu1UploadProg(0,&vu1Triangle,end);


	#define VIF1_ITOPS		0x10003CC0
#define VIF1_ITOP		0x10003CD0
	RW_REGISTER_U32(VIF1_ITOPS) = 511; //GIF
	RW_REGISTER_U32(VIF1_ITOP)  = 0;   //VU1
}

void drawvu1(float* matrix,LMP3D_Model *model)
{
	void *data = model->v;
	void *currentBuffer,*kickBuffer;
	int i,n1,l,n2,result,tn1,tn2,tn,n3,n4,tn3,tn4;
	u32 adress = 0;
	int switchBuffer = 0;
	char dmaBuffer [2][17*1024] __attribute__((aligned(16)));

	currentBuffer = (void *)VU1_Mem;

	*((float*)currentBuffer)++ = matrix[(0<<2)+0];
	*((float*)currentBuffer)++ = matrix[(0<<2)+1];
	*((float*)currentBuffer)++ = matrix[(0<<2)+2];
	*((float*)currentBuffer)++ = matrix[(0<<2)+3];

	*((float*)currentBuffer)++ = matrix[(1<<2)+0];
	*((float*)currentBuffer)++ = matrix[(1<<2)+1];
	*((float*)currentBuffer)++ = matrix[(1<<2)+2];
	*((float*)currentBuffer)++ = matrix[(1<<2)+3];

	*((float*)currentBuffer)++ = matrix[(2<<2)+0];
	*((float*)currentBuffer)++ = matrix[(2<<2)+1];
	*((float*)currentBuffer)++ = matrix[(2<<2)+2];
	*((float*)currentBuffer)++ = matrix[(2<<2)+3];

	*((float*)currentBuffer)++ = matrix[(3<<2)+0];
	*((float*)currentBuffer)++ = matrix[(3<<2)+1];
	*((float*)currentBuffer)++ = matrix[(3<<2)+2];
	*((float*)currentBuffer)++ = matrix[(3<<2)+3];

	*((u64*)currentBuffer)++ = 0;
	*((u64*)currentBuffer)++ = 0;

	u64 primv = GS_SET_PRIM(GS_PRIM_TRIANGLE,GS_IIP_FLAT,GS_TME_TEXTURE_ON, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_STQ,
							GS_CTXT_0, GS_FIX_0);
	*((u64*)currentBuffer)++ = GS_SET_GIFTAG(3, 1, 1, primv,
											   0, 3);
	*((u64*)currentBuffer)++ = 0x512;

	u32 out[12];
	u32 matrix[12];
	u32 in[12];


	result = model->nf;
	while(result > 0)
	{

		currentBuffer = (char*)&dmaBuffer[switchBuffer];

		kickBuffer = currentBuffer;



		switchBuffer = !switchBuffer;


		result -= 84;

		if(result >= 0)
		{

			tn1 = 252;
			tn2 = 252;


			tn = 252;
		}else
		{
			//
			if(result > -42)
			{
				tn1 = 252;
				tn2 = ( (42+result)*6);


			}else
			{
				tn1 = ( (84+result)*6);
				tn2 = 0;
			}


			tn = (tn1+tn2)>>1;

		}

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,1,4);

		*((u64*)currentBuffer)++ = 0;
		*((u32*)currentBuffer)++ = 0;
		*((u32*)currentBuffer)++ =tn;

		if(tn == 252)
		{

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress , tn>>1);
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_STCYL,0,0x0101); //CL = 1 WL = 1
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_UNPACK_V4_16,tn1,6);


			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,0);


			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(2016) , tn>>1);
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_STCYL,0,0x0101); //CL = 1 WL = 1
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_UNPACK_V4_16,tn1,6+(tn1) );

			adress += 4032;

		}else
		{
			/*
			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);*/
/*

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress , tn1>>1);
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_STCYL,0,0x0101); //CL = 1 WL = 1
			*((u32*)currentBuffer)++ = VIF_CODE( VIF_UNPACK_V4_16,tn1,6);


			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,0);

			if(tn2)
			{
				*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(2016) , tn2>>1);
				*((u32*)currentBuffer)++ = VIF_CODE( VIF_STCYL,0,0x0101); //CL = 1 WL = 1
				*((u32*)currentBuffer)++ = VIF_CODE( VIF_UNPACK_V4_16,tn2,6+(tn1) );
			}*/



		}



		*((u64*)currentBuffer)++ = DMA_END_TAG(1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);

		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);




		int i = 0;
		//for(i = 0;i < 30;i++) RW_REGISTER_U32(D1_CHCR2)) & 0x100;
		while( (RW_REGISTER_U32(D1_CHCR)) &0x100)
		{
			asm __volatile__ (
			"lqc2		vf1, 0x00(%0)	\n"
			"lqc2		vf2, 0x10(%0)	\n"
			"lqc2		vf3, 0x20(%0)	\n"
			"lqc2		vf4, 0x30(%0)	\n"

			"lqc2		vf6, 0x20(%2)	\n"
			"vmulax		ACC,vf1, vf6	\n"
			"vmadday	ACC,vf2, vf6	\n"
			"vmaddaz	ACC,vf3, vf6	\n"
			"vmaddw		vf5,vf4, vf6	\n"


			"vdiv		Q, vf0w, vf5w	\n"
			"vwaitq						\n"
			"vmulq.xyz	vf6, vf5, Q		\n"
			"vftoi4		vf6, vf6		\n"


			"sqc2		vf6, 0x20(%1)	\n"
			"sqc2		vf6, 0x00(%1)	\n"
			"sqc2		vf6, 0x10(%1)	\n"
			: : "r" (matrix), "r" (out), "r" (in): "memory");
			i++;
		}
		if(i > model->test) model->test = i;


		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);



		FlushCache(0);

	}

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

}


void drawvu0(float* matrix,void *data,int n)
{
	int i = 0,l;
	void *in = data;
	u64 output[2][32*1024] __attribute__((aligned(16)));
	u32 out[12];


	void *kickBuffer,*currentBuffer;
	n = n/2;
	for(l  = 0;l < 2;l++)
	{
		currentBuffer = (char*)&output[l];


		kickBuffer = currentBuffer;



		*((u64*)currentBuffer)++ = DMA_CNT_TAG(3);
		*((u64*)currentBuffer)++ = 0;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(1,0,0,0,0,1);
		*((u64*)currentBuffer)++ = 0x0E;

		*((u64*)currentBuffer)++ = GS_SET_PRIM(GS_PRIM_TRIANGLE,0, 1, 0, 0,0, 1, 0, 0);
		*((u64*)currentBuffer)++ = 0x00;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(n*3,0,0,0,1,3);
		*((u64*)currentBuffer)++ = 0x531;


		*((u64*)currentBuffer)++ = DMA_CNT_TAG(n*9);
		*((u64*)currentBuffer)++ = 0;



		for(i  = 0;i < n*3;i++)
		{

			asm __volatile__ (
			"lqc2		vf1, 0x00(%0)	\n"
			"lqc2		vf2, 0x10(%0)	\n"
			"lqc2		vf3, 0x20(%0)	\n"
			"lqc2		vf4, 0x30(%0)	\n"

			"lqc2		vf6, 0x20(%2)	\n"
			"vmulax		ACC,vf1, vf6	\n"
			"vmadday	ACC,vf2, vf6	\n"
			"vmaddaz	ACC,vf3, vf6	\n"
			"vmaddw		vf5,vf4, vf6	\n"


			"vdiv		Q, vf0w, vf5w	\n"
			"vwaitq						\n"
			"vmulq.xyz	vf6, vf5, Q		\n"
			"vftoi4		vf6, vf6		\n"


			"sqc2		vf6, 0x20(%1)	\n"
			"sqc2		vf6, 0x00(%1)	\n"
			"sqc2		vf6, 0x10(%1)	\n"
			: : "r" (matrix), "r" (out), "r" (in): "memory");
			in += 48;
			*((u64*)currentBuffer)++ = GS_SET_RGBAQ(128, 128, 128, 0x80,0);
			//*((u64*)currentBuffer)++ = 1;

			*((u64*)currentBuffer)++ = (out[8]) + ( (out[9])<<16) + ((u64)out[10]<<32);
			//*((u64*)currentBuffer)++ = 3;

			*((u64*)currentBuffer)++ = (out[8]) + ( (out[9])<<16) + ((u64)out[10]<<32);
			//*((u64*)currentBuffer)++ = 5;

			//printf("%d : %d %d %d\n",i,out[8]>>4,out[9]>>4,out[10]>>4);

		}

		*((u64*)currentBuffer)++ = DMA_END_TAG(2);
		*((u64*)currentBuffer)++ = 0;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(1,1,0,0,0,1);
		*((u64*)currentBuffer)++ = 0X0E;

		*((u64*)currentBuffer)++ = 1;
		*((u64*)currentBuffer)++ = 0X61;



		while( (RW_REGISTER_U32(D2_CHCR)) &0x100);


		RW_REGISTER_U32(D2_QWC ) = 0;
		RW_REGISTER_U32(D2_MADR) = 0;
		RW_REGISTER_U32(D2_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

		FlushCache(0);

	}


	while( (RW_REGISTER_U32(D2_CHCR)) &0x100);
}
#endif

