

#ifdef DREAMCAST

#include <kos.h>

#include "LMP3D/DC/DC.h"

#define PVR_OPB_COUNT   5

typedef struct {
    uint32  vertex, vertex_size;            /* Vertex buffer */
    uint32  opb, opb_size;                  /* Object pointer buffers, size */
    uint32  opb_type[PVR_OPB_COUNT];        /* Object pointer buffers (of each type) */
    uint32  tile_matrix;  /* Tile matrix, size */
} pvr_ta_buffers_t;

// Frame buffers structure: we have two sets of these
typedef struct {
    uint32  frame, frame_size;      // Output frame buffer, size
} pvr_frame_buffers_t;

typedef struct {
    // If this is zero, then this state isn't valid

    // General configuration
    uint32  lists_enabled;              // opb_completed's value when we're ready to render
    uint32  list_reg_mask;              // Active lists register mask
    int     opb_size[PVR_OPB_COUNT];    // opb size flags
    uint32  opb_ind[PVR_OPB_COUNT];     // Individual opb sizes (in bytes)

    // Pipeline state
    int     ta_target;                  // TA buffer we're writing (or DMAing) into
                                        // (^1 == TA buffer we're rendering from)
    int     view_target;                // Frame buffer we're viewing
                                        // (^1 == frame buffer we're rendering to)

    int     list_reg_open;              // Which list is open for registration, if any? (non-DMA only)
    uint32  lists_closed;               // (1 << idx) for each list which the SH4 has lost interest in


    // Memory pointers / buffers
    pvr_ta_buffers_t    ta_buffers[2];      // TA buffers
    pvr_frame_buffers_t frame_buffers[2];   // Frame buffers

    // Screen size / clipping constants
    int     w, h;                       // Screen width, height
    int     tw, th;                     // Screen tile width, height
    uint32  tsize_const;                // Screen tile size constant
    float   zclip;                      // Z clip plane
    uint32  bg_color;                   // Background color in ARGB format




} pvr_state_t;
pvr_state_t pvr_state;

void pvr_set_bg_color2(float r, float g, float b) {
    int ir, ig, ib;

    ir = (int)(255 * r);
    ig = (int)(255 * g);
    ib = (int)(255 * b);

    pvr_state.bg_color = (ir << 16) | (ig << 8) | (ib << 0);
}

typedef struct pvr_bkg_poly {
    uint32      flags1, flags2;
    uint32      dummy;
    float       x1, y1, z1;
    uint32      argb1;
    float       x2, y2, z2;
    uint32      argb2;
    float       x3, y3, z3;
    uint32      argb3;
} pvr_bkg_poly_t;


void pvr_begin_queued_render2() {
    volatile pvr_ta_buffers_t   * tbuf;
    volatile pvr_frame_buffers_t    * rbuf;
    pvr_bkg_poly_t  bkg;
    uint32      *vrl, *bkgdata;
    uint32      vert_end;
    int     i;

    /* Get the appropriate buffer */
    tbuf = pvr_state.ta_buffers;
    rbuf = pvr_state.frame_buffers + (pvr_state.view_target ^ 1);

    /* Calculate background value for below */
    /* Small side note: during setup, the value is originally
       0x01203000... I'm thinking that the upper word signifies
       the length of the background plane list in dwords
       shifted up by 4. */
    vert_end = 0x01000000 | ((PVR_GET(PVR_TA_VERTBUF_POS) - tbuf->vertex) << 1);

    /* Throw the background data on the end of the TA's list */
    bkg.flags1 = 0x90800000;    /* These are from libdream.. ought to figure out */
    bkg.flags2 = 0x20800440;    /*   what they mean for sure... heh =) */
    bkg.dummy = 0;
    bkg.x1 =   0.0f;
    bkg.y1 = 480.0f;
    bkg.z1 = 0.2f;
    bkg.argb1 = pvr_state.bg_color;
    bkg.x2 =   0.0f;
    bkg.y2 =   0.0f;
    bkg.z2 = 0.2f;
    bkg.argb2 = pvr_state.bg_color;
    bkg.x3 = 640.0f;
    bkg.y3 = 480.0f;
    bkg.z3 = 0.2f;
    bkg.argb3 = pvr_state.bg_color;
    bkgdata = (uint32 *)&bkg;
    vrl = (uint32*)(PVR_RAM_BASE | PVR_GET(PVR_TA_VERTBUF_POS));

    for(i = 0; i < 0x10; i++)
        vrl[i] = bkgdata[i];

    vrl[0x11] = 0;

    /* Reset the ISP/TSP, just in case */
    RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_ISPTSP;
    RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_NONE;

    /* Finish up rendering the current frame (into the other buffer) */
	RW_REGISTER_U32(RENDER_POLYBASE+DC_P4) = tbuf->vertex;

	RW_REGISTER_U32(RENDER_ADDR1+DC_P4) = rbuf->frame;
	RW_REGISTER_U32(RENDER_BGPLANE+DC_P4) = vert_end; /* Bkg plane location */


    RW_REGISTER_U32(RENDER_START+DC_P4) = PVR_ISP_START_GO;   /* Start render */
}

void pvr_int_handler5()
{
		pvr_state.view_target ^= 1;

        vid_set_start(pvr_state.frame_buffers[pvr_state.view_target].frame);


        pvr_begin_queued_render2();


        volatile pvr_ta_buffers_t *buf;
		buf = pvr_state.ta_buffers;

		RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_TA;
		RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_NONE;

		PVR_SET(PVR_TA_VERTBUF_START,   buf->vertex);
		PVR_SET(PVR_TA_VERTBUF_END, buf->vertex + buf->vertex_size);

		PVR_SET(PVR_TA_INIT,        PVR_TA_INIT_GO);        /* Confirm settings */
		(void)PVR_GET(PVR_TA_INIT);
}


void pvr_allocate_buffers2(pvr_init_params_t *params)
{
    volatile pvr_ta_buffers_t   *buf;
    volatile pvr_frame_buffers_t    *fbuf;
    int i, j;
    uint32  outaddr, polybuf, sconst, polybuf_alloc;

    /* Set screen sizes; pvr_init has ensured that we have a valid mode
       and all that by now, so we can freely dig into the vid_mode
       structure here. */
    pvr_state.w = vid_mode->width;
    pvr_state.h = vid_mode->height;
    pvr_state.tw = pvr_state.w / 32;
    pvr_state.th = pvr_state.h / 32;

    pvr_state.tsize_const = ((pvr_state.th - 1) << 16)
                            | ((pvr_state.tw - 1) << 0);

    /* Set clipping parameters */
    pvr_state.zclip = 0.0001f;

    /* Look at active lists and figure out how much to allocate
       for each poly type */
    polybuf = 0;
    pvr_state.list_reg_mask = 1 << 20;

    for(i = 0; i < PVR_OPB_COUNT; i++) {
        pvr_state.opb_size[i] = params->opb_sizes[i] * 4;   /* in bytes */
        pvr_state.opb_ind[i] = pvr_state.opb_size[i] * pvr_state.tw * pvr_state.th;
        polybuf += pvr_state.opb_ind[i];

        switch(params->opb_sizes[i]) {
            case PVR_BINSIZE_0:
                sconst = 0;
                break;
            case PVR_BINSIZE_8:
                sconst = 1;
                break;
            case PVR_BINSIZE_16:
                sconst = 2;
                break;
            case PVR_BINSIZE_32:
                sconst = 3;
                break;
            default:
                //assert_msg(0, "invalid poly_buf_size");
                sconst = 2;
                break;
        }

        if(sconst > 0) {
            pvr_state.lists_enabled |= (1 << i);
            pvr_state.list_reg_mask |= sconst << (4 * i);
        }
    }

	buf = &pvr_state.ta_buffers[0];
	buf->vertex = 0;
	buf->vertex_size = 0x300000;

	buf->opb = 0x300000;
	buf->opb_size = polybuf;
	polybuf_alloc = buf->opb;
	for(j = 0; j < PVR_OPB_COUNT; j++)
	{
		if(pvr_state.opb_size[j] > 0)
		{
			buf->opb_type[j] = polybuf_alloc;
			polybuf_alloc += pvr_state.opb_ind[j];
		}
		else {
			buf->opb_type[j] = 0x80000000;
		}
	}

	buf->tile_matrix = 0x309600;
	//buf->tile_matrix_size = (18 + 6 * pvr_state.tw * pvr_state.th) * 4;

	//printf("%x\n",polybuf);




	//-------------
	pvr_state.frame_buffers[0].frame = 0x30b280;
	pvr_state.frame_buffers[1].frame = 0x40b280;

}

void pvr_init_tile_matrix2(int which) {
    volatile pvr_ta_buffers_t   *buf;
    int     x, y;
    uint32      *vr;
    volatile int    *opbs;

    vr = (uint32*)PVR_RAM_BASE;
    buf = pvr_state.ta_buffers + which;
    opbs = pvr_state.opb_size;


    /* Header of zeros */
    vr += buf->tile_matrix / 4;


    /* Initial init tile */
    vr[0] = 0x10000000;
    vr[1] = 0x80000000;
    vr[2] = 0x80000000;
    vr[3] = 0x80000000;
    vr[4] = 0x80000000;
    vr[5] = 0x80000000;
    vr += 6;

    /* Now the main tile matrix */


    for(x = 0; x < pvr_state.tw; x++)
    {
        for(y = 0; y < pvr_state.th; y++)
        {
            /* Control word */
            vr[0] = (y << 8) | (x << 2);

            /* Opaque poly buffer */
            vr[1] = buf->opb_type[0] + opbs[0] * pvr_state.tw * y + opbs[0] * x;

            /* Opaque volume mod buffer */
            vr[2] = buf->opb_type[1] + opbs[1] * pvr_state.tw * y + opbs[1] * x;

            /* Translucent poly buffer */
            vr[3] = buf->opb_type[2] + opbs[2] * pvr_state.tw * y + opbs[2] * x;

            /* Translucent volume mod buffer */
            vr[4] = buf->opb_type[3] + opbs[3] * pvr_state.tw * y + opbs[3] * x;

            /* Punch-thru poly buffer */
            vr[5] = buf->opb_type[4] + opbs[4] * pvr_state.tw * y + opbs[4] * x;
            vr += 6;
        }
    }

    vr[-6] |= 1 << 31;

}

/* Fill all tile matrices */
void pvr_init_tile_matrices2() {
    int i = 0;

	pvr_init_tile_matrix2(0);
}
int pvr_init2(pvr_init_params_t *params)
{
    vid_empty();																	//---------------------------------------------

    /* Reset all PVR systems (in case it's still doing something) */
    RW_REGISTER_U32(PVR_RESET) = PVR_RESET_ALL;
    RW_REGISTER_U32(PVR_RESET) = PVR_RESET_NONE;


    /* Start off with a nice empty structure */
    memset((void *)&pvr_state, 0, sizeof(pvr_state));

    /* Everything's clear, do the initial buffer pointer setup */
    pvr_allocate_buffers2(params);																	//---------------------------------------------

    // Initialize tile matrices
    pvr_init_tile_matrices2();																	//---------------------------------------------

    pvr_state.ta_target = 0;
    pvr_state.view_target = 0;

    // Sync all the hardware registers with our pipeline state.
	RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_TA;
	RW_REGISTER_U32(PVRRESET+DC_P4) = PVR_RESET_NONE;

	/* Set buffer pointers */
	RW_REGISTER_U32(TA_TILEBASE+DC_P4) = pvr_state.ta_buffers[0].opb;
	RW_REGISTER_U32(TA_LISTEND +DC_P4) = pvr_state.ta_buffers[0].opb - pvr_state.ta_buffers[0].opb_size;

	RW_REGISTER_U32(TA_POLYBASE+DC_P4) = pvr_state.ta_buffers[0].vertex;
	RW_REGISTER_U32(TA_POLYEND +DC_P4) = pvr_state.ta_buffers[0].vertex + pvr_state.ta_buffers[0].vertex_size;


	RW_REGISTER_U32(TA_TILESIZE+DC_P4) = pvr_state.tsize_const;
	RW_REGISTER_U32(TA_TILECFG+DC_P4) = pvr_state.list_reg_mask;


	RW_REGISTER_U32(TA_LISTBASE+DC_P4) = pvr_state.ta_buffers[0].opb;

	RW_REGISTER_U32(RENDER_POLYBASE+DC_P4) = pvr_state.ta_buffers[0].vertex;
	RW_REGISTER_U32(RENDER_TILEBASE+DC_P4) = pvr_state.ta_buffers[0].tile_matrix;

	RW_REGISTER_U32(TA_INIT+DC_P4) = PVR_TA_INIT_GO;

    PVR_SET(PVR_SCALER_CFG, 0x401);

    PVR_SET(PVR_BGPLANE_Z, *((uint32*)&pvr_state.zclip));
	RW_REGISTER_U32(RENDER_HCLIP+DC_P4) = (680-1)<<16;
	RW_REGISTER_U32(RENDER_VCLIP+DC_P4) = (480-1)<<16;


	PVR_SET(PVR_RENDER_MODULO, (pvr_state.w * 2) / 8);

    /* Validate our memory pool */
    pvr_mem_reset();																	//---------------------------------------------

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

    return pvr_init2(&params);
}

void pvr_blank_polyhdr_buf3(int type, pvr_poly_hdr_t * poly) {
    /* Empty it out */
    memset(poly, 0, sizeof(pvr_poly_hdr_t));

    /* Put in the list type */
    poly->cmd = (type << PVR_TA_CMD_TYPE_SHIFT) | 0x80840012;

    /* Fill in dummy values */
    poly->d1 = poly->d2 = poly->d3 = poly->d4 = 0xffffffff;

}

int pvr_scene_finish3()
{

    unsigned int barray[32*5] __attribute__((aligned(32)));
	void *base = barray;

	pvr_blank_polyhdr_buf(0, (pvr_poly_hdr_t*)(base));
	memset(base+32, 0, 32);

    dcache_flush_range((ptr_t)base, (ptr_t)(32*3) );
	while(!pvr_dma_ready());
	pvr_dma_load_ta(base,32*2,0,NULL,0);
	while(!pvr_dma_ready());

	//--------------------
	pvr_blank_polyhdr_buf(2, (pvr_poly_hdr_t*)(base));
	memset(base+32, 0, 32);

    dcache_flush_range((ptr_t)base, (ptr_t)(32*3) );
	while(!pvr_dma_ready());
	pvr_dma_load_ta(base,32*2,0,NULL,0);
	while(!pvr_dma_ready());

	return 0;

}


#endif

