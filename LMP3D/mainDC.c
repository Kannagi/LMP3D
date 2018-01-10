/*
Test Dreamcast pvr lib

conclusion : pvr lib is bad u_u



*/

#ifdef DREAMCAST

/*

int main()
{
    //Init Dreamcast
    pvr_init_defaults();

    glKosInit();

    glClearColor(0.1f, 0.5f, 0.5f, 0.5f);


    printf("Hello world ! \n");

    while(1)
    {
        glKosBeginFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glKosFinishFrame();
    }
    return 0;
}

*/


#include <kos.h>

static pvr_poly_cxt_t  cxt;
static pvr_poly_hdr_t  poly;

#define DC_ClearColor(_dc_r,_dc_g,_dc_b) pvr_set_bg_color2(_dc_r, _dc_g, _dc_b);

#define UNCACHED_P2(x) ( (void*) ( (unsigned int)(x) | (0xA0000000) ) )
#define CACHED_P3(x) ( (void*) ( (unsigned int)(x) | (0xC0000000) ) )
#define UNCACHED_P4(x) ( (void*) ( (unsigned int)(x) | (0xE0000000) ) )

#include "DC.h"
void DC_BeginFrame()
{

    pvr_scene_begin2();

    sq_cpy2((void *)PVR_TA_INPUT, &poly, sizeof(pvr_poly_hdr_t));
}

void DC_FinishFrame()
{
    pvr_scene_finish2();
}


void DC_Drawprim(int x,int y,int z,float u,float v,uint32 argb,int end)
{
	pvr_vertex_t verts;

	if(end == 0) verts.flags = PVR_CMD_VERTEX;
	else verts.flags = PVR_CMD_VERTEX_EOL;
	verts.x = x;
	verts.y = y;
	verts.z = z;
	verts.u = u;
	verts.v = v;
	verts.argb = argb;
	verts.oargb = argb;

	sq_cpy2((void *)0, &verts, sizeof(pvr_vertex_t));



}
#define UNCACHED_P4(x) ( (void*) ( (unsigned int)(x) | (0xE0000000) ) )
void DC_DMA()
{
	int dma[8*3]__attribute__((aligned(32)));
	int i,l;

	//int *dma = UNCACHED_P4(packdma);


	for(i = 0;i < 3;i++)
	{
		l = i*8;
		if(i == 0)
		{
			dma[l+0] = PVR_CMD_VERTEX;
			dma[l+1] = 250;
			dma[l+2] = 300;
			dma[l+3] = 1;

			dma[l+4] = 0;
			dma[l+5] = 0;
		}

		if(i == 1)
		{
			dma[l+0] = PVR_CMD_VERTEX;
			dma[l+1] = 250;
			dma[l+2] = 400;
			dma[l+3] = 1;

			dma[l+4] = 1;
			dma[l+5] = 0;
		}

		if(i == 2)
		{
			dma[l+0] = PVR_CMD_VERTEX_EOL;
			dma[l+1] = 400;
			dma[l+2] = 400;
			dma[l+3] = 1;

			dma[l+4] = 1;
			dma[l+5] = 1;
		}

		dma[l+6] = 0xFFFFFFFF;
		dma[l+7] = 0xFFFFFFFF;
	}

	RW_REGISTER_U32(SB_PDSTAP) = DC_SET_PDSTAP(0xE0000000>>5);
	RW_REGISTER_U32(SB_PDSTAR) = DC_SET_PDSTAR(dma>>5);
	RW_REGISTER_U32(SB_PDLEN)  = DC_SET_PDLEN(0x3);
	RW_REGISTER_U32(SB_PDDIR)  = DC_SET_PDDIR(1);
	RW_REGISTER_U32(SB_PDTSEL) = DC_SET_PDTSEL(0);
	RW_REGISTER_U32(SB_PDST)   = DC_SET_PDST(1);


	while(RW_REGISTER_U32(SB_PDST) == 1);


}



void wait_bovp()
{
  volatile unsigned int *vblreg = (volatile unsigned int *)(void*)0xA05f6900;
  *vblreg = 0x08;
  while (!(*vblreg & 0x08)) ;
  //*vblreg = 0x08;
}



float trans_coords[8][3];

#define set_trans(_i,_x,_y,_z) \
			trans_coords[_i][0] = _x;  \
			trans_coords[_i][1] = _y;  \
			trans_coords[_i][2] = _z;
//video multiple 32
int main2()
{
    maple_device_t *cont;
    cont_state_t *state;

    pvr_init_defaults2();

	RW_REGISTER_U32(SB_PDDEN)  = DC_SET_PDDEN(1);

	int tmpx = 0;
    while(1)
    {


    	pvr_poly_cxt_col2(&cxt, PVR_LIST_OP_POLY);
		pvr_poly_compile2(&poly, &cxt);



		DC_ClearColor(0.5,0.5,0.5);

		DC_BeginFrame();

			//DC_BindTexture(texture);

			DC_Drawprim(tmpx+120 - 50,120 + 50,1, 0,0,0xFFFF0000,0);
			DC_Drawprim(tmpx+120 - 50,120 - 50,1, 0,1,0xFFFFFFFF,0);
			DC_Drawprim(tmpx+120 + 50,120 + 50,1, 1,1,0xFFFFFFFF,1);

			//DC_BindTexture(t2);

			DC_Drawprim(200-50,170 + 50,1,   0,0,0xFFFFFFFF,0);
			DC_Drawprim(200 - 50,170 - 50,1, 0,1,0xFFFFFFFF,0);
			DC_Drawprim(200 + 50,170 + 50,1, 1,0,0xFFFFFFFF,1);


		DC_FinishFrame();
		//------------------------------


        cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        state = (cont_state_t *)maple_dev_status(cont);

        if(state->buttons & CONT_START)
            break;


		if(state->buttons & CONT_A) tmpx++;
		if(state->buttons & CONT_B) tmpx--;


		wait_bovp();
    }

    return 0;
}



#endif
