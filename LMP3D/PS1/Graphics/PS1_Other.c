#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>
#include <psxgpu.h>

#include "LMP3D/LMP3D.h"

#define BUFFERMAX 0x1000
static unsigned int Bufferz[BUFFERMAX];

static int Bufferaddr;
static int Buffermaddr[BUFFERMAX];
static int Buffersaddr[BUFFERMAX+BUFFERMAX];

static int Bufferv[BUFFERMAX+BUFFERMAX][4];
static int *Buffervt[BUFFERMAX+BUFFERMAX];

#include "LMP3D/PS1/PS1.h"
asm(EQU_GTE);





//----
void bitmap_font2(char *texte,int tx,int ty)
{

}

void PS1_Buffer_Init()
{
	Bufferaddr = BUFFERMAX;

	while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_CMD_READY));
	while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_DMA_READY));


	RW_REGISTER_U32(DPCR) = 0x08000000;

	RW_REGISTER_U32(D6_MADR) = (unsigned int)Bufferz+(BUFFERMAX*4)-4;
	RW_REGISTER_U32(D6_BCR) = BUFFERMAX;
	RW_REGISTER_U32(D6_CHCR) = DMA_CHCR_SET(0,1,0,0,0,0,1,1,0,0);




	RW_REGISTER_U32(DPCR) = 0x00000800;
}


//Draw model 3D
void LMP3D_Model_Draw(LMP3D_Model *model)
{
	int z;
	int l,k = 0;

	short *v = model->v;
	int *vt = model->vt;
	int n = model->nf;
	int nv = model->nv;

	n = 9*n;

	for(l = 0;l < n;l+=9)
	{
		asm (

		//Vec
		"lwc2	VXY0,0(%0)\n	"
		"lwc2	VZ0,4(%0)\n	"

		"lwc2	VXY1,6(%0)\n	"
		"lwc2	VZ1,10(%0)\n	"

		"lwc2	VXY2,12(%0)\n	"
		"lwc2	VZ2,16(%0)\n	"

		"cop2	RTPT\n	"
		"cop2	AVSZ3\n	"

		//"swc2	$7,0(%1)\n	"

		 :: "r"(&v[l]));

		 asm  (
		"mfc2 %0,OTZ\n	"
		 : "=r"(z): "r"(z) );
		z &= 0xFFF;

		if(Bufferz[z] > BUFFERMAX)
		{
			Buffersaddr[z] = -1;
			asm  (
			//SXY 1/2/3 : $12/$13/$14
			"swc2	SXY0,0(%0)\n	"
			"swc2	SXY1,4(%0)\n	"
			"swc2	SXY2,8(%0)\n	"

			 :: "r"(Bufferv[z]));

			Buffervt[z] = &vt[k];
			Bufferz[z] = z;

		}else
		{
			if(Buffersaddr[z] == -1)
			{
				Buffersaddr[z] = Bufferaddr;
			}else
			{
				Buffersaddr[Buffermaddr[z]] = Bufferaddr;
			}


			Buffermaddr[z] = Bufferaddr;
			Buffersaddr[Bufferaddr] = -1;
			asm  (
			//SXY 1/2/3 : $12/$13/$14
			"swc2	SXY0,0(%0)\n	"
			"swc2	SXY1,4(%0)\n	"
			"swc2	SXY2,8(%0)\n	"

			 :: "r"(Bufferv[Bufferaddr]));

			 Buffervt[Bufferaddr] = &vt[k];

			 Bufferaddr++;

		}

		k+=3;
	}
}


void PS1BufferDraw()
{
	RW_REGISTER_U32(DPCR) = 0x00000800;
	int list[10];

	list[0] = (7<<24);
	list[1] = GP0_CMD_COLOR(GP0_TRIANGLE_TOB,255,255,255);
	list[8] = 0x00ffffff;

	int i,l;
	Bufferz[BUFFERMAX-1] = BUFFERMAX-1;
	Buffersaddr[BUFFERMAX-1] = -1;

	for(i = 0;i < BUFFERMAX;i++)
	{
		while( (Bufferz[i] != i)) i++;

		l = i;

		while(1)
		{

			//printf("%d %d %d\n",Bufferv[l][0]&0xFF,Bufferv[l][1]&0xFF,Bufferv[l][2]&0xFF);
			//printf("%d %d %d\n-------------\n",Bufferv[l][0]>>16,Bufferv[l][1]>>16,Bufferv[l][2]>>16);
			list[2] = Bufferv[l][0];
			list[3] = Buffervt[l][0]+((240<<6)<<16);
			list[4] = Bufferv[l][1];
			list[5] = Buffervt[l][1]+((GP0_TEXPAGE(0,1,0,1,0,1,0,0,0)&0xFFFF)<<16);
			list[6] = Bufferv[l][2];
			list[7] = Buffervt[l][2];

			RW_REGISTER_U32(D2_MADR) = list;
			RW_REGISTER_U32(D2_BCR) = 0;
			RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0); //0x01000401;

			while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_CMD_READY));
			while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_DMA_READY));

			if(Buffersaddr[l] == -1) break;

			l = Buffersaddr[l];
		}

	}

	PS1_Buffer_Init();

	//printf("%d\n",sizeof(Bufferz)+sizeof(Buffermaddr)+sizeof(Buffersaddr)+sizeof(Bufferv)+sizeof(Buffervt));
}

#endif

