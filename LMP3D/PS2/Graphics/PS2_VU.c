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
	int size = (end-start)>>1;

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
	void* chain = (u64*)&tempDma;

	// get the size of the code as we can only send 256 instructions in each MPGtag
	count = pdkVu1Size( start, end );

	while( count > 0 )
	{
		u32 current_count = count > 256 ? 256 : count;

		*((u64*) chain)++ = DMA_REF_TAG( (u32)start, current_count>>1 );
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
	//asm __volatile__("sync.l");

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

	//RW_REGISTER_U32(VIF1_ITOPS) = 980; //GIF
	//RW_REGISTER_U32(VIF1_ITOP)  = 0;   //VU1

}

void PS2_VU1_MatrixGif(float *matrix)
{
	void *currentBuffer,*kickBuffer;
	u64 dmaBuffer [256] __attribute__((aligned(16)));
	u64 primv = GS_SET_PRIM(GS_PRIM_TRIANGLE,GS_IIP_FLAT,GS_TME_TEXTURE_ON, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_STQ,
							GS_CTXT_0, GS_FIX_0);

	u64 giftag[2];
	giftag[0] = GS_SET_GIFTAG(3, 1, 1, primv,0, 3);
	giftag[1] = 0x512;

	kickBuffer = dmaBuffer;
	currentBuffer = (kickBuffer);

	*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)matrix , 4);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,4,1);

	*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)&giftag, 1);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,1,1003);

	*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)&giftag, 1);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,1,1013);

	*((u64*)currentBuffer)++ = DMA_END_TAG(0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSH,0,0);

	FlushCache(0);

	RW_REGISTER_U32(D1_MADR) = 0;
	RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
	RW_REGISTER_U32(D1_QWC ) = 0;
	RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);
}

void PS2_VU_Draw(float* matrix,LMP3D_Model *model)
{
	void *data = model->v;
	void *dataIndex = model->index;
	void *datavt = model->vt;
	void *currentBuffer,*kickBuffer;
	int i;
	int switchBuffer = 0;
	u64 dmaBuffer [2][2048] __attribute__((aligned(16)));

	int nindex1,nvertex1;
	int nindex2,nvertex2;
	u32 adress=0,adress2=0;
	int db = 2;
	int dnvertex1,snindex1,snvertex1;
	int dnvertex2,snindex2,snvertex2;

	LMP3D_Texture_Setup(model->texture[0]);

	PS2_VU1_MatrixGif(matrix);

	i = 0;


	for(i = 0;i < model->ngroup;i+=2)
	{
		kickBuffer = &dmaBuffer[switchBuffer];

		currentBuffer =  UNCACHED_SEG(kickBuffer);

		switchBuffer = !switchBuffer;
		if(i == model->ngroup-1) db = 1;

		nvertex1 = model->groupvertex[i];
		nindex1 = model->groupface[i];

		dnvertex1 = nvertex1>>2;

		snvertex1 = (nvertex1 + (nvertex1&1) );
		snindex1  = (nindex1 + (nindex1&1) );

		if(nvertex1&3) dnvertex1++;

		nindex2 = model->groupface[i+1];
		nvertex2 = model->groupvertex[i+1];

		dnvertex2 = nvertex2>>2;

		snindex2  = (nindex2 + (nindex2&1) );
		snvertex2 = (nvertex2 + (nvertex2&1) );

		if(nvertex2&3) dnvertex2++;

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,1,0);

		*((u16*)currentBuffer)++ = db;
		*((u16*)currentBuffer)++ = 0;
		*((u16*)currentBuffer)++ = 0;
		*((u16*)currentBuffer)++ = 0;

		*((u64*)currentBuffer)++ = 0;


		*((u64*)currentBuffer)++ = DMA_CNT_TAG(1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,1,5);

		*((u16*)currentBuffer)++ = nindex2;
		*((u16*)currentBuffer)++ = dnvertex2;
		*((u16*)currentBuffer)++ = nindex1;
		*((u16*)currentBuffer)++ = dnvertex1;

		*((u64*)currentBuffer)++ = 0;

		if(i == 0)
		{
			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(data+adress)  , snvertex1>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex1,6);
		}

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,0);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(datavt+adress)  , snvertex1>>1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex1,166);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(dataIndex+adress2) , snindex1>>1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snindex1,326);

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCNT,0,512);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);

		//-----------
		if(db == 2)
		{
			adress2 += 178*4*2;
			adress += 160*4*2;

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(data+adress) , snvertex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex2,504);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCNT,0,896);

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(datavt+adress) , snvertex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex2,504+160);

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(dataIndex+adress2) , snindex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snindex2,824);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCNT,0,512);
		}

		//-----------

		adress2 += 178*4*2;
		adress += 160*4*2;


		if(i+2 < model->ngroup)
		{
			nvertex1 = model->groupvertex[i];
			snvertex1 = (nvertex1 + (nvertex1&1) );

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(data+adress)  , snvertex1>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex1,6);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCNT,0,896);
		}

		//-----------

		//printf("%d %d\n",RW_REGISTER_U32(VIF1_TOP),RW_REGISTER_U32(VIF1_TOPS));
		*((u64*)currentBuffer)++ = DMA_END_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);

		FlushCache(0);
		while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);
	}

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);
}

#endif

