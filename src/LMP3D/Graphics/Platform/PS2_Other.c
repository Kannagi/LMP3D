#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "LMP3D/LMP3D.h"


#include "LMP3D/Platform/PS2_Register.h"
int PS2_vram_allocate(int width, int height, int psm);
/*

	LMP3D_VIDEO_MODE0 640x256 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE1 640x240 interlaced 50 HZ (PAL)

	LMP3D_VIDEO_MODE2 640x512 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE3 640x480 interlaced 50 HZ (PAL)

	LMP3D_VIDEO_MODE4 320x240 interlaced 60 HZ (NTSC)
	LMP3D_VIDEO_MODE5 320x240 interlaced 50 HZ (PAL)

*/

void PS2_Init_Graphic(int width, int height, int psm,int fbaddr)
{

	// Reset GS.
	RW_REGISTER_U64(GS_CSR) = (unsigned long)1<<9;

	// Clear GS CSR.
	RW_REGISTER_U64(GS_CSR) = 0;


	// Unmask GS VSYNC Interrupt.
	GsPutIMR(0x00007F00);

	// Set the requested mode.
	SetGsCrt(0, 2, 0);

	//GS_SET_SMODE1(4,32,1,0,2,0,1,1,0,0,4,0,0,0,0,0,1,1,1,1,0);


	// Set black background
	RW_REGISTER_U64(GS_BGCOLOR) = GS_SET_BGCOLOR(0,0,0);

	// Set the screen up
	RW_REGISTER_U64(GS_DISPLAY1) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),(height-1));
	RW_REGISTER_U64(GS_DISPLAY2) = GS_SET_DISPLAYX(652,26,3,0,(2560-1),(height-1));


	RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(fbaddr>>11,width>>6,psm,0,0);
	RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(fbaddr>>11,width>>6,psm,0,0);


	RW_REGISTER_U64(GS_PMODE) = GS_SET_PMODE(1,1,1, 1, 0, 0, 0x70);

}

void PS2_Setup_Graphic(LMP3D_Buffer buffer)
{

	int i = 0;
	u64 *gif;
	u64 gif_array[64] __attribute__((aligned(64)));

	gif =  UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(5,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_FRAMEX(buffer.faddress1>>11,buffer.width>>6,buffer.fpsm,0);
	gif[i++] = 0x4C;

	gif[i++] = GS_SET_ZBUFX(buffer.zaddress>>11,buffer.zpsm,0);
	gif[i++] = 0x4E;


	gif[i++] = GS_SET_XYOFFSET((2048-320)<<4,(2048-256)<<4);
	gif[i++] = 0x18;


	gif[i++] = GS_SET_SCISSORX(0,buffer.width-1,0,buffer.height-1);
	gif[i++] = 0x40;


	gif[i++] = GS_SET_PRMODECONT(1);
	gif[i++] = 0x1A;


	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & ((unsigned int)1<<8));

}
void PS2_VU_Init();

LMP3D_Buffer LMP3D_Init_Graphic(int width, int height)
{
	LMP3D_Buffer buffer;
	buffer.width = 640;
	buffer.height = 256;
	buffer.fpsm = GS_PSMCT32;
	buffer.zpsm = GS_PSMZ16S;
	buffer.faddress1 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.faddress2 = PS2_vram_allocate(buffer.width,buffer.height, buffer.fpsm);
	buffer.zaddress  = PS2_vram_allocate(buffer.width,buffer.height, buffer.zpsm);

	buffer.switchBuffer = 0;


	PS2_Init_Graphic(buffer.width ,buffer.height ,buffer.fpsm ,buffer.faddress1);
	PS2_Setup_Graphic(buffer);

	PS2_VU_Init();

	return buffer;
}



void LMP3D_Clear()
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[64] __attribute__((aligned(64)));

	gif = UNCACHED_SEG(gif_array);

	u64 primv = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT, GS_TME_TEXTURE_OFF, GS_FGE_FOGGING_OFF, GS_ABE_ALPHA_OFF,
							GS_AA1_ANTIALIASING_OFF, GS_FST_STQ, GS_CTXT_0, GS_FIX_0);
	gif[i++] = GS_SET_GIFTAG(6,1,1,primv,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = GS_SET_TEST(DRAW_DISABLE,ATEST_METHOD_EQUAL,0x80,ATEST_KEEP_FRAMEBUFFER,DRAW_DISABLE,DRAW_DISABLE,DRAW_ENABLE,ZTEST_METHOD_ALLPASS);
	gif[i++] = 0x47;

	gif[i++] = GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x00, 0);
	gif[i++] = GS_REG_RGBAQ;


	gif[i++] = GS_SET_XYZ((2048-320)<<4, (2048-256)<<4, 2048<<4);
	gif[i++] = GS_REG_XYZ2;

	gif[i++] = GS_SET_XYZ((2048+320)<<4, (2048+256)<<4, 2048<<4);
	gif[i++] = GS_REG_XYZ2;


	gif[i++] = GS_SET_TEST(DRAW_ENABLE,ATEST_METHOD_GREATER_EQUAL,0x80,ATEST_KEEP_FRAMEBUFFER,DRAW_DISABLE,DRAW_DISABLE,DRAW_ENABLE,ZTEST_METHOD_GREATER_EQUAL);
	gif[i++] = 0x47;


	gif[i++] = 1;
	gif[i++] = 0X61;


	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);



	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);
}

void GsDrawSprite(int posx,int posy,int tile)
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[128] __attribute__((aligned(64)));


	gif = UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(1,0,0,0,0,1);
	gif[i++] = GS_REG_AD;


	gif[i++] = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT,GS_TME_TEXTURE_ON,GS_FGE_FOGGING_OFF,
							GS_ABE_ALPHA_OFF,GS_AA1_ANTIALIASING_OFF, GS_FST_UV, GS_CTXT_0, GS_FIX_0);
	gif[i++] = GS_REG_PRIM;

	gif[i++] = GS_SET_GIFTAG(2,0,0,0,1,2);
	gif[i++] = 0x0053;


	int tx,ty;
	tx = (tile%16)<<8;
	ty = (tile/16)<<8;


	gif[i++] = GS_SET_UV(tx,ty);
	gif[i++] = GS_SET_XYZ((2048-320+posx)<<4, (2048-256+posy)<<4, 4048<<4);

	gif[i++] = GS_SET_UV((tx+0X100),ty+0X100);
	gif[i++] = GS_SET_XYZ((2048-320+posx+16)<<4, (2048-256+posy+16)<<4, 4048<<4);


	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = 1;
	gif[i++] = 0X61;


	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);



}

void GsDrawSpriteTest(int posx,int posy)
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[128] __attribute__((aligned(64)));


	gif = UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(1,0,0,0,0,1);
	gif[i++] = GS_REG_AD;


	gif[i++] = GS_SET_PRIM(GS_PRIM_SPRITE,GS_IIP_FLAT,GS_TME_TEXTURE_ON,GS_FGE_FOGGING_OFF,
							GS_ABE_ALPHA_OFF,GS_AA1_ANTIALIASING_OFF, GS_FST_UV, GS_CTXT_0, GS_FIX_0);
	gif[i++] = GS_REG_PRIM;

	gif[i++] = GS_SET_GIFTAG(2,0,0,0,1,2);
	gif[i++] = 0x0053;






	gif[i++] = GS_SET_UV(0,0);
	gif[i++] = GS_SET_XYZ((2048-320+posx)<<4, (2048-256+posy)<<4, 4048<<4);

	gif[i++] = GS_SET_UV(256<<4,256<<4);
	gif[i++] = GS_SET_XYZ((2048-320+posx+256)<<4, (2048-256+posy+256)<<4, 4048<<4);


	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	gif[i++] = 1;
	gif[i++] = 0X61;


	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);


	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);



}


void bitmap_font2(unsigned char *texte,int tx,int ty)
{
    int i = 0;
    int x = tx;


    while(texte[i] != 0)
    {


        if(texte[i] != ' ' && texte[i] != '\n')
        GsDrawSprite(tx,ty,texte[i]);

        tx += 16;

        if(texte[i] == '\n')
        {
            tx = x;
            ty += 16;
        }

        i++;
    }
}



void PS2_MatrixIdentity(float* matrix)
{
	matrix[(0<<2)+0] = 1;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = 1;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 0;
	matrix[(2<<2)+1] = 0;
	matrix[(2<<2)+2] = 1;
	matrix[(2<<2)+3] = 0;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = 0;
	matrix[(3<<2)+3] = 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PS2_MatrixMultiply(float* dest,const float* src1,const float* src2)
{
  asm __volatile__(
	//"abs.s vf16,vf17\n"
    "lqc2            vf16,0x00(%1)\n"
    "lqc2            vf17,0x10(%1)\n"
    "lqc2            vf18,0x20(%1)\n"
    "lqc2            vf19,0x30(%1)\n"
    "lqc2            vf20,0x00(%2)\n"
    "lqc2            vf21,0x10(%2)\n"
    "lqc2            vf22,0x20(%2)\n"
    "lqc2            vf23,0x30(%2)\n"
    "vmulax          ACC,vf20,vf16\n"
    "vmadday         ACC,vf21,vf16\n"
    "vmaddaz         ACC,vf22,vf16\n"
    "vmaddw          vf16,vf23,vf16\n"
    "vmulax          ACC,vf20,vf17\n"
    "vmadday         ACC,vf21,vf17\n"
    "vmaddaz         ACC,vf22,vf17\n"
    "vmaddw          vf17,vf23,vf17\n"
    "vmulax          ACC,vf20,vf18\n"
    "vmadday         ACC,vf21,vf18\n"
    "vmaddaz         ACC,vf22,vf18\n"
    "vmaddw          vf18,vf23,vf18\n"
    "vmulax          ACC,vf20,vf19\n"
    "vmadday         ACC,vf21,vf19\n"
    "vmaddaz         ACC,vf22,vf19\n"
    "vmaddw          vf19,vf23,vf19\n"
    "sqc2            vf16,0x00(%0)\n"
    "sqc2            vf17,0x10(%0)\n"
    "sqc2            vf18,0x20(%0)\n"
    "sqc2            vf19,0x30(%0)\n"
    : : "r"(dest), "r"(src1), "r"(src2) : "memory");
}
 float fsin2(float x)
 {
	int q = (x * 6.3661977236758138e-1f) +0.5f; //floor

	float t = x - q * 1.5707963267923333e+00f;
	    t = t - q * 2.5633441515945189e-12f;
	float t2 = t * t;
	if (q & 1) {
		t = ((-1.36058866707554670912e-03f * t2 + 4.16566258031673830195e-02f) * t2 - 4.99998875577611430384e-01f) * t2 + 9.99999980750852337007e-01f;
	} else {
		t = (((-1.94842039748219288187e-04f * t2 + 8.33179571459221545387e-03f) * t2 - 1.66666423796176028737e-01f) * t2 + 9.99999989793669848536e-01f) * t;
	}
	if (q & 2) t = -t;
	return t;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 float fsin2(float x);
void PS2_MatrixRotateY(float* matrix, float angle)
{
  float sinAngle = (float)fsin2(angle);
  float cosAngle = (float)fsin2(angle+1.57 );

  matrix[(0<<2)+0] = cosAngle;
  matrix[(0<<2)+1] = 0;
  matrix[(0<<2)+2] = -sinAngle;
  matrix[(0<<2)+3] = 0;

  matrix[(1<<2)+0] = 0;
  matrix[(1<<2)+1] = 1;
  matrix[(1<<2)+2] = 0;
  matrix[(1<<2)+3] = 0;

  matrix[(2<<2)+0] = sinAngle;
  matrix[(2<<2)+1] = 0;
  matrix[(2<<2)+2] = cosAngle;
  matrix[(2<<2)+3] = 0;

  matrix[(3<<2)+0] = 0;
  matrix[(3<<2)+1] = 0;
  matrix[(3<<2)+2] = 0;
  matrix[(3<<2)+3] = 1;
}

///////////////////////////////////////////////////////////////////////////////
// void floatBuildPitch
///////////////////////////////////////////////////////////////////////////////

void PS2_MatrixRotateX(float* matrix, float angle)
{
  float sinAngle = fsin2( angle );
  float cosAngle = fsin2(angle+1.57 );

  matrix[(0<<2)+0] = 1;
  matrix[(0<<2)+1] = 0;
  matrix[(0<<2)+2] = 0;
  matrix[(0<<2)+3] = 0;

  matrix[(1<<2)+0] = 0;
  matrix[(1<<2)+1] = cosAngle;
  matrix[(1<<2)+2] = sinAngle;
  matrix[(1<<2)+3] = 0;

  matrix[(2<<2)+0] = 0;
  matrix[(2<<2)+1] = -sinAngle;
  matrix[(2<<2)+2] = cosAngle;
  matrix[(2<<2)+3] = 0;

  matrix[(3<<2)+0] = 0;
  matrix[(3<<2)+1] = 0;
  matrix[(3<<2)+2] = 0;
  matrix[(3<<2)+3] = 1;
}


void PS2_MatrixRotateZ(float* matrix, float angle)
{
  float sinAngle = (float)fsin2(angle );
  float cosAngle = (float)fsin2(angle+1.57 );

  matrix[(0<<2)+0] = cosAngle;
  matrix[(0<<2)+1] = sinAngle;
  matrix[(0<<2)+2] = 0;
  matrix[(0<<2)+3] = 0;

  matrix[(1<<2)+0] = -sinAngle;
  matrix[(1<<2)+1] = cosAngle;
  matrix[(1<<2)+2] = 0;
  matrix[(1<<2)+3] = 0;

  matrix[(2<<2)+0] = 0;
  matrix[(2<<2)+1] = 0;
  matrix[(2<<2)+2] = 1;
  matrix[(2<<2)+3] = 0;

  matrix[(3<<2)+0] = 0;
  matrix[(3<<2)+1] = 0;
  matrix[(3<<2)+2] = 0;
  matrix[(3<<2)+3] = 1;
}


void PS2_MatrixTranslate(float* matrix, float x, float y, float z)
{
  matrix[(3<<2)+0] = x;
  matrix[(3<<2)+1] = y;
  matrix[(3<<2)+2] = z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PS2_MatrixScale(float* matrix, float xscale, float yscale, float zscale)
{
  matrix[(0<<2)+0] = xscale;
  matrix[(1<<2)+1] = yscale;
  matrix[(2<<2)+2] = zscale;
  matrix[(3<<2)+3] = 1;
}

void PS2_MatrixProjection(float* matrix)
{

	matrix[(0<<2)+0] = 480;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = 640;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 2048;
	matrix[(2<<2)+1] = 2048-128;
	matrix[(2<<2)+2] = 2048;
	matrix[(2<<2)+3] = 1;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = 10000;
	matrix[(3<<2)+3] = 0.0f;
}

void GsAlpha()
{
	int i = 0;
	unsigned long *gif;
	unsigned long gif_array[64] __attribute__((aligned(64)));

	gif = UNCACHED_SEG(gif_array);


	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = 0x0E;


	gif[i++] = GS_SET_TEST(DRAW_DISABLE,ATEST_METHOD_GREATER_EQUAL,0x80,ATEST_KEEP_FRAMEBUFFER,DRAW_DISABLE,DRAW_DISABLE,DRAW_ENABLE,ZTEST_METHOD_GREATER_EQUAL);
	gif[i++] = 0x47;




	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);



	while( (RW_REGISTER_U32(D2_CHCR)) & 0x100);
}

void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{
	//printf("buf2 %x %x %x\n",buffer->faddress1,buffer->faddress2,buffer->zaddress);
	int i = 0;
	u64 *gif;
	u64 gif_array[64] __attribute__((aligned(64)));

	gif =  UNCACHED_SEG(gif_array);

	gif[i++] = GS_SET_GIFTAG(1,1,0,0,0,1);
	gif[i++] = GS_REG_AD;

	if(buffer->switchBuffer == 0)
	{
		RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(buffer->faddress1,buffer->width>>6,buffer->fpsm,0,0);
		RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(buffer->faddress1>>11,buffer->width>>6,buffer->fpsm,0,0);

		gif[i++] = GS_SET_FRAMEX(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0);
		gif[i++] = GS_REG_FRAME_1;
	}else
	{
		RW_REGISTER_U64(GS_DISPFB1) = GS_SET_DISPFB1(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0,0);
		RW_REGISTER_U64(GS_DISPFB2) = GS_SET_DISPFB2(buffer->faddress2>>11,buffer->width>>6,buffer->fpsm,0,0);

		gif[i++] = GS_SET_FRAMEX(buffer->faddress1>>11,buffer->width>>6,buffer->fpsm,0);
		gif[i++] = GS_REG_FRAME_1;
	}



	RW_REGISTER_U32(D2_MADR) = EE_SET_ADR(gif_array,0);
	RW_REGISTER_U32(D2_QWC ) = i/2;

	RW_REGISTER_U32(D2_CHCR) = EE_SET_CHCR(1,0,0,0,0,1,0);

	buffer->switchBuffer = !buffer->switchBuffer;

	while( (RW_REGISTER_U32(D2_CHCR)) & ((unsigned int)1<<8));


}


#endif

