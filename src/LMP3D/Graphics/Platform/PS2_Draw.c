#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2
#include <kernel.h>

#include "LMP3D/LMP3D.h"


#include "LMP3D/Platform/PS2.h"

LMP3D_Draw2D(LMP3D_Texture *texture,Vector3 position,int flip)
{

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




#endif


