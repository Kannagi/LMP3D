

#ifdef DREAMCAST
#include <kos.h>


#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

#define PVR_OPB_COUNT   5

typedef struct {
	uint32  vertex, vertex_size;			/* Vertex buffer */
	uint32  opb, opb_size;				  /* Object pointer buffers, size */
	uint32  opb_type[PVR_OPB_COUNT];		/* Object pointer buffers (of each type) */
	uint32  tile_matrix;  /* Tile matrix, size */
} pvr_ta_buffers_t;

// Frame buffers structure: we have two sets of these
typedef struct {
	uint32  frame, frame_size;	  // Output frame buffer, size
} pvr_frame_buffers_t;

typedef struct {
	// If this is zero, then this state isn't valid

	// General configuration
	uint32  lists_enabled;			  // opb_completed's value when we're ready to render
	uint32  list_reg_mask;			  // Active lists register mask
	int	 opb_size[PVR_OPB_COUNT];	// opb size flags
	uint32  opb_ind[PVR_OPB_COUNT];	 // Individual opb sizes (in bytes)

	// Pipeline state
	int	 ta_target;				  // TA buffer we're writing (or DMAing) into
										// (^1 == TA buffer we're rendering from)
	int	 view_target;				// Frame buffer we're viewing
										// (^1 == frame buffer we're rendering to)
	// Memory pointers / buffers
	pvr_ta_buffers_t	ta_buffers;	  // TA buffers
	pvr_frame_buffers_t frame_buffers[2];   // Frame buffers

	// Screen size / clipping constants
	int	 w, h;					   // Screen width, height
	int	 tw, th;					 // Screen tile width, height
	uint32  tsize_const;				// Screen tile size constant
	float   zclip;					  // Z clip plane
	uint32  pclip_left, pclip_right;	// X pixel clip constants
	uint32  pclip_top, pclip_bottom;	// Y pixel clip constants
	uint32  pclip_x, pclip_y;		   // Composited clip constants
	uint32  bg_color;				   // Background color in ARGB format

} pvr_state_t;
pvr_state_t pvr_state;

typedef struct pvr_bkg_poly {
	uint32	  flags1, flags2;
	uint32	  dummy;
	float	   x1, y1, z1;
	uint32	  argb1;
	float	   x2, y2, z2;
	uint32	  argb2;
	float	   x3, y3, z3;
	uint32	  argb3;
} pvr_bkg_poly_t;

void DC_Clear_Color(float r, float g, float b)
{
	int ir, ig, ib;

	ir = (int)(255 * r);
	ig = (int)(255 * g);
	ib = (int)(255 * b);

	pvr_state.bg_color = (ir << 16) | (ig << 8) | (ib << 0) ;
}

int pvr_offset;
void DC_pvr_begin_queued_render()
{
	volatile pvr_ta_buffers_t   * tbuf;
	volatile pvr_frame_buffers_t	* rbuf;
	pvr_bkg_poly_t  bkg;
	uint32	  *vrl, *bkgdata;
	uint32	  vert_end;
	int	 i;

	/* Get the appropriate buffer */
	tbuf = &pvr_state.ta_buffers;
	rbuf = pvr_state.frame_buffers + (pvr_state.view_target ^ 1);

	/* Calculate background value for below */
	/* Small side note: during setup, the value is originally
	   0x01203000... I'm thinking that the upper word signifies
	   the length of the background plane list in dwords
	   shifted up by 4. */
	vert_end = 0x01000000 | ((PVR_GET(PVR_TA_VERTBUF_POS) - tbuf->vertex) << 1);

	/* Throw the background data on the end of the TA's list */
	bkg.flags1 = 0x90800000;	/* These are from libdream.. ought to figure out */
	bkg.flags2 = 0x20800440;	/*   what they mean for sure... heh =) */
	bkg.dummy = 0;
	bkg.x1 =   0.0f;
	bkg.y1 = 320.0f;
	bkg.z1 = 1.0f;
	bkg.argb1 = pvr_state.bg_color;
	bkg.x2 =   0.0f;
	bkg.y2 =   0.0f;
	bkg.z2 = 1.0f;
	bkg.argb2 = pvr_state.bg_color;
	bkg.x3 = 320.0f;
	bkg.y3 = 240.0f;
	bkg.z3 = 1.0f;
	bkg.argb3 = pvr_state.bg_color;
	bkgdata = (uint32 *)&bkg;
	vrl = (uint32*)(PVR_RAM_BASE | PVR_GET(PVR_TA_VERTBUF_POS));

	for(i = 0; i < 0x10; i++)
		vrl[i] = bkgdata[i];

	vrl[0x11] = 0;

	/* Reset the ISP/TSP, just in case */
	RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_ISPTSP;
	RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_NONE;

	/* Finish up rendering the current frame (into the other buffer) */
	RW_REGISTER_U32(RENDER_POLYBASE+DC_P2) = tbuf->vertex;

	RW_REGISTER_U32(RENDER_ADDR1+DC_P2) = rbuf->frame;
	RW_REGISTER_U32(RENDER_BGPLANE+DC_P2) = vert_end; /* Bkg plane location */
	//RW_REGISTER_FLOAT(RENDER_FARCLIP+DC_P2) = 1.00f;

	RW_REGISTER_U32(RENDER_START+DC_P2) = PVR_ISP_START_GO;   /* Start render */

	pvr_offset = PVR_GET(PVR_TA_VERTBUF_POS);
}

void DC_SwapBuffers()
{
		pvr_state.view_target ^= 1;

		vid_set_start(pvr_state.frame_buffers[pvr_state.view_target].frame);

		DC_pvr_begin_queued_render();

		volatile pvr_ta_buffers_t *buf;
		buf = &pvr_state.ta_buffers;

		PVR_SET(PVR_TA_VERTBUF_START,   buf->vertex);
		PVR_SET(PVR_TA_VERTBUF_END, buf->vertex + buf->vertex_size);

		//RW_REGISTER_FLOAT(RENDER_NEARCLIP+DC_P2) = -100.000f;
		PVR_SET(PVR_BGPLANE_Z, *((uint32*)&pvr_state.zclip));
		PVR_SET(PVR_PCLIP_X, pvr_state.pclip_x);
		PVR_SET(PVR_PCLIP_Y, pvr_state.pclip_y);

		RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_TA;
		RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_NONE;

		PVR_SET(PVR_TA_INIT,		PVR_TA_INIT_GO);		/* Confirm settings */
		(void)PVR_GET(PVR_TA_INIT);
}


void DC_pvr_allocate_buffers(pvr_init_params_t *params)
{
	volatile pvr_ta_buffers_t   *buf;
	int i, j;
	uint32  polybuf, sconst, polybuf_alloc,outaddr;

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
	pvr_state.zclip = 1.000f;
	pvr_state.pclip_left = 0;
	pvr_state.pclip_right = vid_mode->width - 1;
	pvr_state.pclip_top = 0;
	pvr_state.pclip_bottom = vid_mode->height - 1;
	pvr_state.pclip_x = (pvr_state.pclip_right << 16) | (pvr_state.pclip_left);
	pvr_state.pclip_y = (pvr_state.pclip_bottom << 16) | (pvr_state.pclip_top);

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


	volatile pvr_frame_buffers_t	*fbuf;

	outaddr = 0;

	buf = &pvr_state.ta_buffers;
	fbuf = &pvr_state.frame_buffers[0];

	/* Vertex buffer */
	buf->vertex = outaddr;
	buf->vertex_size = 0x1F0000-0x1400;
	outaddr += buf->vertex_size;

	/* Object Pointer Buffers */
	/* XXX What the heck is this 0x50580 magic value?? All I
	remember about it is that removing it makes it fail. */
	buf->opb_size = 0x50580 + polybuf;
	outaddr += buf->opb_size;
	polybuf_alloc = buf->opb = outaddr - polybuf;

	for(j = 0; j < PVR_OPB_COUNT; j++)
	{
		if(pvr_state.opb_size[j] > 0) {
			buf->opb_type[j] = polybuf_alloc;
			polybuf_alloc += pvr_state.opb_ind[j];
		}
		else {
			buf->opb_type[j] = 0x80000000;
		}
	}

	outaddr += buf->opb_size;   /* Do we _really_ need this twice? */

	outaddr = 0x2A2300;
	/* Tile Matrix */
	buf->tile_matrix = 0x2A2300;
	outaddr +=(18 + 6 * pvr_state.tw * pvr_state.th) * 4;;

	/* Output buffer */
	fbuf->frame = 0x2A4000;
	fbuf->frame_size = pvr_state.w * pvr_state.h * 2;
	outaddr += fbuf->frame_size;


	fbuf = &pvr_state.frame_buffers[1];

	/* Output buffer */
	fbuf->frame = 0x2A4000+0x96000;
	fbuf->frame_size = pvr_state.w * pvr_state.h * 2;
	outaddr += fbuf->frame_size;

	LMP3D_VRAM_Set(0x3D0000);
	//printf("%x\n",outaddr);
/*
	printf("%x\n",outaddr);


/*
	buf = &pvr_state.ta_buffers;
	buf->vertex = 0x600000;
	buf->vertex_size = 0x1F0000;

	buf->opb = 0x6F0000;
	buf->opb_size = 0xE000;
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

	buf->tile_matrix = 0x6FE000;
	//buf->tile_matrix_size = (18 + 6 * pvr_state.tw * pvr_state.th) * 4;

	//printf("%x\n",polybuf);

	//-------------
	pvr_state.frame_buffers[0].frame = 0x000000;
	pvr_state.frame_buffers[1].frame = 0x0B0000;*/

}

void DC_pvr_init_tile_matrix()
{
	volatile pvr_ta_buffers_t   *buf;
	int	 x, y;
	uint32	  *vr;
	volatile int	*opbs;

	vr = (uint32*)PVR_RAM_BASE;
	buf = &pvr_state.ta_buffers;
	opbs = pvr_state.opb_size;

	/* Header of zeros */
	vr += buf->tile_matrix>>2;

	/* Initial init tile */
	vr[0] = 0x10000000; //aa bb YY XX
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


void DC_Init_defaults(pvr_init_params_t *params)
{
	vid_empty();																	//---------------------------------------------

	/* Reset all PVR systems (in case it's still doing something) */
	RW_REGISTER_U32(PVR_RESET) = PVR_RESET_ALL;
	RW_REGISTER_U32(PVR_RESET) = PVR_RESET_NONE;


	/* Start off with a nice empty structure */
	memset((void *)&pvr_state, 0, sizeof(pvr_state));

	/* Everything's clear, do the initial buffer pointer setup */
	DC_pvr_allocate_buffers(params);																	//---------------------------------------------

	// Initialize tile matrices
	DC_pvr_init_tile_matrix();																	//---------------------------------------------

	pvr_state.ta_target = 0;
	pvr_state.view_target = 0;

	// Sync all the hardware registers with our pipeline state.
	RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_TA;
	RW_REGISTER_U32(PVRRESET+DC_P2) = PVR_RESET_NONE;

	/* Set buffer pointers */
	RW_REGISTER_U32(TA_TILEBASE+DC_P2) = pvr_state.ta_buffers.opb;
	RW_REGISTER_U32(TA_LISTEND +DC_P2) = pvr_state.ta_buffers.opb - pvr_state.ta_buffers.opb_size;

	RW_REGISTER_U32(TA_POLYBASE+DC_P2) = pvr_state.ta_buffers.vertex;
	RW_REGISTER_U32(TA_POLYEND +DC_P2) = pvr_state.ta_buffers.vertex + pvr_state.ta_buffers.vertex_size;


	RW_REGISTER_U32(TA_TILESIZE+DC_P2) = pvr_state.tsize_const;
	RW_REGISTER_U32(TA_TILECFG+DC_P2) = pvr_state.list_reg_mask;


	RW_REGISTER_U32(TA_LISTBASE+DC_P2) = pvr_state.ta_buffers.opb;

	RW_REGISTER_U32(RENDER_POLYBASE+DC_P2) = pvr_state.ta_buffers.vertex;
	RW_REGISTER_U32(RENDER_TILEBASE+DC_P2) = pvr_state.ta_buffers.tile_matrix;

	RW_REGISTER_U32(TA_INIT+DC_P2) = PVR_TA_INIT_GO;

	PVR_SET(PVR_SCALER_CFG, 0x401);

	PVR_SET(PVR_BGPLANE_Z, *((uint32*)&pvr_state.zclip));
	//RW_REGISTER_U32(RENDER_HCLIP+DC_P2) = (680-1)<<16;
	//RW_REGISTER_U32(RENDER_VCLIP+DC_P2) = (480-1)<<16;


	PVR_SET(PVR_RENDER_MODULO, (pvr_state.w * 2) / 8);

	/* Validate our memory pool */
	pvr_mem_reset();																	//---------------------------------------------
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DC_Init()
{
	//vid_set_mode(DM_640x480_NTSC_IL_MB,PM_RGB555);

	// Enable opaque and translucent polygons with size 16
	pvr_init_params_t params = { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0 };

	DC_Init_defaults(&params);

	DC_Clear_Color(0.5,0.5,0.5);

	RW_REGISTER_U32(DMAOR) = 0x8201;

	QACR0 = 0x10;
	QACR1 = 0x10;
}


void DC_Finish()
{
	unsigned int *d = UNCACHED_P4(0);
	d[0] = 0;

	asm("pref @%0" : : "r" (d));
}


#endif


