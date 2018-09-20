#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>


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

void LMP3D_Math_NormalizeInt(short *snormal)
{
	int normal[3];
	normal[0] = snormal[0];
	normal[1] = snormal[1];
	normal[2] = snormal[2];

	register unsigned int mag = LMP3D_sqrti12( (normal[0]*normal[0]) + (normal[1]*normal[1]) + (normal[2]*normal[2]) );


	/*
	if(mag != 1.0f)
	{
		mag = 1.0f/mag;
		normal[0] *= mag;
		normal[1] *= mag;
		normal[2] *= mag;
	}*/
}
/*
void LMP3D_Math_NormalizeFPU(float *normal)
{
	register float mag = sqrtf( (normal[0]*normal[0]) + (normal[1]*normal[1]) + (normal[2]*normal[2]) );
	if(mag != 1.0f)
	{
		mag = 1.0f/mag;
		normal[0] *= mag;
		normal[1] *= mag;
		normal[2] *= mag;
	}
}

float LMP3D_Math_Dot3FPU(float *x,float *y)
{
	return (x[0]*y[0]) + (x[1]*y[1]) + (x[2]*y[2]);
}

float LMP3D_Math_Dot4FPU(float *x,float *y)
{
	return  (x[0]*y[0]) + (x[1]*y[1]) + (x[2]*y[2]) + (x[3]*y[3]);
}

void LMP3D_Math_CrossFPU(float *r,float *x,float *y)
{
	register float x0,x1,x2;
	x0 = x[0];
	x1 = x[1];
	x2 = x[2];
	r[0] =  (x1*y[2]) - (x2*y[1]);
	r[1] =  (x2*y[0]) - (x0*y[2]);
	r[2] =  (x0*y[1]) - (x1*y[0]);
}
*/
void PS1_Camera_LookAt(short *MatrixLookAt,Vector3i eye,Vector3i center,Vector3i up)
{
	short x[3], y[3], z[3];

	short teye[3];
	teye[0] = eye.x;
	teye[1] = eye.y;
	teye[2] = eye.z;

	// Z = Eye - Center;
	z[0] = eye.x - center.x;
	z[1] = eye.y - center.y;
	z[2] = eye.z - center.z;

	//Z normalize
	LMP3D_Math_NormalizeFPU(z);


	y[0] = up.x;
	y[1] = up.y;
	y[2] = up.z;
/*
	LMP3D_Math_CrossFPU(x,y,z);
	LMP3D_Math_CrossFPU(y,z,x);

	LMP3D_Math_NormalizeFPU(x);
	LMP3D_Math_NormalizeFPU(y);

	MatrixLookAt[(0<<2)+0] = x[0];
	MatrixLookAt[(0<<2)+1] = y[0];
	MatrixLookAt[(0<<2)+2] = z[0];
	MatrixLookAt[(0<<2)+3] = 0;

	MatrixLookAt[(1<<2)+0] = x[1];
	MatrixLookAt[(1<<2)+1] = y[1];
	MatrixLookAt[(1<<2)+2] = z[1];
	MatrixLookAt[(1<<2)+3] = 0;

	MatrixLookAt[(2<<2)+0] = x[2];
	MatrixLookAt[(2<<2)+1] = y[2];
	MatrixLookAt[(2<<2)+2] = z[2];
	MatrixLookAt[(2<<2)+3] = 0;

	MatrixLookAt[(3<<2)+0] = -LMP3D_Math_Dot3FPU(x,teye);
	MatrixLookAt[(3<<2)+1] = -LMP3D_Math_Dot3FPU(y,teye);
	MatrixLookAt[(3<<2)+2] = -LMP3D_Math_Dot3FPU(z,teye);
	MatrixLookAt[(3<<2)+3] = 1.0f;
	*/
}


void PS1_TRS(Vector3i position,Vector3s rotate,Vector3s scale)
{
	short vec[10];

	vec[0] = 1<<12;
	LMP3D_Math_NormalizeInt(vec);
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
	Bufferaddr = BUFFERMAX;

	while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_CMD_READY));
	while(!(RW_REGISTER_U32(GPUSTAT) & GPUSTAT_DMA_READY));


	RW_REGISTER_U32(DPCR) = 0x08000000;

	RW_REGISTER_U32(D6_MADR) = (unsigned int)Bufferz+(BUFFERMAX*4)-4;
	RW_REGISTER_U32(D6_BCR) = BUFFERMAX;
	RW_REGISTER_U32(D6_CHCR) = DMA_CHCR_SET(0,1,0,0,0,0,1,1,0,0);


	//printf("%x %x\n",Bufferz[0],Bufferz[2]);


	RW_REGISTER_U32(DPCR) = 0x00000800;
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

	//int val;

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


		 :: "r"(&v[l]));

		 asm  (
		"cop2	NCLIP\n	"
		"mfc2 %0,MAC0\n	"
		 : "=r"(normal): "r"(normal) );

		if(normal >= 0)
		{
			k+=3;
			continue;
		}

		 asm  (
		"cop2	AVSZ3\n	"
		"mfc2 %0,OTZ\n	"
		 : "=r"(z): "r"(z) );

		// if(l == 0) printf("%d\n",z);

		if(z < 0x100 )
		{
			k+=3;
			continue;
		}
		z = z>>4;
		z = -z;
		z &= 0xFFF;

		if(Bufferz[z] > BUFFERMAX)
		{
			Buffersaddr[z] = -1;
			asm  (
			//SXY 0/1/2 : $12/$13/$14
			"swc2	SXY0,0(%0)\n	"
			"swc2	SXY1,4(%0)\n	"
			"swc2	SXY2,8(%0)\n	"

			 :: "r"(Bufferv[z]));

			Buffervt[z] = &vt[k];
			Bufferz[z] = z;

		}
		else
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
			//SXY 0/1/2 : $12/$13/$14
			"swc2	SXY0,0(%0)\n	"
			"swc2	SXY1,4(%0)\n	"
			"swc2	SXY2,8(%0)\n	"

			 :: "r"(Bufferv[Bufferaddr]));

			 Buffervt[Bufferaddr] = &vt[k];

			 Bufferaddr++;
		}

		k+=3;
	}

	asm (

	"li	$t0,0\n	"

	"ctc2 $t0,TRX\n	"
	"ctc2 $t0,TRY\n	"
	"ctc2 $t0,TRZ\n	"

	);

	//printf("%d\n",val);
}


void PS1_BufferDraw()
{
	RW_REGISTER_U32(DPCR) = 0x00000800;

	//int list2[0x100];
	int list[10];

	list[0] = (7<<24);
	list[1] = GP0_CMD_COLOR(GP0_TRIANGLE_TOR,0,0,0);
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

/*
			list2[j++] = GP0_CMD_COLOR(GP0_TRIANGLE_TOR,0,0,0);
			list2[j++] = Bufferv[l][0];
			list2[j++] = Buffervt[l][0]+((240<<6)<<16);
			list2[j++] = Bufferv[l][1];
			list2[j++] = Buffervt[l][1]+((GP0_TEXPAGE(0,1,0,1,0,1,0,0,0)&0xFFFF)<<16);
			list2[j++] = Bufferv[l][2];
			list2[j++] = Buffervt[l][2];


			if(j >= 0xFF-7)
			{
				list2[0] = (j-1)<<24;
				list2[j] = 0x00ffffff;
				RW_REGISTER_U32(D2_MADR) = list2;
				RW_REGISTER_U32(D2_BCR) = 0;

				RW_REGISTER_U32(D2_CHCR) = DMA_CHCR_SET(1,0,0,2,0,0,1,0,0,0);
				j = 1;

			}
*/
			//printf("%d %d %d\n",Bufferv[l][0]&0xFF,Bufferv[l][1]&0xFF,Bufferv[l][2]&0xFF);
			//printf("%d %d %d\n-------------\n",Bufferv[l][0]>>16,Bufferv[l][1]>>16,Bufferv[l][2]>>16);

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

	PS1_Buffer_Init();

	//printf("%d\n",sizeof(Bufferz)+sizeof(Buffermaddr)+sizeof(Buffersaddr)+sizeof(Bufferv)+sizeof(Buffervt));
}


#endif

