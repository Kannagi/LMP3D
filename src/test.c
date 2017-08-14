/*
Test Dreamcast pvr lib

conclusion : pvr lib is bad u_u



*/

#ifdef DREAMCAST

#include <GL/gl.h>
#include <GL/glu.h>

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
#include <png/png.h>
#include <dc/matrix3d.h>
#include <dc/matrix.h>

#include <plx/context.h>
#include <plx/prim.h>


static pvr_poly_cxt_t  cxt;
static pvr_poly_hdr_t  poly;

void DC_Init()
{

	pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
	pvr_poly_compile(&poly, &cxt);
}

void DC_BlendFunc(int blend)
{
	cxt.blend.src = blend;
	cxt.blend.dst = blend;
}

/*
void DC_BindTexture(DC_Texture texture)
{
	cxt.txr.env = PVR_TXRENV_MODULATE;

	cxt.txr.enable = PVR_TEXTURE_ENABLE;
	cxt.txr.filter = PVR_FILTER_BILINEAR;

	cxt.txr.alpha = PVR_TXRALPHA_DISABLE;
	cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
	cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;

	cxt.txr.uv_flip = PVR_UVFLIP_NONE;
	cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;

	cxt.txr.format = texture.format;
	cxt.txr.base = texture.id;
	cxt.txr.width = texture.w;
	cxt.txr.height = texture.h;

	pvr_poly_compile(&poly, &cxt);
    pvr_prim(&poly, sizeof(pvr_poly_hdr_t));
}
*/
#define DC_ClearColor(_dc_r,_dc_g,_dc_b) pvr_set_bg_color(_dc_r, _dc_g, _dc_b);

void DC_BeginFrame()
{
	pvr_wait_ready();
    pvr_scene_begin();

    pvr_list_begin(PVR_LIST_OP_POLY);

    pvr_poly_compile(&poly, &cxt);
    pvr_prim(&poly, sizeof(pvr_poly_hdr_t));
}

void DC_FinishFrame()
{
	pvr_list_finish();
    pvr_scene_finish();
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

	pvr_prim(&verts, sizeof(pvr_vertex_t));
}

int pvr_init_defaults2()
{
    pvr_init_params_t params = {
        // Enable opaque and translucent polygons with size 16
        { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0 },

        // Vertex buffer size 512K
        512 * 1024,

        // No DMA
        0,

        // No FSAA
        0
    };

    return pvr_init(&params);
}

int main2()
{
    maple_device_t *cont;
    cont_state_t *state;

    pvr_init_defaults2();
    DC_Init();
	DC_ClearColor(0.5,0.5,0.5);

	DC_BeginFrame();

		//DC_BindTexture(texture);

		DC_Drawprim(120 - 50,120 + 50,1, 0,0,0xFFFF0000,0);
		DC_Drawprim(120 - 50,120 - 50,1, 0,1,0xFFFFFFFF,0);
		DC_Drawprim(120 + 50,120 + 50,1, 1,1,0xFFFFFFFF,1);

		//DC_BindTexture(t2);

		DC_Drawprim(200-50,170 + 50,1,   0,0,0xFFFFFFFF,0);
		DC_Drawprim(200 - 50,170 - 50,1, 0,1,0xFFFFFFFF,0);
		DC_Drawprim(200 + 50,170 + 50,1, 1,0,0xFFFFFFFF,1);


	DC_FinishFrame();



    while(1)
    {

        cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        state = (cont_state_t *)maple_dev_status(cont);

        if(state->buttons & CONT_START)
            break;
    }

    return 0;
}



#endif
