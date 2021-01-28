#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include <kernel.h>
#include "LMP3D/LMP3D.h"
#include <tamtypes.h>

#include "LMP3D/PS2/PS2.h"
#include "PS2_vu1Triangle.inc"

#define PS2_VU_Func_Vectrex 		(0x0000>>3)
#define PS2_VU_Func_Vectrex_Loop 	(0x0010>>3)
#define PS2_VU_Func_Index	 		(0x0020>>3)


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
	u8 tempDma[0x200] ALIGNED(16);

	// get the size of the code as we can only send 256 instructions in each MPGtag
	count = pdkVu1Size( start, end );
	void* chain = (u64*)&tempDma;

	while( count > 0 )
	{
		u32 current_count = count > 256 ? 256 : count;

		*((u64*) chain)++ = DMA_REF_TAG( (u32)start, current_count>>1 );
		*((u32*) chain)++ = VIF_CODE( VIF_NOP,0,0 );
		*((u32*) chain)++ = VIF_CODE( VIF_MPG,current_count&0xff,Dest );

		start += current_count*8;
		count -= current_count;
		Dest += current_count;
	}


	*((u64*) chain)++ = DMA_END_TAG( 0 );
	*((u32*) chain)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*) chain)++ = VIF_CODE(VIF_NOP,0,0);


	// Send it to vif1
	FlushCache(0);
	asm __volatile__("sync.l");

	RW_REGISTER_U32(D1_MADR) = 0;
	RW_REGISTER_U32(D1_TADR) = (u32)tempDma;
	RW_REGISTER_U32(D1_QWC ) = 0;
	RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static u64 dmaBuffer[16] __attribute__((aligned(16)));
static u64 matrixup[8];
static u64 giftag[2];


void PS2_VU_Init()
{

	void *end = vu1Triangle_data + vu1Triangle_size;
	pdkVu1UploadProg(0,&vu1Triangle_data,end);

	//RW_REGISTER_U32(VIF1_ITOPS) = 980; //GIF
	//RW_REGISTER_U32(VIF1_ITOP)  = 0;   //VU1

	//-------------------
	void *currentBuffer;
	u64 primv = GS_SET_PRIM(GS_PRIM_TRIANGLE,GS_IIP_FLAT,GS_TME_TEXTURE_ON, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_ON, GS_FST_STQ,
							GS_CTXT_0, GS_FIX_0);

	giftag[0] = GS_SET_GIFTAG(3*1, 1, 1, primv,0, 3);
	giftag[1] = 0x512;
	//5 : XYZ2
	//4 : XYZF2
	//3 : UV
	//2 : ST
	//1 : RGBAQ

	currentBuffer = (dmaBuffer);

	*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)matrixup , 4);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_STCYL,0,0x0101 );
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,4,1);

	*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)giftag, 1);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_32,1,942);

	*((u64*)currentBuffer)++ = DMA_END_TAG(0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
	*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);

	FlushCache(0);

}








void PS2_VU1_MatrixGif(float *matrix)
{
	u64 *matrixc = matrix;
	u64 *matrixus = UNCACHED_SEG(matrixup);

	matrixus[0] = matrixc[0];
	matrixus[1] = matrixc[1];
	matrixus[2] = matrixc[2];
	matrixus[3] = matrixc[3];
	matrixus[4] = matrixc[4];
	matrixus[5] = matrixc[5];
	matrixus[6] = matrixc[6];
	matrixus[7] = matrixc[7];

	RW_REGISTER_U32(D1_MADR) = 0;
	RW_REGISTER_U32(D1_TADR) = (u32)dmaBuffer;
	RW_REGISTER_U32(D1_QWC ) = 0;
	RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);

	//while( (RW_REGISTER_U32(D1_CHCR)) &0x100);
}

extern int varg_debug;




void PS2_VU_Load(LMP3D_Model *model)
{
	void *data = model->v;
	void *dataIndex = model->index;
	void *datavt = model->vt;
	void *currentBuffer;
	int i=0;

	int nindex1,nvertex1;
	int nindex2,nvertex2;
	u32 adress=0,adress2=0;
	int db = 2;
	int dnvertex1,snindex1,snvertex1;
	int dnvertex2,snindex2,snvertex2;

	int n = sizeof(u64)*model->ngroup*18;
	model->displaylist[0] = malloc(n);
/*
	printf("---ok---------------\n");
	u64 tmp = model->displaylist[0] ;

	printf("%d %p %p %d\n",n,model->displaylist[0] ,tmp&0xFFFFFFF0,model->ngroup);
	printf("------------------\n");
*/
	if(model->displaylist[0]  == NULL) return;

	//currentBuffer =  model->displaylist[0] + (sizeof(u64)*17*i);

	for(i = 0;i < model->ngroup;i+=2)
	{
		currentBuffer =  model->displaylist[0] + (sizeof(u64)*17*i);

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
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,1,0);

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
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCAL,0,PS2_VU_Func_Vectrex);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(datavt+adress)  , snvertex1>>1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex1,6+128);

		*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(dataIndex+adress2) , snindex1>>1);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snindex1,518);

		*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCALF,0,PS2_VU_Func_Index);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);

		//-----------

		if(db == 2)
		{
			adress2 += snindex1*4*2;
			adress += snvertex1*4*2;

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(data+adress) , snvertex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex2,262);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCALF,0,PS2_VU_Func_Vectrex_Loop);

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(datavt+adress) , snvertex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex2,262+128);

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(dataIndex+adress2) , snindex2>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snindex2,518+212);

			*((u64*)currentBuffer)++ = DMA_CNT_TAG(0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_MSCALF,0,PS2_VU_Func_Index);
		}

		//-----------

		adress2 += snindex2*4*2;
		adress += snvertex2*4*2;


		if(i+2 < model->ngroup)
		{
			nvertex1 = model->groupvertex[i+2];
			snvertex1 = (nvertex1 + (nvertex1&1) );

			*((u64*)currentBuffer)++ = DMA_REF_TAG( (u32)(data+adress)  , snvertex1>>1);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_NOP,0,0);
			*((u32*)currentBuffer)++ = VIF_CODE(VIF_UNPACK_V4_16,snvertex1,6);
		}

		//-----------

		*((u64*)currentBuffer)++ = DMA_END_TAG(0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHE,0,0);
		*((u32*)currentBuffer)++ = VIF_CODE(VIF_FLUSHA,0,0);

	}


	FlushCache(0);
}

void PS2_VU_Draw(float* matrix,LMP3D_Model *model)
{

	void *kickBuffer;
	int i;


	PS2_VU1_MatrixGif(matrix);


	LMP3D_Texture_Setup(model->texture[0]);

	//u32 debug = RW_REGISTER_U32(VU1_Mem+(4*1)+(16*8));
	//printf("%d\n",debug>>4);

	i = 0;

	int z = 0;
	int l = 0;
	int n = 0;
/*
	for(i = 0;i < model->ngroup;i++)
	{

		n += model->groupface[i];
		printf("%d,",n);

	}

	printf("\n");

	for(i = 0;i < model->ntexture;i++)
	{
		printf("%d\n",model->texture_begin[i]/3);
	}
	n = 0;
*/
	for(i = 0;i < model->ngroup;i+=2)
	{
		/*
		if(n >= model->texture_begin[l])
			l++;
		LMP3D_Texture_Setup(model->texture[l]);
		n += model->groupface[i];
		n += model->groupface[i+1];
*/
		kickBuffer = model->displaylist[0] + (sizeof(u64)*17*i);

		while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

		RW_REGISTER_U32(D1_MADR) = 0;
		RW_REGISTER_U32(D1_TADR) = (u32)kickBuffer;
		RW_REGISTER_U32(D1_QWC ) = 0;
		RW_REGISTER_U32(D1_CHCR) = EE_SET_CHCR(1,1,0,1,0,1,0);
	}

	while( (RW_REGISTER_U32(D1_CHCR)) &0x100);

	//printf("%d\n",z);
}



#endif
