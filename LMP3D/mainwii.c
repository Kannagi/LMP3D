
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

void GX2_Begin(u8 primitve,u8 vtxfmt,u16 vtxcnt)
{
	u8 reg = primitve|(vtxfmt&7);

	RW_REGISTER_U8(GX_FIFO) = reg;
	RW_REGISTER_U16(GX_FIFO) = vtxcnt;
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

	float rtri = 0.0f , rquad = 0.0f;

	u32	fb = 0; 	// initial framebuffer index
	GXColor background = {0, 0, 0, 0xff};


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

	// clears the bg to color and clears the z buffer
	GX_SetCopyClear(background, 0x00ffffff);

	// other gx setup
	GX2_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(frameBuffer[fb],GX_TRUE);
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

	guVector Yaxis = {0,1,0};
	guVector Xaxis = {1,0,0};

	int axisx = 0,i,n;
	float x = 0;

	fatInitDefault();

	FILE *file = fopen("zack.bcm","rb");

	if(file != NULL)
	{
		printf("\nOPEN\n");
		fclose(file);
	}else
		printf("\nNO OPEN\n");

	LMP3D_Model * model2;

	//model2 = LMP3D_Load_Model_bcm("zack.bcm");

	float vec[3];

	while(1) {

		WPAD_ScanPads();

		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

		//axisx = (RW_REGISTER_U32(0xCC006404)&0xFF00)>>8;
		//x += (float)(axisx-0x80)*0.001;

		//printf("\n state : %x %x\n",RW_REGISTER_U32(0xCC006404),sizeof(unsigned long));

		// do this before drawing
		GX2_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);

		guMtxIdentity(model);
		guMtxScale(model, 1.1f,1.1f,1.1f);
		//guMtxRotAxisDeg(model, &Yaxis, rtri);
		guMtxTransApply(model, model, x-1.5f,0.0f,-6.0f);

		guMtxConcat(view,model,modelview);
		// load the modelview matrix into matrix memory
		GX2_LoadPosMtxImm(modelview, GX_PNMTX0);
/*
		for(i = 0;i < 30;i+=3)
		{
			GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);

			n = model2->f[i+0];
			vec[0] = model2->v[i+0];


			n = model2->f[i+1];
			vec[1] = model2->v[i+1];

			n = model2->f[i+2];
			vec[2] = model2->v[i+2];

			printf("\n%f %f %f\n",vec[0],vec[1],vec[2]);

			GX2_Position3f32( vec[0],vec[1],vec[2]);		// Top
			GX2_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red



			n = model2->f[i+0];
			vec[0] = model2->v[n+0];

			n = model2->f[i+1];
			vec[1] = model2->v[n+1];

			n = model2->f[i+2];
			vec[2] = model2->v[n+2];

			GX2_Position3f32( vec[0],vec[1],vec[2]);		// Top
			GX2_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green




			n = model2->f[i+0];
			vec[0] = model2->v[n+0];

			n = model2->f[i+1];
			vec[1] = model2->v[n+1];

			n = model2->f[i+2];
			vec[2] = model2->v[n+2];

			GX2_Position3f32( vec[0],vec[1],vec[2]);		// Top
			GX2_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue

		}
*/
		GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
			GX2_Position3f32( 0.0f, 1.0f, 0.0f);		// Top
			GX2_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
			GX2_Position3f32(-1.0f,-1.0f, 0.0f);	// Bottom Left
			GX2_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
			GX2_Position3f32( 1.0f,-1.0f, 0.0f);	// Bottom Right
			GX2_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue



		guMtxIdentity(model);
		guMtxRotAxisDeg(model, &Xaxis, rquad);
		guMtxTransApply(model, model, 1.5f,0.0f,-6.0f);
		guMtxConcat(view,model,modelview);
		// load the modelview matrix into matrix memory
		GX2_LoadPosMtxImm(modelview, GX_PNMTX0);

		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);			// Draw A Quad
			GX2_Position3f32(-1.0f, 1.0f, 0.0f);	// Top Left
			GX2_Color3f32(0.5f,0.5f,1.0f);			// Set The Color To Blue
			GX2_Position3f32( 1.0f, 1.0f, 0.0f);		// Top Right
			GX2_Color3f32(0.5f,0.5f,1.0f);			// Set The Color To Blue
			GX2_Position3f32( 1.0f,-1.0f, 0.0f);	// Bottom Right
			GX2_Color3f32(0.5f,0.5f,1.0f);			// Set The Color To Blue
			GX2_Position3f32(-1.0f,-1.0f, 0.0f);	// Bottom Left
			GX2_Color3f32(0.5f,0.5f,1.0f);			// Set The Color To Blue

		// do this stuff after drawing
		GX_DrawDone();

		fb ^= 1;		// flip framebuffer
		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);

		VIDEO_Flush();

		VIDEO_WaitVSync();

		rtri+=0.2f;				// Increase The Rotation Variable For The Triangle ( NEW )
		rquad-=0.15f;			// Decrease The Rotation Variable For The Quad     ( NEW )

	}
	return 0;
}




#endif



