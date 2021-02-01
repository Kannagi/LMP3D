
#include <stdio.h>


#ifdef GC

/*
0x00000000 	0x017fffff 	24MB 	Physical address of the RAM
0x80000000 	0x817fffff 	24MB 	Logical address of the RAM, cached
0xC0000000 	0xC17fffff 	24MB 	Logical address of the RAM, not cached
0xc8000000 	  	2MB 	Embedded Framebuffer (EFB)
0xCC000000 	  	  	Hardware registers
0xCC000000 	  	  	CP - Command Processor
0xCC001000 	  	  	PE - Pixel Engine
0xCC002000 	  	  	VI - Video Interface
0xCC003000 	  	  	PI - Processor Interface (Interrupt Interface)
0xCC004000 	  	  	MI - Memory Interface
0xCC005000 	  	  	AI - Audio Interface
0xCC006000 	  	  	DI - DVD Interface
0xCC006400 	  	  	SI - Serial Interface
0xCC006800 	  	  	EXI - External Interface
0xCC006C00 	  	  	Streaming Interface
0xCC008000 	  	  	GX FIFO (Graphic display lists)
0xe0000000 	0xe0003fff 	16k 	L2 Cache
0xfff00000 	  	1MB 	IPL (mapped here at bootup)



*/

#include "AURAE/AURAE.h"

#define RW_REGISTER_U16(REG) 	*((volatile u16 *)(REG))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#define DEFAULT_FIFO_SIZE	(256*1024)

static void *frameBuffer[2] = { NULL, NULL};
GXRModeObj *rmode;

static vu16* const viReg = (u16*)0xCC002000;

void printRegs()
{
	vu16* const _viReg = viReg;
	printf("\n 0/7 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[0],_viReg[1],_viReg[2],_viReg[3],_viReg[4],_viReg[5],_viReg[6],_viReg[7]);
		printf("\n 8/15 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[8],_viReg[9],_viReg[10],_viReg[11],_viReg[12],_viReg[13],_viReg[14],_viReg[15]);
		printf("\n 16/23 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[16],_viReg[17],_viReg[18],_viReg[19],_viReg[20],_viReg[21],_viReg[22],_viReg[23]);
		printf("\n 24/31 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[24],_viReg[25],_viReg[26],_viReg[27],_viReg[28],_viReg[29],_viReg[30],_viReg[31]);
		printf("\n 32/39 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[32],_viReg[33],_viReg[34],_viReg[35],_viReg[36],_viReg[37],_viReg[38],_viReg[39]);
		printf("\n 40/47 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[40],_viReg[41],_viReg[42],_viReg[43],_viReg[44],_viReg[45],_viReg[46],_viReg[47]);
		printf("\n 48/55 : %04x %04x %04x %04x %04x %04x %04x %04x\n",_viReg[48],_viReg[49],_viReg[50],_viReg[51],_viReg[52],_viReg[53],_viReg[54],_viReg[55]);
		printf("\n 56/59 : %04x %04x %04x %04x\n--------------------------\n",_viReg[56],_viReg[57],_viReg[58],_viReg[59]);

}




WGPipe* const wgPipe2 = (WGPipe*)0xCC008000;


void GX2_Begin(u8 primitve,u8 vtxfmt,u16 vtxcnt)
{
	u8 reg = primitve|(vtxfmt&7);

	wgPipe2->U8 = reg;
	wgPipe2->U16 = vtxcnt;
}

static inline void GX2_Position3f32(f32 x,f32 y,f32 z)
{
	wgPipe2->F32 = x;
	wgPipe2->F32 = y;
	wgPipe2->F32 = z;
}

static inline void GX2_Color3f32(f32 r, f32 g, f32 b)
{
	wgPipe2->U8 = (u8)(r * 255.0);
	wgPipe2->U8 = (u8)(g * 255.0);
	wgPipe2->U8 = (u8)(b * 255.0);
}
#define _SHIFTL(v, s, w)	\
	((u32) (((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w)	\
	((u32)(((u32)(v) >> (s)) & ((0x01 << (w)) - 1)))

#define GX_LOAD_XF_REGS(x, n)			\
	do {								\
		wgPipe2->U8 = 0x10;				\
		asm volatile ("" ::: "memory" ); \
		wgPipe2->U32 = (u32)(((((n)&0xffff)-1)<<16)|((x)&0xffff));				\
		asm volatile ("" ::: "memory" ); \
	} while(0)

static inline void WriteMtxPS4x3(register Mtx mt,register void *wgpipe)
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
		  : "b"(mt), "b"(wgpipe)
		  : "memory"
	);
}

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

	GX_LOAD_XF_REGS(0x101a,6);
	wgPipe->F32 = x0;
	wgPipe->F32 = y0;
	wgPipe->F32 = z;
	wgPipe->F32 = x1;
	wgPipe->F32 = y1;
	wgPipe->F32 = f;
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
	wgPipe->F32 = tmp[0];
	wgPipe->F32 = tmp[1];
	wgPipe->F32 = tmp[2];
	wgPipe->F32 = tmp[3];
	wgPipe->F32 = tmp[4];
	wgPipe->F32 = tmp[5];
	wgPipe->F32 = tmp[6];
}
//---------------------------------------------------------------------------------
int game4(void)
{
//---------------------------------------------------------------------------------
	f32 yscale;

	u32 xfbHeight;

	Mtx view;
	Mtx44 perspective;
	Mtx model, modelview;

	u32	fb = 0; 	// initial framebuffer index
	GXColor background = {0, 0, 0, 0xff};

	unsigned int reg = 0;

	printRegs();

	// init the vi.
	VIDEO_Init();
	//WPAD_Init();

	printRegs();
	rmode = VIDEO_GetPreferredMode(NULL);

	// allocate 2 framebuffers for double buffering
	frameBuffer[0] = malloc(800*800*4);

	VIDEO_Configure(rmode);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// setup the fifo and then init the flipper
	void *gp_fifo = NULL;
	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);

	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GX_SetCopyClear(background, 0x00ffffff);

	// other gx setup
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	GX_SetCullMode(GX_CULL_NONE);
	GX_SetDispCopyGamma(GX_GM_1_0);


	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
 	GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);




	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
	f32 w = rmode->viWidth;
	f32 h = rmode->viHeight;
	guPerspective(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GX2_LoadProjectionMtx(perspective, GX_PERSPECTIVE);


	while(1)
	{

		//WPAD_ScanPads();

		//if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

		// do this before drawing
		GX2_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);

		guMtxIdentity(model);
		guMtxTransApply(model, model, -1.5f,0.0f,-6.0f);

		GX2_LoadPosMtxImm(model, GX_PNMTX0);

		GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
			GX2_Position3f32( 0.0f, 1.0f, 0.0f);		// Top
			GX2_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
			GX2_Position3f32(-1.0f,-1.0f, 0.0f);	// Bottom Left
			GX2_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
			GX2_Position3f32( 1.0f,-1.0f, 0.0f);	// Bottom Right
			GX2_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
		//GX_End();


		// do this stuff after drawing
		GX_DrawDone();

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



