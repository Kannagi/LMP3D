/*
Test Dreamcast pvr lib




*/
#include <stdio.h>
#include <stdlib.h>
#ifdef DREAMCAST
#include <time.h>



#include <kos.h>



static pvr_poly_cxt_t  cxt;
static pvr_poly_hdr_t  poly __attribute__((aligned(32)));


#define DC_ClearColor(_dc_r,_dc_g,_dc_b) pvr_set_bg_color2(_dc_r, _dc_g, _dc_b);
#include "LMP3D/DC/DC.h"
#define u32 unsigned int
#define TA_PARAMETER_CONTROL_WORD(para,group,obj) \
							((u32)obj   <<   0  ) | \
							((u32)group <<  16  ) | \
							((u32)para  <<  24  )
int vcnt = 0;
void wait_bovp2()
{
	vcnt = 0;
	RW_REGISTER_U32(0xA05f6900) = 0x08;
	while (!(RW_REGISTER_U32(0xA05f6900) & 0x08)) vcnt++;
}

void ta_commit_list(void *src)
{
	unsigned int *s = (unsigned int *)src;
	unsigned int *d = UNCACHED_P4(0);

	asm("pref @%0" : : "r"(s + 8));
	d[0] = *s++;
	d[1] = *s++;
	d[2] = *s++;
	d[3] = *s++;
	d[4] = *s++;
	d[5] = *s++;
	d[6] = *s++;
	d[7] = *s++;

	asm("pref @%0" : : "r" (d));

	d = (unsigned int *)0xe0000000;
    d[0] = d[8] = 0;

}

void DC_Drawprim0(int x,int y,int z,float u,float v,uint32 argb,int end)
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

	ta_commit_list(&verts);
}

void *currentBuffer;
void DC_Drawprim1(float x,float y,float z,float u,float v,uint32 argb,int end)
{
	u32 flags;

	if(end == 0) flags = PVR_CMD_VERTEX;
	else flags = PVR_CMD_VERTEX_EOL;

	*((u32*)currentBuffer+0) = flags;

	*((float*)currentBuffer+1) = x;
	*((float*)currentBuffer+2) = y;
	*((float*)currentBuffer+3) = z;
	*((float*)currentBuffer+4) = u;
	*((float*)currentBuffer+5) = v;
	*((u32*)currentBuffer+6) = argb;
	*((u32*)currentBuffer+7) = argb;

	currentBuffer += 32;
}

void DC_Init()
{

	pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
	pvr_poly_compile(&poly, &cxt);

	poly.cmd |= 0x0;
}

void DC_BeginFrame()
{
    ta_commit_list(&poly);
}

void DC_FinishFrame()
{
    pvr_scene_finish3();
}



int main2()
{
    maple_device_t *cont;
    cont_state_t *state;
/*
	vid_init(DM_640x480, PM_RGB888);
	vid_init(DM_640x480, PM_RGB888);*/
    pvr_init_defaults2();

    DC_Init();
	DC_ClearColor(0.5,0.5,0.5);

	QACR0 = 0x10;
	QACR1 = 0x10;

	int i=0,ok = 1,tmpx=0,tmpy=0;

	float varray[32*5] __attribute__((aligned(32)));
	void *vertex = varray;

	RW_REGISTER_U32(0xFFA00040) = 0x8201;

	int type = 1;

	FILE *file = fopen("/sd/DATA","rb");

	int y =0;
	if(file != NULL)
	{
		y += 200;
		printf("\nOPEN\n");
		fclose(file);
	}else
		printf("\nNO OPEN %d\n",PM_RGB888);


    while(1)
    {

    	DC_BeginFrame();

		for(i = 0;i <ok;i++)
		{
			tmpx = (i%600)+(type*-16);
			tmpy = (i/600)*80;
			currentBuffer = UNCACHED_P2(vertex);
			if(type == 0)
			{
				DC_Drawprim1(tmpx+100-50,tmpy+70 + 50,100,   0,0,0xFFFFFFFF,0);
				DC_Drawprim1(tmpx+100 - 50,tmpy+70 - 50,100, 0,1,0xFFFFFFFF,0);
				DC_Drawprim1(tmpx+100 + 50,tmpy+70 + 50,100, 1,0,0xFFFFFFFF,1);

				//dcache_flush_range((ptr_t)vertex, (ptr_t)(currentBuffer-vertex) );
				while(!pvr_dma_ready());
				pvr_dma_load_ta(vertex,32*3,0,NULL,0);
				while(!pvr_dma_ready());
			}else
			{
				DC_Drawprim0(tmpx+100-50,tmpy+70 + 50,1,   0,0,0xFFFFFFFF,0);
				DC_Drawprim0(tmpx+100 - 50,tmpy+70 - 50,1, 0,1,0xFFFFFFFF,0);
				DC_Drawprim0(tmpx+100 + 50,tmpy+70 + 50,1, 1,0,0xFFFFFFFF,1);
			}
		}

		DC_FinishFrame();

		cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        state = (cont_state_t *)maple_dev_status(cont);

        if(state->buttons & CONT_A) type = 1;
        if(state->buttons & CONT_B) type = 0;

        if(state->buttons & CONT_X) ok+=4;
		if(state->buttons & CONT_Y) ok-=4;

        if(state->buttons & CONT_START)
            break;

		pvr_int_handler5();
		wait_bovp2();
    }

    return 0;
}




#endif
