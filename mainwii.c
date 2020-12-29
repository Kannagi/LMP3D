
#include <stdio.h>


#ifdef Wii

// adapted from	the	original acube demo	by tkcne.
#include "LMP3D/LMP3D.h"

// enjoy

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>



void update_screen(Mtx viewMatrix);
static void	copy_buffers(u32 unused);


WGPipe* const wgPipe2 = (WGPipe*)0xCC008000;

#define GX_FIFO 0xCC008000
#define RW_REGISTER_U8(REG) 	*((volatile u8 *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile u16 *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile u32 *)(REG))
#define RW_REGISTER_FLOAT(REG) 	*((volatile float *)(REG))

#define GX_LOAD_BP_REG(x)				\
	do {								\
		wgPipe->U8 = 0x61;				\
		asm volatile ("" ::: "memory" ); \
		wgPipe->U32 = (u32)(x);		\
		asm volatile ("" ::: "memory" ); \
	} while(0)

void GX2_BeginTri(float x)
{
	//printf("%d\n",primitve);
	//return;

	/*
GX_QUADS				0x80
GX_TRIANGLES			0x90
GX_TRIANGLESTRIP		0x98
GX_TRIANGLEFAN			0xA0
GX_LINES				0xA8
GX_LINESTRIP			0xB0
GX_POINTS				0xB8
0-7 Vertex format index
	*/

	RW_REGISTER_U8(GX_FIFO) = 0x90;
	RW_REGISTER_U16(GX_FIFO) = 3*2;

	RW_REGISTER_FLOAT(GX_FIFO) = 1.0f+x;
	RW_REGISTER_FLOAT(GX_FIFO) = 1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;

	RW_REGISTER_U8(GX_FIFO) = 0x00;
	RW_REGISTER_U8(GX_FIFO) = 0xF1;
	RW_REGISTER_U8(GX_FIFO) = 0xF2;

	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;

	RW_REGISTER_U8(GX_FIFO) = 0xF3;
	RW_REGISTER_U8(GX_FIFO) = 0x04;
	RW_REGISTER_U8(GX_FIFO) = 0xF5;

	RW_REGISTER_FLOAT(GX_FIFO) = 1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;

	RW_REGISTER_U8(GX_FIFO) = 0xF6;
	RW_REGISTER_U8(GX_FIFO) = 0xF7;
	RW_REGISTER_U8(GX_FIFO) = 0x08;





	RW_REGISTER_FLOAT(GX_FIFO) = 2.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;

	RW_REGISTER_U8(GX_FIFO) = 0x00;
	RW_REGISTER_U8(GX_FIFO) = 0xF1;
	RW_REGISTER_U8(GX_FIFO) = 0xF2;

	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;

	RW_REGISTER_U8(GX_FIFO) = 0x00;
	RW_REGISTER_U8(GX_FIFO) = 0x04;
	RW_REGISTER_U8(GX_FIFO) = 0x00;

	RW_REGISTER_FLOAT(GX_FIFO) = 3.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = -1.0f;
	RW_REGISTER_FLOAT(GX_FIFO) = 0.0f;

	RW_REGISTER_U8(GX_FIFO) = 0xF6;
	RW_REGISTER_U8(GX_FIFO) = 0xF7;
	RW_REGISTER_U8(GX_FIFO) = 0x08;

}


static inline void GX2_Position3f32(f32 x,f32 y,f32 z)
{
	RW_REGISTER_FLOAT(GX_FIFO) = x;
	RW_REGISTER_FLOAT(GX_FIFO) = y;
	RW_REGISTER_FLOAT(GX_FIFO) = z;
}

static inline void GX2_Color3f32(f32 r, f32 g, f32 b)
{
	RW_REGISTER_U8(GX_FIFO) = (u8)(r * 255.0);
	RW_REGISTER_U8(GX_FIFO) = (u8)(g * 255.0);
	RW_REGISTER_U8(GX_FIFO) = (u8)(b * 255.0);
}
#define _SHIFTL(v, s, w)	\
	((u32) (((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w)	\
	((u32)(((u32)(v) >> (s)) & ((0x01 << (w)) - 1)))

#define GX_LOAD_XF_REGS(x, n)			\
		RW_REGISTER_U8(GX_FIFO) = 0x10;				\
		RW_REGISTER_U32(GX_FIFO) = (u32)(((((n)&0xffff)-1)<<16)|((x)&0xffff));


void GX2_SetCopyClear(GXColor color)
{
	u32 val;

	val = (_SHIFTL(color.a,8,8))|(color.r&0xff);

	RW_REGISTER_U8(GX_FIFO) = 0x61;
	RW_REGISTER_U32(GX_FIFO) = 0x4f000000|val;

	val = (_SHIFTL(color.g,8,8))|(color.b&0xff);
	RW_REGISTER_U8(GX_FIFO) = 0x61;
	RW_REGISTER_U32(GX_FIFO) = 0x50000000|val;

	RW_REGISTER_U8(GX_FIFO) = 0x61;
	RW_REGISTER_U32(GX_FIFO) = 0x51000000|0x00ffffff;
}

static inline void WriteMtxPS4x3(register Mtx mt,register void *wgPipe2)
{
	register f32 tmp0,tmp1,tmp2,tmp3,tmp4,tmp5;
	__asm__ __volatile__ (
		 "psq_l %0,0(%6),0,0\n\
		  psq_l %1,8(%6),0,0\n\
		  psq_l %2,16(%6),0,0\n\
		  psq_l %3,24(%6),0,0\n\
		  psq_l %4,32(%6),0,0\n\
		  psq_l %5,40(%6),0,0\n\
		  psq_st %0,0(%7),0,0\n\
		  psq_st %1,0(%7),0,0\n\
		  psq_st %2,0(%7),0,0\n\
		  psq_st %3,0(%7),0,0\n\
		  psq_st %4,0(%7),0,0\n\
		  psq_st %5,0(%7),0,0"
		  : "=&f"(tmp0),"=&f"(tmp1),"=&f"(tmp2),"=&f"(tmp3),"=&f"(tmp4),"=&f"(tmp5)
		  : "b"(mt), "b"(wgPipe2)
		  : "memory"
	);
}
/*
#define XF_CMD(opcode,length,addr,reg) \
		((u64)ate   <<   0 ) |	\
		((u64)atst  <<   1 ) |	\
		((u64)aref  <<   4 ) |	\
		((u64)afail <<  12 ) |	\
		((u64)date  <<  14 ) |	\
		((u64)datm  <<  15 ) |	\
		((u64)zte   <<  16 ) |	\
		((u64)ztst  <<  17 )
		*/
void GX2_SetViewportJitter(f32 xOrig,f32 yOrig,f32 wd,f32 ht,f32 nearZ,f32 farZ,u32 field)
{
	f32 x0,y0,x1,y1,n,f,z;
	static f32 Xfactor = 0.5;
	static f32 Yfactor = 342.0;
	static f32 Zfactor = 16777215.0;

	if(!field) yOrig -= Xfactor;

	x0 = wd*Xfactor;
	y0 = (-ht)*Xfactor;
	x1 = (xOrig+(wd*Xfactor))+Yfactor;
	y1 = (yOrig+(ht*Xfactor))+Yfactor;
	n = Zfactor*nearZ;
	f = Zfactor*farZ;
	z = f-n;

	//GX_LOAD_XF_REGS(0x101a,6);
	RW_REGISTER_U8(GX_FIFO) = 0x10;
	RW_REGISTER_U32(GX_FIFO) = (u32)(((6-1)<<16)|(0));
	RW_REGISTER_FLOAT(GX_FIFO) = x0;
	RW_REGISTER_FLOAT(GX_FIFO) = y0;
	RW_REGISTER_FLOAT(GX_FIFO) = z;
	RW_REGISTER_FLOAT(GX_FIFO) = x1;
	RW_REGISTER_FLOAT(GX_FIFO) = y1;
	RW_REGISTER_FLOAT(GX_FIFO) = f;
}

void GX2_SetViewport(f32 xOrig,f32 yOrig,f32 wd,f32 ht,f32 nearZ,f32 farZ)
{
	GX2_SetViewportJitter(xOrig,yOrig,wd,ht,nearZ,farZ,1);
}

void GX2_LoadPosMtxImm(Mtx mt,u32 pnidx)
{
	GX_LOAD_XF_REGS((0x0000|(_SHIFTL(pnidx,2,8))),12);
	WriteMtxPS4x3(mt,(void*)wgPipe2);
}

void GX2_LoadProjectionMtx(Mtx44 mt,u8 type)
{
	f32 tmp[7];

	((u32*)((void*)tmp))[6] = (u32)type;
	tmp[0] = mt[0][0];
	tmp[2] = mt[1][1];
	tmp[4] = mt[2][2];
	tmp[5] = mt[2][3];

	switch(type) {
		case GX_PERSPECTIVE:
			tmp[1] = mt[0][2];
			tmp[3] = mt[1][2];
			break;
		case GX_ORTHOGRAPHIC:
			tmp[1] = mt[0][3];
			tmp[3] = mt[1][3];
			break;
		default:
			tmp[1] = 0.0;
			tmp[3] = 0.0;
			break;
	}

	GX_LOAD_XF_REGS(0x1020,7);
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[0];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[1];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[2];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[3];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[4];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[5];
	RW_REGISTER_FLOAT(GX_FIFO) = tmp[6];
}

void GX2_SetScissor(u32 xOrigin,u32 yOrigin,u32 wd,u32 ht)
{
	u32 xo = xOrigin+0x156;
	u32 yo = yOrigin+0x156;
	u32 nwd = xo+(wd-1);
	u32 nht = yo+(ht-1);

	u32 TL=0,BR=0;

	TL = (TL&~0x7ff)|(yo&0x7ff);
	TL = (TL&~0x7FF000)|(_SHIFTL(xo,12,11));

	BR = (BR&~0x7ff)|(nht&0xfff);
	BR = (BR&~0x7FF000)|(_SHIFTL(nwd,12,11));

	GX_LOAD_BP_REG(TL);
	GX_LOAD_BP_REG(BR);
}

void GX2_SetGenMode(void)
{
	u32 flag = 0xFFFFFF;
	GX_LOAD_BP_REG(flag);
}

void GX3_SetCullMode(u8 mode)
{
	static const u8 cm2hw[] = { 0, 2, 1, 3 };

	u32 flag = (0&~0xff000000)|(_SHIFTL(0x00,24,8));
	//flag = (f&~0xC000)|(_SHIFTL(cm2hw[mode],14,2));

	GX_LOAD_BP_REG(flag);
}


/*---------------------------------------------------------------------------------

	nehe lesson 6 port to GX by shagkur

---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>



#define DEFAULT_FIFO_SIZE	(256*1024)

static GXRModeObj *rmode = NULL;
static void *frameBuffer[2] = { NULL, NULL};
//---------------------------------------------------------------------------------
void game4()
{
//---------------------------------------------------------------------------------
	f32 yscale;

	u32 xfbHeight;

	Mtx view;
	Mtx44 perspective;
	Mtx model, modelview;

	u32	fb = 0; 	// initial framebuffer index
	GXColor background = {0x80, 0x80, 0x80, 0xff};


	// init the vi.
	VIDEO_Init();
	WPAD_Init();

	rmode = VIDEO_GetPreferredMode(NULL);

	// allocate 2 framebuffers for double buffering
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// setup the fifo and then init the flipper
	void *gp_fifo = NULL;
	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);

	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);



	//GX2_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	//GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	//GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	//GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	//GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	GX_SetCullMode(GX_CULL_NONE); //4
	//GX2_SetGenMode();
	//GX_CopyDisp(frameBuffer[fb],GX_TRUE);
	//GX_SetDispCopyGamma(GX_GM_1_0);


	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GX_ClearVtxDesc(); //8

	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT); //08
 	GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); //10
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GX_Flush();


	// clears the bg to color and clears the z buffer
	GX2_SetCopyClear(background);

	// other gx setup
	GX2_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);

	//GX2_SetGenMode();

	GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	// setup our camera at the origin
	// looking down the -z axis with y up
	guVector cam = {0.0F, 0.0F, 0.0F},
			up = {0.0F, 1.0F, 0.0F},
		  look = {0.0F, 0.0F, -1.0F};
	guLookAt(view, &cam, &up, &look);


	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
	f32 w = rmode->viWidth;
	f32 h = rmode->viHeight;
	guPerspective(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GX2_LoadProjectionMtx(perspective, GX_PERSPECTIVE);


	int axisx = 0,i,n;
	float x = 0;
/*
	fatInitDefault();

	FILE *file = fopen("zack.bcm","rb");

	if(file != NULL)
	{
		printf("\nOPEN\n");
		fclose(file);
	}else
		printf("\nNO OPEN\n");

	LMP3D_Model * model2;
*/
	//model2 = LMP3D_Load_Model_bcm("zack.bcm");

	float vec[3];

	printf("OK\n");

	float a = 0;

	while(1) {

		WPAD_ScanPads();

		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_LEFT) x -= 0.1;
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_RIGHT) x += 0.1;

		//-----------------


		x = cos(a);
		a += 0.01;
		guMtxIdentity(model);
		guMtxScale(model, 1.1f,1.1f,1.1f);
		guMtxTransApply(model, model, x-1.5f,0.0f,-6.0f);
		guMtxConcat(view,model,modelview);

		GX2_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
		GX2_LoadPosMtxImm(modelview, GX_PNMTX0);
		GX2_BeginTri(x);

		// do this stuff after drawing
		GX_DrawDone();
/*
		RW_REGISTER_U8(GX_FIFO) = 0x61;
		RW_REGISTER_U16(GX_FIFO) = 0x45000002;
		GX_Flush();*/
/*
#define GX_LOAD_BP_REG(x)				\
	do {								\
		wgPipe->U8 = 0x61;				\
		asm volatile ("" ::: "memory" ); \
		wgPipe->U32 = (u32)(x);		\
		asm volatile ("" ::: "memory" ); \
	} while(0)



	void GX_SetZMode(u8 enable,u8 func,u8 update_enable)
{
	__gx->peZMode = (__gx->peZMode&~0x1)|(enable&1);
	__gx->peZMode = (__gx->peZMode&~0xe)|(_SHIFTL(func,1,3));
	__gx->peZMode = (__gx->peZMode&~0x10)|(_SHIFTL(update_enable,4,1));
	GX_LOAD_BP_REG(__gx->peZMode);
}

__gx->peZMode = (__gx->peZMode&~0xff000000)|(_SHIFTL(0x40,24,8));

*/
		fb ^= 1;		// flip framebuffer
		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		VIDEO_Flush();
		VIDEO_WaitVSync();



	}
	return 0;
}




#endif



