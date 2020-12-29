#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>


#include "LMP3D/LMP3D.h"

#define BUFFERMAX 0x1000

const short buffermax = BUFFERMAX;
static short Bufferz[BUFFERMAX];

static int Bufferaddr;
static short Buffermaddr[BUFFERMAX];
static short Buffersaddr[BUFFERMAX];

static int Bufferv[BUFFERMAX][4];
static int *Buffervt[BUFFERMAX];

#include "LMP3D/PS1/PS1.h"
asm(EQU_GTE);

void PS1_TRS(Vector3i position,Vector3s rotate,Vector3s scale)
{
	short rotateMatrix[10],rotateMatrix2[10];
	short projectionMatrix[10],cameraMatrix[10];

	//T
	position.x = position.x<<4;
	position.y = position.y<<4;
	position.z = position.z<<4;

	int rotateOn = 1;

	if(rotate.y != 0)
	{
		LMP3D_MatrixRotateYS(rotateMatrix, rotate.y);

		if(rotate.x != 0)
		{
			LMP3D_MatrixRotateXS(rotateMatrix2, rotate.x);
			PS1_GTE_MulMat(rotateMatrix2,rotateMatrix,rotateMatrix);
		}

		if(rotate.z != 0)
		{
			LMP3D_MatrixRotateZS(rotateMatrix2, rotate.z);
			PS1_GTE_MulMat(rotateMatrix2,rotateMatrix,rotateMatrix);
		}
	}else
	{
		if(rotate.x != 0)
		{
			LMP3D_MatrixRotateXS(rotateMatrix, rotate.x);

			if(rotate.z != 0)
			{
				LMP3D_MatrixRotateZS(rotateMatrix2, rotate.z);
				PS1_GTE_MulMat(rotateMatrix2,rotateMatrix,rotateMatrix);
			}
		}else
		{
			if(rotate.z != 0)
				LMP3D_MatrixRotateZS(rotateMatrix, rotate.z);
			else rotateOn = 0;
		}
	}

	//S
	cameraMatrix[0+0] = scale.x;
	cameraMatrix[0+1] = 0;
	cameraMatrix[0+2] = 0;

	cameraMatrix[3+0] = 0;
	cameraMatrix[3+1] = scale.y;
	cameraMatrix[3+2] = 0;

	cameraMatrix[6+0] = 0;
	cameraMatrix[6+1] = 0;
	cameraMatrix[6+2] = scale.z;


	if(rotateOn == 1)
	{
		//S+R
		PS1_GTE_MulMat(cameraMatrix,rotateMatrix,cameraMatrix);
	}

	PS1_GTE_ModelView_Set(cameraMatrix,&position);
}

void PS1_Buffer_Init()
{
	Bufferaddr = 0;

	register int i;

	for(i = 0;i < BUFFERMAX;i++)
	{
		Bufferz[i] = 0x7000;
	}
}


//Draw model 3D
void LMP3D_Model_Draw(LMP3D_Model *model)
{
	PS1_TRS(model->iposition,model->irotate,model->iscale);

	int z;
	int l,k = 0;

	short *v = model->v;
	int *vt = model->vt;
	int n = model->nf;
	//int nv = model->nv;

	n = 9*n;

	int normal = 0;

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

		"cop2	RTPT\n	" //23


		 :: "r"(&v[l]));

		 asm  (
		"cop2	NCLIP\n	" //8
		"mfc2 %0,MAC0\n	"
		 : "=r"(normal): "r"(normal) );

		if(normal >= 0)
		{
			k+=3;
			continue;
		}

		 asm  (
		"cop2	AVSZ3\n	" //5
		"mfc2 %0,OTZ\n	"
		 : "=r"(z): "r"(z) );

		if(z < 0x100 )
		{
			k+=3;
			continue;
		}
		z = z>>4;
		z = -z;
		z &= 0xFFF;

		if(Bufferz[z] >= BUFFERMAX)
		{
			Bufferz[z] = Bufferaddr;
		}else
		{
			int j = Bufferz[z];

			if(Buffersaddr[j] == -1)
			{
				Buffersaddr[j] = Bufferaddr;
			}else
			{
				Buffersaddr[Buffermaddr[z]] = Bufferaddr;
			}

			Buffermaddr[z] = Bufferaddr;
		}

		asm  (
		//SXY 0/1/2 : $12/$13/$14
		"swc2	SXY0,0(%0)\n	"
		"swc2	SXY1,4(%0)\n	"
		"swc2	SXY2,8(%0)\n	"

		 :: "r"(Bufferv[Bufferaddr]));

		Buffervt[Bufferaddr] = &vt[k];
		Buffersaddr[Bufferaddr] = -1;
		Bufferaddr++;

		k+=3;
	}

	asm (

	"li	$t0,0\n	"

	"ctc2 $t0,TRX\n	"
	"ctc2 $t0,TRY\n	"
	"ctc2 $t0,TRZ\n	"

	);
}


void PS1_BufferDraw()
{
	RW_REGISTER_U32(DPCR) = 0x00000800;

	int list[10];

	list[0] = (7<<24);
	list[1] = GP0_CMD_COLOR(GP0_TRIANGLE_TOR,0,0,0);
	list[8] = 0x00ffffff;

	int i,l;
	Bufferz[BUFFERMAX-1] = BUFFERMAX-1;
	Buffersaddr[BUFFERMAX-1] = -1;

	int val = 0;

	for(i = 0;i < BUFFERMAX;i++)
	{
		while(Bufferz[i] >= buffermax) i++;

		l = Bufferz[i];

		while(1)
		{
			list[2] = Bufferv[l][0];
			list[3] = Buffervt[l][0];
			list[4] = Bufferv[l][1];
			list[5] = Buffervt[l][1];
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

	Bufferaddr = 0;

	PS1_Buffer_Init();
}


#endif
