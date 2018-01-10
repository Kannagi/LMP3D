

#ifdef DREAMCAST



#include <kos.h>


#define RW_REGISTER_U8(REG) 	*((volatile u8  *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile u16 *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile int *)(REG))
#define RW_REGISTER_U64(REG) 	*((volatile u64 *)(REG))


static pvr_poly_cxt_t  cxt;
static pvr_poly_hdr_t  poly;



#define PVR_OPB_OP      0   /* Array indeces for these structures */
#define PVR_OPB_OM      1
#define PVR_OPB_TP      2
#define PVR_OPB_TM      3
#define PVR_OPB_PT      4
#define PVR_OPB_COUNT   5
#define PVR_SYNC_VBLANK     1   /* VBlank IRQ */
#define PVR_SYNC_BUFSTART   2   /* DMA buffer fill started */
#define PVR_SYNC_BUFDONE    3   /* DMA buffer fill complete */
#define PVR_SYNC_REGSTART   4   /* Registration started */
#define PVR_SYNC_REGDONE    5   /* Registration complete */
#define PVR_SYNC_RNDSTART   6   /* Render started */
#define PVR_SYNC_RNDDONE    7   /* Render complete IRQ */
#define PVR_SYNC_PAGEFLIP   8   /* View page was flipped */
typedef struct {
    uint32  vertex, vertex_size;            /* Vertex buffer */
    uint32  opb, opb_size;                  /* Object pointer buffers, size */
    uint32  opb_type[PVR_OPB_COUNT];        /* Object pointer buffers (of each type) */
    uint32  tile_matrix, tile_matrix_size;  /* Tile matrix, size */
} pvr_ta_buffers_t;

// DMA buffers structure: we have two sets of these
typedef struct {
    uint8   * base[PVR_OPB_COUNT];  // DMA buffers, if assigned
    uint32  ptr[PVR_OPB_COUNT];     // DMA buffer write pointer, if used
    uint32  size[PVR_OPB_COUNT];    // DMA buffer sizes, or zero if none
    int ready;                      // >0 if these buffers are ready to be DMAed
} pvr_dma_buffers_t;

// Frame buffers structure: we have two sets of these
typedef struct {
    uint32  frame, frame_size;      // Output frame buffer, size
} pvr_frame_buffers_t;

typedef struct {
    // If this is zero, then this state isn't valid
    int     valid;

    // General configuration
    uint32  lists_enabled;              // opb_completed's value when we're ready to render
    uint32  list_reg_mask;              // Active lists register mask
    int     dma_mode;                   // 1 if we are using DMA to transfer vertices
    int     opb_size[PVR_OPB_COUNT];    // opb size flags
    uint32  opb_ind[PVR_OPB_COUNT];     // Individual opb sizes (in bytes)

    // Pipeline state
    int     ram_target;                 // RAM buffer we're writing into
                                        // (^1 == RAM buffer we're DMAing from)
    int     ta_target;                  // TA buffer we're writing (or DMAing) into
                                        // (^1 == TA buffer we're rendering from)
    int     view_target;                // Frame buffer we're viewing
                                        // (^1 == frame buffer we're rendering to)

    int     list_reg_open;              // Which list is open for registration, if any? (non-DMA only)
    uint32  lists_closed;               // (1 << idx) for each list which the SH4 has lost interest in
    uint32  lists_transferred;          // (1 << idx) for each list which has completely transferred to the TA
    uint32  lists_dmaed;                // (1 << idx) for each list which has been DMA'd (DMA mode only)

    mutex_t dma_lock;                   // Locked if a DMA is in progress (vertex or texture)
    int     ta_busy;                    // >0 if a DMA is in progress and the TA hasn't signaled completion
    int     render_busy;                // >0 if a render is in progress
    int     render_completed;           // >1 if a render has recently finished

    // Memory pointers / buffers
    pvr_dma_buffers_t   dma_buffers[2];     // DMA buffers (if any)
    pvr_ta_buffers_t    ta_buffers[2];      // TA buffers
    pvr_frame_buffers_t frame_buffers[2];   // Frame buffers
    uint32              texture_base;       // Start of texture RAM

    // Screen size / clipping constants
    int     w, h;                       // Screen width, height
    int     tw, th;                     // Screen tile width, height
    uint32  tsize_const;                // Screen tile size constant
    float   zclip;                      // Z clip plane
    uint32  pclip_left, pclip_right;    // X pixel clip constants
    uint32  pclip_top, pclip_bottom;    // Y pixel clip constants
    uint32  pclip_x, pclip_y;           // Composited clip constants
    uint32  bg_color;                   // Background color in ARGB format

    /* Running statistics on the PVR system. All vars are in terms
       of milliseconds. */
    uint32  vbl_count;                  // VBlank counter for animations and such
    uint32  frame_count;                // Total number of viewed frames
    uint64  frame_last_time;            // When did the last frame completion occur?
    uint64  buf_start_time;             // When did the last DMA buffer fill begin?
    uint64  reg_start_time;             // When did the last registration begin?
    uint64  rnd_start_time;             // When did the last render begin?
    int     frame_last_len;             // VBlank-to-VBlank length for the last frame (1.0/FrameRate)
    int     buf_last_len;               // Cumulative buffer fill time for the last frame
    int     reg_last_len;               // Registration time for the last frame
    int     rnd_last_len;               // Render time for the last frame
    uint32  vtx_buf_used;               // Vertex buffer used size for the last frame
    uint32  vtx_buf_used_max;           // Maximum used vertex buffer size

    /* Wait-ready semaphore: this will be signaled whenever the pvr_wait_ready()
       call should be ready to return. */
    semaphore_t ready_sem;

    // Handle for the vblank interrupt
    int     vbl_handle;

    // Non-zero if FSAA was enabled at init time.
    int     fsaa;

    // Non-zero if we are rendering to a texture
    int     to_texture;

    // Render pitch for to-texture mode
    int     to_txr_rp;

    // Output address for to-texture mode
    uint32  to_txr_addr;
} pvr_state_t;
pvr_state_t pvr_state;

void pvr_sync_view22() {
    vid_set_start(pvr_state.frame_buffers[pvr_state.view_target].frame);
}

void pvr_sync_reg_buffer22() {
    volatile pvr_ta_buffers_t *buf;

    buf = pvr_state.ta_buffers + pvr_state.ta_target;

    /* Reset TA */
    //PVR_SET(PVR_RESET, PVR_RESET_TA);
    //PVR_SET(PVR_RESET, PVR_RESET_NONE);

    /* Set buffer pointers */
    PVR_SET(PVR_TA_OPB_START,   buf->opb);
    PVR_SET(PVR_TA_OPB_END,     buf->opb - buf->opb_size);
    PVR_SET(PVR_TA_VERTBUF_START,   buf->vertex);
    PVR_SET(PVR_TA_VERTBUF_END, buf->vertex + buf->vertex_size);
    PVR_SET(PVR_TA_OPB_INIT,    buf->opb);

    /* Misc config parameters */
    PVR_SET(PVR_TILEMAT_CFG,    pvr_state.tsize_const);     /* Tile count: (H/32-1) << 16 | (W/32-1) */
    PVR_SET(PVR_OPB_CFG,        pvr_state.list_reg_mask);   /* List enables */
    PVR_SET(PVR_TA_INIT,        PVR_TA_INIT_GO);        /* Confirm settings */
    (void)PVR_GET(PVR_TA_INIT);
}





void pvr_blank_polyhdr2_buf(int type, pvr_poly_hdr_t * poly) {
    /* Empty it out */
    memset(poly, 0, sizeof(pvr_poly_hdr_t));

    /* Put in the list type */
    poly->cmd = (type << PVR_TA_CMD_TYPE_SHIFT) | 0x80840012;

    /* Fill in dummy values */
    poly->d1 = poly->d2 = poly->d3 = poly->d4 = 0xffffffff;

}

void pvr_sync_reg_buffer2() {
    volatile pvr_ta_buffers_t *buf;

    buf = pvr_state.ta_buffers + pvr_state.ta_target;

    /* Reset TA */
    //PVR_SET(PVR_RESET, PVR_RESET_TA);
    //PVR_SET(PVR_RESET, PVR_RESET_NONE);

    /* Set buffer pointers */
    PVR_SET(PVR_TA_OPB_START,   buf->opb);
    PVR_SET(PVR_TA_OPB_END,     buf->opb - buf->opb_size);
    PVR_SET(PVR_TA_VERTBUF_START,   buf->vertex);
    PVR_SET(PVR_TA_VERTBUF_END, buf->vertex + buf->vertex_size);
    PVR_SET(PVR_TA_OPB_INIT,    buf->opb);

    /* Misc config parameters */
    PVR_SET(PVR_TILEMAT_CFG,    pvr_state.tsize_const);     /* Tile count: (H/32-1) << 16 | (W/32-1) */
    PVR_SET(PVR_OPB_CFG,        pvr_state.list_reg_mask);   /* List enables */
    PVR_SET(PVR_TA_INIT,        PVR_TA_INIT_GO);        /* Confirm settings */
    (void)PVR_GET(PVR_TA_INIT);
}

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
static semaphore_t dma_done;
static int dma_blocking;
static pvr_dma_callback_t dma_callback;
static ptr_t dma_cbdata;

/* DMA registers */
static vuint32  * const pvrdma = (vuint32 *)0xa05f6800;
static vuint32  * const shdma  = (vuint32 *)0xffa00000;

/* DMAC registers */
#define DMAC_SAR2   0x20/4
#define DMAC_DMATCR2    0x28/4
#define DMAC_CHCR2  0x2c/4
#define DMAC_DMAOR  0x40/4

/* PVR Dma registers - Offset by 0xA05F6800 */
#define PVR_STATE   0x00
#define PVR_LEN     0x04/4
#define PVR_DST     0x08/4
#define PVR_LMMODE0 0x84/4
#define PVR_LMMODE1 0x88/4

static void pvr_dma_irq_hnd(uint32 code) {
    if(shdma[DMAC_DMATCR2] != 0)
        dbglog(DBG_INFO, "pvr_dma: The dma did not complete successfully\n");

    // DBG(("pvr_dma_irq_hnd\n"));

    // Call the callback, if any.
    if(dma_callback) {
        // This song and dance is necessary because the handler
        // could chain to itself.
        pvr_dma_callback_t cb = dma_callback;
        ptr_t d = dma_cbdata;

        dma_callback = NULL;
        dma_cbdata = 0;

        cb(d);
    }

    // Signal the calling thread to continue, if any.
    if(dma_blocking) {
        sem_signal(&dma_done);
        thd_schedule(1, 0);
        dma_blocking = 0;
    }
}

static pvr_ptr_t pvr_mem_base = NULL;
void pvr_mem_reset2() {
    if(!pvr_state.valid)
        pvr_mem_base = NULL;
    else {
        pvr_mem_base = (pvr_ptr_t)(PVR_RAM_INT_BASE + pvr_state.texture_base);
        pvr_int_mem_reset2();
    }
}

void pvr_dma_init2() {
    /* Create an initially blocked semaphore */
    sem_init(&dma_done, 0);
    dma_blocking = 0;
    dma_callback = NULL;
    dma_cbdata = 0;

    /* Hook the neccessary interrupts */
    asic_evt_set_handler(ASIC_EVT_PVR_DMA, pvr_dma_irq_hnd);
    asic_evt_enable(ASIC_EVT_PVR_DMA, ASIC_IRQ_DEFAULT);
}

void pvr_begin_queued_render2() {
    volatile pvr_ta_buffers_t   * tbuf;
    volatile pvr_frame_buffers_t    * rbuf;
    pvr_bkg_poly_t  bkg;
    uint32      *vrl, *bkgdata;
    uint32      vert_end;
    int     i;

    /* Get the appropriate buffer */
    tbuf = pvr_state.ta_buffers + (pvr_state.ta_target ^ 1);
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
    //PVR_SET(PVR_RESET, PVR_RESET_ISPTSP);
    //PVR_SET(PVR_RESET, PVR_RESET_NONE);

    /* Finish up rendering the current frame (into the other buffer) */
    PVR_SET(PVR_ISP_TILEMAT_ADDR, tbuf->tile_matrix);
    PVR_SET(PVR_ISP_VERTBUF_ADDR, tbuf->vertex);

    if(!pvr_state.to_texture)
        PVR_SET(PVR_RENDER_ADDR, rbuf->frame);
    else {
        PVR_SET(PVR_RENDER_ADDR, pvr_state.to_txr_addr | (1 << 24));
        PVR_SET(PVR_RENDER_ADDR_2, pvr_state.to_txr_addr | (1 << 24));
    }

    PVR_SET(PVR_BGPLANE_CFG, vert_end); /* Bkg plane location */
    PVR_SET(PVR_BGPLANE_Z, *((uint32*)&pvr_state.zclip));
    PVR_SET(PVR_PCLIP_X, pvr_state.pclip_x);
    PVR_SET(PVR_PCLIP_Y, pvr_state.pclip_y);

    if(!pvr_state.to_texture)
        PVR_SET(PVR_RENDER_MODULO, (pvr_state.w * 2) / 8);
    else
        PVR_SET(PVR_RENDER_MODULO, pvr_state.to_txr_rp);

    // XXX Do we _really_ need this every time?
    // SETREG(PVR_FB_CFG_2, 0x00000009);        /* Alpha mode */
    PVR_SET(PVR_ISP_START, PVR_ISP_START_GO);   /* Start render */
}

void pvr_sync_stats2(int event) {
    uint64  t;
    volatile pvr_ta_buffers_t *buf;


    /* Get the current time */
    t = timer_ms_gettime64();

    switch(event) {
        case PVR_SYNC_VBLANK:
            pvr_state.vbl_count++;
            break;

        case PVR_SYNC_REGSTART:
            pvr_state.reg_start_time = t;
            break;

        case PVR_SYNC_REGDONE:
            pvr_state.reg_last_len = (uint32)(t - pvr_state.reg_start_time);

            buf = pvr_state.ta_buffers + pvr_state.ta_target;
            pvr_state.vtx_buf_used = PVR_GET(PVR_TA_VERTBUF_POS) - buf->vertex;

            if(pvr_state.vtx_buf_used > pvr_state.vtx_buf_used_max)
                pvr_state.vtx_buf_used_max = pvr_state.vtx_buf_used;

            break;

        case PVR_SYNC_RNDSTART:
            pvr_state.rnd_start_time = t;
            break;

        case PVR_SYNC_RNDDONE:
            pvr_state.rnd_last_len = (uint32)(t - pvr_state.rnd_start_time);
            break;

        case PVR_SYNC_BUFSTART:
            pvr_state.buf_start_time = t;
            break;

        case PVR_SYNC_BUFDONE:
            pvr_state.buf_last_len = (uint32)(t - pvr_state.buf_start_time);
            break;

        case PVR_SYNC_PAGEFLIP:
            pvr_state.frame_last_len = (uint32)(t - pvr_state.frame_last_time);
            pvr_state.frame_last_time = t;
            pvr_state.frame_count++;
            break;

    }
}

static void dma_next_list(ptr_t data) {
    int i, did = 0;
    volatile pvr_dma_buffers_t * b;

    // DBG(("dma_next_list\n"));

    for(i = 0; i < PVR_OPB_COUNT; i++) {
        if((pvr_state.lists_enabled & (1 << i))
                && !(pvr_state.lists_dmaed & (1 << i))) {
            // Get the buffers for this frame.
            b = pvr_state.dma_buffers + (pvr_state.ram_target ^ 1);

            // Flush the last 32 bytes out of dcache, just in case.
            // dcache_flush_range((ptr_t)(b->base[i] + b->ptr[i] - 32), 32);
            dcache_flush_range((ptr_t)(b->base[i]), b->ptr[i] + 32);
            //amt = b->ptr[i] > 16384 ? 16384 : b->ptr[i];
            //dcache_flush_range((ptr_t)(b->base[i] + b->ptr[i] - amt), amt);

            // Start the DMA transfer, chaining to ourselves.
            //DBG(("dma_begin(buf %d, list %d, base %p, len %d)\n",
            //  pvr_state.ram_target ^ 1, i,
            //  b->base[i], b->ptr[i]));
            pvr_dma_load_ta(b->base[i], b->ptr[i], 0, dma_next_list, 0);

            // Mark this list as done, and break out for now.
            pvr_state.lists_dmaed |= 1 << i;
            did++;

            break;
        }
    }

    // If that was the last one, then free up the DMA channel.
    if(!did) {
        //DBG(("dma_complete(buf %d)\n", pvr_state.ram_target ^ 1));

        // Unlock
        mutex_unlock((mutex_t *)&pvr_state.dma_lock);
        pvr_state.lists_dmaed = 0;

        // Buffers are now empty again
        pvr_state.dma_buffers[pvr_state.ram_target ^ 1].ready = 0;

        // Signal the client code to continue onwards.
        sem_signal((semaphore_t *)&pvr_state.ready_sem);
        thd_schedule(1, 0);
    }
}

void pvr_int_handler(uint32 code) {
    // What kind of event did we get?


    switch(code) {
        case ASIC_EVT_PVR_OPAQUEDONE:
            //DBG(("irq_opaquedone\n"));
            pvr_state.lists_transferred |= 1 << PVR_OPB_OP;
            break;
        case ASIC_EVT_PVR_TRANSDONE:
            //DBG(("irq_transdone\n"));
            pvr_state.lists_transferred |= 1 << PVR_OPB_TP;
            break;
        case ASIC_EVT_PVR_OPAQUEMODDONE:
            pvr_state.lists_transferred |= 1 << PVR_OPB_OM;
            break;
        case ASIC_EVT_PVR_TRANSMODDONE:
            pvr_state.lists_transferred |= 1 << PVR_OPB_TM;
            break;
        case ASIC_EVT_PVR_PTDONE:
            pvr_state.lists_transferred |= 1 << PVR_OPB_PT;
            break;
        case ASIC_EVT_PVR_RENDERDONE:
            //DBG(("irq_renderdone\n"));
            pvr_state.render_busy = 0;
            pvr_state.render_completed = 1;
            //pvr_sync_stats2(PVR_SYNC_RNDDONE);
            break;
        case ASIC_EVT_PVR_VBLINT:
            //pvr_sync_stats2(PVR_SYNC_VBLANK);
            break;
    }

    /* Update our stats if we finished all registration */
    switch(code) {
        case ASIC_EVT_PVR_OPAQUEDONE:
        case ASIC_EVT_PVR_TRANSDONE:
        case ASIC_EVT_PVR_OPAQUEMODDONE:
        case ASIC_EVT_PVR_TRANSMODDONE:
        case ASIC_EVT_PVR_PTDONE:

            if(pvr_state.lists_transferred == pvr_state.lists_enabled) {
                pvr_sync_stats2(PVR_SYNC_REGDONE);
            }

            return;
    }

    // If it's not a vblank, ignore for now for the rest of this.
    if(code != ASIC_EVT_PVR_VBLINT)
        return;

    // If the render-done interrupt has fired then we are ready to flip to the
    // new frame buffer.
    if(pvr_state.render_completed) {
        //DBG(("view(%d)\n", pvr_state.view_target ^ 1));

        // Handle PVR stats
        pvr_sync_stats2(PVR_SYNC_PAGEFLIP);

        // Switch view address to the "good" buffer
        if(pvr_state.to_texture != 2)
            pvr_state.view_target ^= 1;

        pvr_sync_view22();

        // Clear the render completed flag.
        pvr_state.render_completed = 0;

        // Set the fact that the next frame should be texture rendered, if it is the case
        pvr_state.to_texture = pvr_state.to_texture == 1 ? 2 : 0;
    }

    // If all lists are fully transferred and a render is not in progress,
    // we are ready to start rendering.
    if(!pvr_state.render_busy
            && pvr_state.lists_transferred == pvr_state.lists_enabled) {
        /* XXX Note:
           For some reason, the render must be started _before_ we sync
           to the new reg buffers. The only reasons I can think of for this
           are that there may be something in the reg sync that messes up
           the render in progress, or we are misusing some bits somewhere. */

        // Begin rendering from the dirty TA buffer into the clean
        // frame buffer.
        //DBG(("start_render(%d -> %d)\n", pvr_state.ta_target, pvr_state.view_target ^ 1));
        pvr_state.ta_target ^= 1;
        pvr_begin_queued_render2();
        pvr_state.render_busy = 1;
        pvr_sync_stats2(PVR_SYNC_RNDSTART);

        // If we're not in DMA mode, then signal the client code
        // to continue onwards.
        if(!pvr_state.dma_mode) {
            sem_signal((semaphore_t *)&pvr_state.ready_sem);
            thd_schedule(1, 0);
        }

        // Switch to the clean TA buffer.
        pvr_state.lists_transferred = 0;
        pvr_sync_reg_buffer22();

        // The TA is no longer busy.
        pvr_state.ta_busy = 0;
    }

    // If we're in DMA mode, the DMA source buffers are ready, and a DMA
    // is not in progress, then we are ready to start DMAing.
    if(pvr_state.dma_mode
            && !pvr_state.ta_busy
            && pvr_state.dma_buffers[pvr_state.ram_target ^ 1].ready
            && mutex_trylock((mutex_t *)&pvr_state.dma_lock) >= 0) {
        pvr_sync_stats2(PVR_SYNC_REGSTART);

        // Begin DMAing the first list.
        pvr_state.ta_busy = 1;
        //dma_next_list(0);
    }
}













#define BUF_ALIGN 128
#define BUF_ALIGN_MASK (BUF_ALIGN - 1)
void pvr_allocate_buffers2(pvr_init_params_t *params) {
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

    /* FSAA -> double the tile buffer width */
    if(pvr_state.fsaa)
        pvr_state.tw *= 2;

    /* We can actually handle non-mod-32 heights pretty easily -- just extend
       the frame buffer a bit, but use a pixel clip for the real mode. */
    if((pvr_state.h % 32) != 0) {
        pvr_state.h = (pvr_state.h + 32) & ~31;
        pvr_state.th++;
    }

    pvr_state.tsize_const = ((pvr_state.th - 1) << 16)
                            | ((pvr_state.tw - 1) << 0);

    /* Set clipping parameters */
    pvr_state.zclip = 0.0001f;
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

    /* Initialize each buffer set */
    for(i = 0; i < 2; i++) {
        /* Frame 0 goes at 0, Frame 1 goes at 0x400000 (half way) */
        if(i == 0)
            outaddr = 0;
        else
            outaddr = 0x400000;

        /* Select a pvr_buffers_t. Note that there's no good reason
           to allocate the frame buffers at the same time as the TA
           buffers except that it's handy to do it all in one place. */
        buf = pvr_state.ta_buffers + i;
        fbuf = pvr_state.frame_buffers + i;

        /* Vertex buffer */
        buf->vertex = outaddr;
        buf->vertex_size = params->vertex_buf_size;
        outaddr += buf->vertex_size;

        /* N-byte align */
        outaddr = (outaddr + BUF_ALIGN_MASK) & ~BUF_ALIGN_MASK;

        /* Object Pointer Buffers */
        /* XXX What the heck is this 0x50580 magic value?? All I
           remember about it is that removing it makes it fail. */
        buf->opb_size = 0x50580 + polybuf;
        outaddr += buf->opb_size;
        // buf->poly_buf_size = (outaddr - polybuf) - buf->vertex;
        polybuf_alloc = buf->opb = outaddr - polybuf;

        for(j = 0; j < PVR_OPB_COUNT; j++) {
            if(pvr_state.opb_size[j] > 0) {
                buf->opb_type[j] = polybuf_alloc;
                polybuf_alloc += pvr_state.opb_ind[j];
            }
            else {
                buf->opb_type[j] = 0x80000000;
            }
        }

        outaddr += buf->opb_size;   /* Do we _really_ need this twice? */

        /* N-byte align */
        outaddr = (outaddr + BUF_ALIGN_MASK) & ~BUF_ALIGN_MASK;

        /* Tile Matrix */
        buf->tile_matrix = outaddr;
        buf->tile_matrix_size = (18 + 6 * pvr_state.tw * pvr_state.th) * 4;
        outaddr += buf->tile_matrix_size;

        /* N-byte align */
        outaddr = (outaddr + BUF_ALIGN_MASK) & ~BUF_ALIGN_MASK;

        /* Output buffer */
        fbuf->frame = outaddr;
        fbuf->frame_size = pvr_state.w * pvr_state.h * 2;
        outaddr += fbuf->frame_size;

        /* N-byte align */
        outaddr = (outaddr + BUF_ALIGN_MASK) & ~BUF_ALIGN_MASK;
    }

    /* Texture ram is whatever is left */
    pvr_state.texture_base = (outaddr - 0x400000) * 2;

}

void pvr_sync_view2() {
    //vid_set_start(pvr_state.frame_buffers[pvr_state.view_target].frame);
}

void pvr_int_mem_reset2() {
    /* This _should_ do it */
    //memset(&av_, 0, sizeof(av_));
}

void pvr_init_tile_matrix2(int which) {
    volatile pvr_ta_buffers_t   *buf;
    int     x, y;
    uint32      *vr;
    volatile int    *opbs;
    uint32      matbase, opbbase;

    vr = (uint32*)PVR_RAM_BASE;
    buf = pvr_state.ta_buffers + which;
    opbs = pvr_state.opb_size;

    matbase = buf->tile_matrix;
    opbbase = buf->opb;

    /* Header of zeros */
    vr += buf->tile_matrix / 4;

    for(x = 0; x < 0x48; x += 4)
        * vr++ = 0;

    /* Initial init tile */
    vr[0] = 0x10000000;
    vr[1] = 0x80000000;
    vr[2] = 0x80000000;
    vr[3] = 0x80000000;
    vr[4] = 0x80000000;
    vr[5] = 0x80000000;
    vr += 6;

    /* Now the main tile matrix */


    for(x = 0; x < pvr_state.tw; x++) {
        for(y = 0; y < pvr_state.th; y++) {
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

    /* Must skip over zeroed header for actual usage */
    buf->tile_matrix += 0x48;
}

/* Fill all tile matrices */
void pvr_init_tile_matrices2() {
    int i;

    for(i = 0; i < 2; i++)
        pvr_init_tile_matrix2(i);
}
int pvr_init2(pvr_init_params_t *params) {




    /* Clear out video memory */
    vid_empty();																	//---------------------------------------------

    /* Reset all PVR systems (in case it's still doing something) */
    RW_REGISTER_U32(PVR_RESET) = PVR_RESET_ALL;
    RW_REGISTER_U32(PVR_RESET) = PVR_RESET_NONE;


    /* Start off with a nice empty structure */
    memset((void *)&pvr_state, 0, sizeof(pvr_state));

    // Enable DMA if the user wants that.
    pvr_state.dma_mode = params->dma_enabled;

    // Copy over FSAA setting.
    pvr_state.fsaa = params->fsaa_enabled;

    /* Everything's clear, do the initial buffer pointer setup */
    pvr_allocate_buffers2(params);																	//---------------------------------------------

    // Initialize tile matrices
    pvr_init_tile_matrices2();																	//---------------------------------------------

    // Setup all pipeline targets. Yes, this is redundant. :) I just
    // like to have it explicit.
    pvr_state.ram_target = 0;
    pvr_state.ta_target = 0;
    pvr_state.view_target = 0;

    pvr_state.list_reg_open = -1;

    // Sync all the hardware registers with our pipeline state.
    pvr_sync_reg_buffer2();

    // Clear out our stats
    pvr_state.vbl_count = 0;
    pvr_state.frame_last_time = 0;
    pvr_state.buf_start_time = 0;
    pvr_state.reg_start_time = 0;
    pvr_state.rnd_start_time = 0;
    pvr_state.frame_last_len = -1;
    pvr_state.buf_last_len = -1;
    pvr_state.reg_last_len = -1;
    pvr_state.rnd_last_len = -1;
    pvr_state.vtx_buf_used = 0;
    pvr_state.vtx_buf_used_max = 0;

    /* If we're on a VGA box, disable vertical smoothing */
    if(vid_mode->cable_type == CT_VGA) {
        dbglog(DBG_KDEBUG, "pvr: disabling vertical scaling for VGA\n");

        if(pvr_state.fsaa)
            PVR_SET(PVR_SCALER_CFG, 0x10400);
        else
            PVR_SET(PVR_SCALER_CFG, 0x400);
    }
    else {
        dbglog(DBG_KDEBUG, "pvr: enabling vertical scaling for non-VGA\n");

        if(pvr_state.fsaa)
            PVR_SET(PVR_SCALER_CFG, 0x10401);
        else
            PVR_SET(PVR_SCALER_CFG, 0x401);
    }

    /* Hook the PVR interrupt events on G2 */

    pvr_state.vbl_handle = vblank_handler_add(pvr_int_handler);
    asic_evt_set_handler(ASIC_EVT_PVR_OPAQUEDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_OPAQUEDONE, ASIC_IRQ_DEFAULT);
    asic_evt_set_handler(ASIC_EVT_PVR_OPAQUEMODDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_OPAQUEMODDONE, ASIC_IRQ_DEFAULT);
    asic_evt_set_handler(ASIC_EVT_PVR_TRANSDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_TRANSDONE, ASIC_IRQ_DEFAULT);
    asic_evt_set_handler(ASIC_EVT_PVR_TRANSMODDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_TRANSMODDONE, ASIC_IRQ_DEFAULT);
    asic_evt_set_handler(ASIC_EVT_PVR_PTDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_PTDONE, ASIC_IRQ_DEFAULT);
    asic_evt_set_handler(ASIC_EVT_PVR_RENDERDONE, pvr_int_handler);
    asic_evt_enable(ASIC_EVT_PVR_RENDERDONE, ASIC_IRQ_DEFAULT);

    /* 3d-specific parameters; these are all about rendering and
       nothing to do with setting up the video; some stuff in here
       is still unknown. */



    RW_REGISTER_U32(PVR_UNK_00A8) = 0x15d1c951;      /* M (Unknown magic value) */
    RW_REGISTER_U32(PVR_UNK_00A0) = 0x00000020;      /* M */
    RW_REGISTER_U32(PVR_FB_CFG_2) = 0x00000009;      /* alpha config */
    RW_REGISTER_U32(PVR_UNK_0110) = 0x00093f39;      /* M */
    RW_REGISTER_U32(PVR_UNK_0098) = 0x00800408;      /* M */
    RW_REGISTER_U32(PVR_TEXTURE_CLIP) = 0x00000000;      /* texture clip distance */
    RW_REGISTER_U32(PVR_SPANSORT_CFG) = 0x00000101;      /* M */
    RW_REGISTER_U32(PVR_FOG_TABLE_COLOR) = 0x007f7f7f;   /* Fog table color */
    RW_REGISTER_U32(PVR_FOG_VERTEX_COLOR) = 0x007f7f7f;  /* Fog vertex color */
    RW_REGISTER_U32(PVR_COLOR_CLAMP_MIN) = 0x00000000;   /* color clamp min */
    RW_REGISTER_U32(PVR_COLOR_CLAMP_MAX) = 0xffffffff;   /* color clamp max */
    RW_REGISTER_U32(PVR_UNK_0080) = 0x00000007;      /* M */
    RW_REGISTER_U32(PVR_CHEAP_SHADOW) = 0x00000001;      /* cheap shadow */
    RW_REGISTER_U32(PVR_UNK_007C) = 0x0027df77;      /* M */
    RW_REGISTER_U32(PVR_TEXTURE_MODULO) = 0x00000000;    /* stride width */
    RW_REGISTER_U32(PVR_FOG_DENSITY) = 0x0000ff07;       /* fog density */
    RW_REGISTER_U32(PVR_UNK_00C8) = (PVR_GET(0x00d4) << 16);   /* M */
    RW_REGISTER_U32(PVR_UNK_0118) = 0x00008040;      /* M */




    /* Initialize PVR DMA */
    mutex_init((mutex_t *)&pvr_state.dma_lock, MUTEX_TYPE_NORMAL);
    pvr_dma_init2();

    /* Setup our wait-ready semaphore */
    sem_init((semaphore_t *)&pvr_state.ready_sem, 0);

    /* Set us as valid and return success */
    pvr_state.valid = 1;

    /* Validate our memory pool */
    //pvr_mem_reset2();																	//---------------------------------------------
    /* This doesn't work right now... */
    /*#ifndef NDEBUG
        dbglog(DBG_KDEBUG, "pvr: free memory is %08lx bytes\n",
            pvr_mem_available());
    #endif*//* !NDEBUG */

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void pvr_poly_compile2(pvr_poly_hdr_t *dst, pvr_poly_cxt_t *src) {
    int u, v;
    uint32  txr_base;

    /* Basically we just take each parameter, clip it, shift it
       into place, and OR it into the final result. */

    /* The base values for CMD */
    dst->cmd = PVR_CMD_POLYHDR;

    if(src->txr.enable == PVR_TEXTURE_ENABLE)
        dst->cmd |= 8;

    /* Or in the list type, shading type, color and UV formats */
    dst->cmd |= (src->list_type << PVR_TA_CMD_TYPE_SHIFT) & PVR_TA_CMD_TYPE_MASK;
    dst->cmd |= (src->fmt.color << PVR_TA_CMD_CLRFMT_SHIFT) & PVR_TA_CMD_CLRFMT_MASK;
    dst->cmd |= (src->gen.shading << PVR_TA_CMD_SHADE_SHIFT) & PVR_TA_CMD_SHADE_MASK;
    dst->cmd |= (src->fmt.uv << PVR_TA_CMD_UVFMT_SHIFT) & PVR_TA_CMD_UVFMT_MASK;
    dst->cmd |= (src->gen.clip_mode << PVR_TA_CMD_USERCLIP_SHIFT) & PVR_TA_CMD_USERCLIP_MASK;
    dst->cmd |= (src->fmt.modifier << PVR_TA_CMD_MODIFIER_SHIFT) & PVR_TA_CMD_MODIFIER_MASK;
    dst->cmd |= (src->gen.modifier_mode << PVR_TA_CMD_MODIFIERMODE_SHIFT) & PVR_TA_CMD_MODIFIERMODE_MASK;

    /* Polygon mode 1 */
    dst->mode1  = (src->depth.comparison << PVR_TA_PM1_DEPTHCMP_SHIFT) & PVR_TA_PM1_DEPTHCMP_MASK;
    dst->mode1 |= (src->gen.culling << PVR_TA_PM1_CULLING_SHIFT) & PVR_TA_PM1_CULLING_MASK;
    dst->mode1 |= (src->depth.write << PVR_TA_PM1_DEPTHWRITE_SHIFT) & PVR_TA_PM1_DEPTHWRITE_MASK;
    dst->mode1 |= (src->txr.enable << PVR_TA_PM1_TXRENABLE_SHIFT) & PVR_TA_PM1_TXRENABLE_MASK;

    /* Polygon mode 2 */
    dst->mode2  = (src->blend.src << PVR_TA_PM2_SRCBLEND_SHIFT) & PVR_TA_PM2_SRCBLEND_MASK;
    dst->mode2 |= (src->blend.dst << PVR_TA_PM2_DSTBLEND_SHIFT) & PVR_TA_PM2_DSTBLEND_MASK;
    dst->mode2 |= (src->blend.src_enable << PVR_TA_PM2_SRCENABLE_SHIFT) & PVR_TA_PM2_SRCENABLE_MASK;
    dst->mode2 |= (src->blend.dst_enable << PVR_TA_PM2_DSTENABLE_SHIFT) & PVR_TA_PM2_DSTENABLE_MASK;
    dst->mode2 |= (src->gen.fog_type << PVR_TA_PM2_FOG_SHIFT) & PVR_TA_PM2_FOG_MASK;
    dst->mode2 |= (src->gen.color_clamp << PVR_TA_PM2_CLAMP_SHIFT) & PVR_TA_PM2_CLAMP_MASK;
    dst->mode2 |= (src->gen.alpha << PVR_TA_PM2_ALPHA_SHIFT) & PVR_TA_PM2_ALPHA_MASK;

    if(src->txr.enable == PVR_TEXTURE_DISABLE) {
        dst->mode3 = 0;
    }
    else {
        dst->mode2 |= (src->txr.alpha << PVR_TA_PM2_TXRALPHA_SHIFT) & PVR_TA_PM2_TXRALPHA_MASK;
        dst->mode2 |= (src->txr.uv_flip << PVR_TA_PM2_UVFLIP_SHIFT) & PVR_TA_PM2_UVFLIP_MASK;
        dst->mode2 |= (src->txr.uv_clamp << PVR_TA_PM2_UVCLAMP_SHIFT) & PVR_TA_PM2_UVCLAMP_MASK;
        dst->mode2 |= (src->txr.filter << PVR_TA_PM2_FILTER_SHIFT) & PVR_TA_PM2_FILTER_MASK;
        dst->mode2 |= (src->txr.mipmap_bias << PVR_TA_PM2_MIPBIAS_SHIFT) & PVR_TA_PM2_MIPBIAS_MASK;
        dst->mode2 |= (src->txr.env << PVR_TA_PM2_TXRENV_SHIFT) & PVR_TA_PM2_TXRENV_MASK;

        switch(src->txr.width) {
            case 8:
                u = 0;
                break;
            case 16:
                u = 1;
                break;
            case 32:
                u = 2;
                break;
            case 64:
                u = 3;
                break;
            case 128:
                u = 4;
                break;
            case 256:
                u = 5;
                break;
            case 512:
                u = 6;
                break;
            case 1024:
                u = 7;
                break;
            default:

                u = 0;
                break;
        }

        switch(src->txr.height) {
            case 8:
                v = 0;
                break;
            case 16:
                v = 1;
                break;
            case 32:
                v = 2;
                break;
            case 64:
                v = 3;
                break;
            case 128:
                v = 4;
                break;
            case 256:
                v = 5;
                break;
            case 512:
                v = 6;
                break;
            case 1024:
                v = 7;
                break;
            default:

                v = 0;
                break;
        }

        dst->mode2 |= (u << PVR_TA_PM2_USIZE_SHIFT) & PVR_TA_PM2_USIZE_MASK;
        dst->mode2 |= (v << PVR_TA_PM2_VSIZE_SHIFT) & PVR_TA_PM2_VSIZE_MASK;

        /* Polygon mode 3 */
        dst->mode3  = (src->txr.mipmap << PVR_TA_PM3_MIPMAP_SHIFT) & PVR_TA_PM3_MIPMAP_MASK;
        dst->mode3 |= (src->txr.format << PVR_TA_PM3_TXRFMT_SHIFT) & PVR_TA_PM3_TXRFMT_MASK;

        /* Convert the texture address */
        txr_base = (uint32)src->txr.base;
        txr_base = (txr_base & 0x00fffff8) >> 3;
        dst->mode3 |= txr_base;
    }

    if(src->fmt.modifier) {
        /* If we're affected by a modifier volume, silently promote the header
           to the one that is affected by a modifier volume. */
        dst->d1 = dst->mode2;
        dst->d2 = dst->mode3;
    }
    else {
        dst->d1 = dst->d2 = 0xffffffff;
    }

    dst->d3 = dst->d4 = 0xffffffff;
}


void pvr_poly_cxt_col2(pvr_poly_cxt_t *dst, pvr_list_t list) {
    int alpha;

    /* Start off blank */
    memset(dst, 0, sizeof(pvr_poly_cxt_t));

    /* Fill in a few values */
    dst->list_type = list;
    alpha = list > PVR_LIST_OP_MOD;
    dst->fmt.color = PVR_CLRFMT_ARGBPACKED;
    dst->fmt.uv = PVR_UVFMT_32BIT;
    dst->gen.shading = PVR_SHADE_GOURAUD;
    dst->depth.comparison = PVR_DEPTHCMP_GREATER;
    dst->depth.write = PVR_DEPTHWRITE_ENABLE;
    dst->gen.culling = PVR_CULLING_CCW;
    dst->txr.enable = PVR_TEXTURE_DISABLE;

    if(!alpha) {
        dst->gen.alpha = PVR_ALPHA_DISABLE;
        dst->blend.src = PVR_BLEND_ONE;
        dst->blend.dst = PVR_BLEND_ZERO;
    }
    else {
        dst->gen.alpha = PVR_ALPHA_ENABLE;
        dst->blend.src = PVR_BLEND_SRCALPHA;
        dst->blend.dst = PVR_BLEND_INVSRCALPHA;
    }

    dst->blend.src_enable = PVR_BLEND_DISABLE;
    dst->blend.dst_enable = PVR_BLEND_DISABLE;
    dst->gen.fog_type = PVR_FOG_DISABLE;
    dst->gen.color_clamp = PVR_CLRCLAMP_DISABLE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------




























#define UNCACHED_P2(x) ( (void*) ( (unsigned int)(x) | (0xA0000000) ) )
#define CACHED_P3(x)   ( (void*) ( (unsigned int)(x) | (0xC0000000) ) )
#define UNCACHED_P4(x) ( (void*) ( (unsigned int)(x) | (0xE0000000) ) )

#define RW_REGISTER_U8(REG) 	*((volatile unsigned char *)(REG))

#define CRT0 0xFFC80038
#define CRT1 0xFFC8003C

void pvr_scene_begin2() {
    int i;

    // Get general stuff ready.
    pvr_state.list_reg_open = -1;

    // Clear these out in case we're using DMA.
    /*
    if(pvr_state.dma_mode) {
        for(i = 0; i < PVR_OPB_COUNT; i++) {
            pvr_state.dma_buffers[pvr_state.ram_target].ptr[i] = 0;
        }

        pvr_sync_stats2(PVR_SYNC_BUFSTART);
        // DBG(("pvr_scene_begin(dma -> %d)\n", pvr_state.ram_target));
    }
    else {
    */
        pvr_state.lists_closed = 0;

        // We assume registration is starting immediately
        //pvr_sync_stats2(PVR_SYNC_REGSTART);
    //}
}

void * sq_set32z(void *s, uint32 c, int n)
{

    unsigned int *d = UNCACHED_P4(0);

    /* Set store queue memory area as desired */
    //QACR0 = ((((unsigned int)s) >> 26) << 2) & 0x1c;

    QACR0 = ((0xb0000000>>26)<<2)&0x1c;

    //printf("%x\n",( ( ( (unsigned int)s) >> 26) << 2) & 0x1c) ;

    /* Fill both store queues with c */
    d[0] = d[1] = d[2] = d[3] = d[4] = d[5] = d[6] = d[7] =
                                           d[8] = d[9] = d[10] = d[11] = d[12] = d[13] = d[14] = d[15] = c;

    /* Write them as many times necessary */
    n >>= 5;

    while(n--) {
        asm("pref @%0" : : "r"(d));
    }



    return s;
}

void * sq_cpy2(void *dest, void *src, int n)
{




	//printf("%x\n",( ( ( (unsigned int)dest) >> 26) << 2) & 0x1c) ;

    unsigned int *d = UNCACHED_P4(0);
    unsigned int *s = src;

    /* Set store queue memory area as desired */
    //QACR0 = ((((unsigned int)dest) >> 26) << 2) & 0x1c;
    //RW_REGISTER_U8(CRT0) = ((((unsigned int)dest) >> 26) << 2) & 0x1c;
	//RW_REGISTER_U8(CRT1) = ((((unsigned int)dest) >> 26) << 2) & 0x1c;
    //QACR1 = ((((unsigned int)dest) >> 26) << 2) & 0x1c;

    /* fill/write queues as many times necessary */
    n >>= 5;


        asm("pref @%0" : : "r"(s + 8));  /* prefetch 32 bytes for next loop */
        d[0] = *(s++);
        d[1] = *(s++);
        d[2] = *(s++);
        d[3] = *(s++);
        d[4] = *(s++);
        d[5] = *(s++);
        d[6] = *(s++);
        d[7] = *(s++);

        asm("pref @%0" : : "r"(d));
        d += 8;


    /* Wait for both store queues to complete */



    return dest;
}


void pvr_blank_polyhdr_buf2(int type, pvr_poly_hdr_t * poly) {
    /* Empty it out */
    memset(poly, 0, sizeof(pvr_poly_hdr_t));

    /* Put in the list type */
    poly->cmd = (type << PVR_TA_CMD_TYPE_SHIFT) | 0x80840012;

    /* Fill in dummy values */
    poly->d1 = poly->d2 = poly->d3 = poly->d4 = 0xffffffff;

}

void ta_commit_list(void *src)
{
  unsigned int *s = (unsigned int *)src;
  unsigned int *d = (unsigned int *)0xe0000000;
  QACR0 = ((0xb0000000>>26)<<2)&0x1c;
  d[0] = *s++;
  d[1] = *s++;
  d[2] = *s++;
  d[3] = *s++;
  d[4] = *s++;
  d[5] = *s++;
  d[6] = *s++;
  d[7] = *s++;
  asm("pref @%0" : : "r" (d));
}

void pvr_blank_polyhdr2(int type) {
    pvr_poly_hdr_t poly;

    // Make it.
    pvr_blank_polyhdr_buf2(type, &poly);

    // Submit it
    //pvr_prim(&poly, sizeof(poly));

	pvr_poly_hdr_t * pvrpoly = &poly;
	//printf("ta : %x w: %x %x %x d: %x %x %x %x\n",pvrpoly->cmd,pvrpoly->mode1,pvrpoly->mode2,pvrpoly->mode3,pvrpoly->d1,pvrpoly->d2,pvrpoly->d3,pvrpoly->d4);

	ta_commit_list(&poly);
    //sq_cpy2((void *)PVR_TA_INPUT, &poly, sizeof(poly));
}

int pvr_scene_finish2() {
    int i, o;
    volatile pvr_dma_buffers_t * b;

    // If we're in DMA mode, then this works a little differently...


/*
	// If any lists weren't submitted, then submit blank ones now
	for(i = 0; i < PVR_OPB_COUNT; i++)
	{
		if((pvr_state.lists_enabled & (1 << i)) && (!(pvr_state.lists_closed & (1 << i))))
		{
			printf("%d /%d\n",i,PVR_OPB_COUNT) ;
			pvr_state.list_reg_open = i;
			pvr_list_finish2();
		}
	}
	*/

	pvr_state.list_reg_open = 0;
	pvr_list_finish2();

	pvr_state.list_reg_open = 2;
	pvr_list_finish2();

	pvr_state.list_reg_open = -1;


    /* Ok, now it's just a matter of waiting for the interrupt... */
    return 0;
}

int pvr_list_finish2()
{



        /* In case we haven't sent anything in this list, send a dummy */
	pvr_blank_polyhdr2(pvr_state.list_reg_open); 																	//---------------------------------------------

	/* Set the flags */
	pvr_state.lists_closed |= (1 << pvr_state.list_reg_open);

	/* Send an EOL marker */
	sq_set32z((void *)PVR_TA_INPUT, 0, 32);



    return 0;
}




#endif

