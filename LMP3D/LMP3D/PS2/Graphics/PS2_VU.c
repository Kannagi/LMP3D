#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include <kernel.h>
#include "LMP3D/LMP3D.h"
#include <tamtypes.h>

#include "LMP3D/PS2/PS2.h"
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PS2_VU_Init()
{

	void *end = vu1Triangle + size_vu1Triangle;
	pdkVu1UploadProg(0,&vu1Triangle,end);

	RW_REGISTER_U32(VIF1_ITOPS) = 980; //GIF
	RW_REGISTER_U32(VIF1_ITOP)  = 0;   //VU1

}

void drawvu0(float* matrix,void *data,int n)
{
	int i = 0,l;
	float *in = data;
	u64 output[2][32*1024] __attribute__((aligned(16)));
	u32 out[12];

	void *kickBuffer,*currentBuffer;
	//n = n/2;
	//for(l  = 0;l < 2;l++)
	{
		currentBuffer = (char*)&output[l];


		kickBuffer = currentBuffer;



		*((u64*)currentBuffer)++ = DMA_CNT_TAG(3);
		*((u64*)currentBuffer)++ = 0;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(1,0,0,0,0,1);
		*((u64*)currentBuffer)++ = 0x0E;

		*((u64*)currentBuffer)++ = GS_SET_PRIM(GS_PRIM_TRIANGLE,0, 1, 0, 0,0, 1, 0, 0);
		*((u64*)currentBuffer)++ = 0x00;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(3*n*3,0,0,0,0,1);
		*((u64*)currentBuffer)++ = GS_REG_AD;


		*((u64*)currentBuffer)++ = DMA_CNT_TAG( (n*9));
		*((u64*)currentBuffer)++ = 0;



		for(i  = 0;i < n*3;i++)
		{
			asm __volatile__ (
			"lqc2		vf1, 0x00(%0)	\n"
			"lqc2		vf2, 0x10(%0)	\n"
			"lqc2		vf3, 0x20(%0)	\n"
			"lqc2		vf4, 0x30(%0)	\n"

			"lqc2		vf6, 0x00(%2)	\n"
			"lqc2		vf8, 0x10(%2)	\n"
			"vmulax		ACC,vf1, vf6x	\n"
			"vmadday	ACC,vf2, vf6y	\n"
			"vmaddaz	ACC,vf3, vf6z	\n"
			"vmaddw		vf5,vf4, vf6w	\n"


			"vdiv		Q, vf0w, vf5w	\n"
			"vwaitq						\n"
			"vmulq.xyz	vf6, vf5, Q		\n"
			"vftoi4		vf7, vf6		\n"
			"vftoi4		vf9, vf8		\n"

			"sqc2		vf7, 0x20(%1)	\n"
			"sqc2		vf7, 0x00(%1)	\n"
			"sqc2		vf7, 0x10(%1)	\n"
			: : "r" (matrix), "r" (out), "r" (in));
			in += 0x20;
			*((u64*)currentBuffer)++ = GS_SET_RGBAQ(128, 128, 128, 0x80,0);
			*((u64*)currentBuffer)++ = 1;

			//*((u64*)currentBuffer)++ = (out[0]) + ( (out[1])<<16) + ((u64)out[2]<<32);

			//*((u64*)currentBuffer)++ = 0;//(out[0]) + ( (2048+(i*10))<<16);
			*((u64*)currentBuffer)++ = (out[0]) + ( (out[1])<<16) + ((u64)out[2]<<32);
			*((u64*)currentBuffer)++ = 3;

			//*((u64*)currentBuffer)++ = (out[8]) + ( (out[9])<<16) + ((u64)out[10]<<32);
			*((u64*)currentBuffer)++ = (out[8]) + ( (out[9])<<16) + ((u64)out[10]<<32);
			*((u64*)currentBuffer)++ = 5;

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

int addv = 2;


void drawvu1(float* matrix,LMP3D_Model *model)
{

	void *data = model->v;
	void *currentBuffer,*kickBuffer;
	int i,result,n1,n2,n3,n4,n,l;
	u32 adress = 0;
	int switchBuffer = 0,gifswitchBuffer = 0;
	u64 dmaBuffer [2][1024+20] __attribute__((aligned(64)));
	u64 primv = GS_SET_PRIM(GS_PRIM_TRIANGLE,GS_IIP_FLAT,GS_TME_TEXTURE_ON, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_STQ,
							GS_CTXT_0, GS_FIX_0);

/*
	float vec[4];
	u32 out[12];

	vec[0] = model->position.x+5;
	vec[1] = model->position.y+6;
	vec[2] = model->position.z+7;
	vec[3] = 1.0f;

	printf("av %f %f %f\n",vec[0],vec[1],vec[2]);

	asm __volatile__ (
	"lqc2		vf1, 0x00(%0)	\n"
	"lqc2		vf2, 0x10(%0)	\n"
	"lqc2		vf3, 0x20(%0)	\n"
	"lqc2		vf4, 0x30(%0)	\n"

	"lqc2		vf6, 0x00(%2)	\n"
	"vmulax		ACC,vf1, vf6	\n"
	"vmadday	ACC,vf2, vf6	\n"
	"vmaddaz	ACC,vf3, vf6	\n"
	"vmaddw		vf5,vf4, vf6	\n"


	"vdiv		Q, vf0w, vf5w	\n"
	"vwaitq						\n"
	"vmulq.xyz	vf6, vf5, Q		\n"
	//"vftoi4		vf6, vf6		\n"


	"sqc2		vf6, 0x00(%2)	\n"
	: : "r" (matrix), "r" (out), "r" (vec): "memory");

	printf("ap %f %f %f\n",vec[0],vec[1],vec[2]);
*/
	//printf("ap %x %x %x\n",out[0],out[1],out[2]);

	result = model->nf&0xFFFE;
	int vu1nf = (42*3)+40;

	while(result >= 0)
	{
		kickBuffer = &dmaBuffer[switchBuffer];
		currentBuffer = UNCACHED_SEG(kickBuffer);

		switchBuffer = !switchBuffer;


		result -= vu1nf;

		if(result >= 0)
		{
			n = vu1nf;
		}else
		{
			n = vu1nf + result;

			n4 = n-42-42-42;
			n3 = n-42-42;
			n2 = n-42;
			n1 = n;

			if(n1 >= 42) n1 = 42;
			if(n2 >= 42) n2 = 42;
			if(n3 >= 42) n3 = 42;


			//printf("%d %d %d %d %d\n",n,n1,n2,n3,n4);
		}

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)matrix , 4);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,4,0);

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(2);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,2,4);

		*((u64*)currentBuffer)++ = 0;
		*((u32*)currentBuffer)++ = 0;
		*((u32*)currentBuffer)++ = n;

		*((u64*)currentBuffer)++ = GS_SET_GIFTAG(3, 1, 1, primv,0, 3);
		*((u64*)currentBuffer)++ = 0x512;



		if(n == vu1nf)
		{

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress , 120);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,240,6);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,0);

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(1920) , 126);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0 );
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,252,6+(240));

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(1920+2016) , 126);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,252,6+(240+252) );

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(1920+2016+2016) , 126);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,252,6+(240+252+252) );


			adress += 1920+(2016*3);

		}else
		{

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress , n1*3);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,n1*6,6);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,0);

			if(n2 > 0)
			{
				*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(2016) , n2*3);
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0 );
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,n2*6,6+(252));
			}

			if(n3 > 0)
			{
				*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(2016+2016) , n3*3);
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,n3*6,6+(252+252) );
			}

			if(n4 > 0)
			{
				*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)data+adress+(2016+2016+2016) , n4*3);
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
				*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,n4*6,6+(252+252+252) );
			}

			//printf("%x\n",RW_REGISTER_U32(0x10003C40));
		}




		*((u64*)currentBuffer)++ = DMA_END_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);
/*

		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);*/

		FlushCache(0);

		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);


		l = 0;
		while( (RW_REGISTER_U32(D1_CHCR)) &0x100)l++;
		if(l > model->test) model->test = l;

	}

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

}

#endif

