
//OXOOOO OOOO - 0X001F FFFF ROM 2MB
//OXOO20 OOOO - 0X0021 FFFF Flash Memory 128 KB
//OXOO5F 68OO - 0X005F 69FF System Control Reg 512B
//OXOO5F 6COO - 0X005F 6CFF Maple i/f Control Reg 256B
//OXOO5f 7000 - 0X005F 70FF GD-ROM  256B
//OXOO5f 74OO - 0X005F 74FF G1 i/f Control Reg 256B
//OXOO5F 7800 - 0X005F 78FF G2 i/f Control Reg 256B

//0X005F 7C00 - 0X005F 7CFF PVR i/f Control Reg 256B
//OXOO5F 8000 - 0X005F 9FFF TA/PVR Core Reg 8KB
//OXOO60 0000 - 0X0060 07FF MODEM 2KB

//OXOO70 0000 - 0X0070 7FFF AICA-SOUND Cntr Reg 32KB
//OXOO71 0000 - 0X0071 000B AICA-RTC Cntr Reg 12B
//OXOO80 0000 - 0X009F FFFF AICA-Wave Memory 2MB

//OXO400 0000 - 0X047F FFFF Tex Mem 64bit Acc 8MB
//OXO500 0000 - 0X057F FFFF Tex Mem 32bit Acc 8MB

//OXOC00 0000 - 0X0C7F FFFF System Memory 16MB

//OX1000 0000 - 0X107F FFFF TA FIFO Polygon Cnv 8MB
//OX1080 0000 - 0X10FF FFFF TA FIFO YUV Conv  8MB
//OX1100 0000 - 0X11FF FFFF Tex Mem 32/64Bit Acc 16 MB

/*


*/

#define CACHED_P1(x)   ( (void*) ( (unsigned int)(x) | (0x80000000) ) )
#define UNCACHED_P2(x) ( (void*) ( (unsigned int)(x) | (0xA0000000) ) )
#define CACHED_P3(x)   ( (void*) ( (unsigned int)(x) | (0xC0000000) ) )
#define UNCACHED_P4(x) ( (void*) ( (unsigned int)(x) | (0xE0000000) ) )

#define DC_P4 0xE0000000
#define DC_P2 0xA0000000

#define DMAOR 0xFFA00040

#define FRQCR 0XFFC00000
#define STBCR 0XFFC00004
#define CRT0  0xFFC80038
#define CRT1  0xFFC8003C


#define DC_SET_STBCR(mstp0,mstp1,mstp2,mstp3,mstp4,ppu,phz,stby) \
		((unsigned char)mstp0 <<  0  ) |   \
		((unsigned char)mstp1 <<  1  ) |   \
		((unsigned char)mstp2 <<  2  ) |   \
		((unsigned char)mstp3 <<  3  ) |   \
		((unsigned char)mstp4 <<  4  ) |   \
		((unsigned char)ppu   <<  5  ) |   \
		((unsigned char)phz   <<  6  ) |   \
		((unsigned char)stby  <<  7  )

#define DC_SET_FRQCR(pfc,bfc,ifc,pll2en,pll1en,ck0en) \
		((unsigned short)pfc <<  0  ) |   \
		((unsigned short)bfc <<  3  ) |   \
		((unsigned short)ifc <<  6  ) |   \
		((unsigned short)pll2en <<  9  ) |   \
		((unsigned short)pll1en <<  10  ) |   \
		((unsigned short)ck0en  <<  11  )

#define PVRID	 				0X005F8000
#define PVRVER	 				0X005F8004
#define PVRRESET	 			0X005F8008
#define RENDER_START	 		0X005F8014 //Start render
#define TEST_SELECT		 		0X005F8018
#define RENDER_POLYBASE		 	0X005F8020 //Object buffer base offset
#define RENDER_TILEBASE		 	0X005F802C //Tile buffer base offset
#define RENDER_TSPCFG		 	0X005F8030
#define DISP_BORDER			 	0X005F8040
#define DISP_MODE			 	0X005F8044
#define RENDER_MODE				0X005F8048
#define RENDER_SIZE			 	0X005F804C
#define DISP_ADDR1				0X005F8050
#define DISP_ADDR2			 	0X005F8054
#define DISP_SIZE			 	0X005F805C
#define RENDER_ADDR1			0X005F8060
#define RENDER_ADDR2			0X005F8064
#define RENDER_HCLIP			0x005F8068 //Horizontal clipping area
#define RENDER_VCLIP			0x005F806C //Vertical clipping area
#define RENDER_SHADOW			0x005F8074 //Shadowing
#define RENDER_NEARCLIP			0x005F8078 //Object clip distance (float32)
#define RENDER_OBJCFG			0x005F807C //Object config
#define HALF_OFFSET				0x005F8080 //PVR2 unknown register 2
#define RENDER_TSPCLIP			0x005F8084 //Texture clip distance (float32)
#define RENDER_FARCLIP			0x005F8088 //Background plane depth (float32)
#define RENDER_BGPLANE			0x005F808C //Background plane config
#define RENDER_ISPCFG			0x005F8098 //ISP config
#define VRAM_CFG1				0x005F80A0 //VRAM config 1
#define VRAM_CFG2				0x005F80A4 //VRAM config 2
#define VRAM_CFG3				0x005F80A8 //VRAM config 3
#define RENDER_FOGTBLCOL		0x005F80B0 //Fog table colour
#define RENDER_FOGVRTCOL		0x005F80B4 //Fog vertex colour
#define RENDER_FOGCOEFF			0x005F80B8 //Fog density coefficient (float16)
#define RENDER_CLAMPHI			0x005F80BC //Clamp high colour
#define RENDER_CLAMPLO			0x005F80C0 //Clamp low colour
#define GUNPOS					0x005F80C4 //Lightgun position
#define DISP_HPOSIRQ			0x005F80C8 //Raster horizontal event position
#define DISP_VPOSIRQ			0x005F80CC //Raster event position
#define DISP_SYNCCFG			0x005F80D0 //Sync configuration & enable
#define DISP_HBORDER			0x005F80D4 //Horizontal border area
#define DISP_TOTAL				0x005F80D8 //Total display area
#define DISP_VBORDER			0x005F80DC //Vertical border area
#define DISP_SYNCTIME			0x005F80E0 //Horizontal sync pulse timing
#define RENDER_TEXSIZE			0x005F80E4 //Texture modulo width
#define DISP_CFG2				0x005F80E8 //Video configuration 2
#define DISP_HPOS				0x005F80EC //Horizontal display position
#define DISP_VPOS				0x005F80F0 //Vertical display position
#define RENDER_SCALER			0x005F80F4 //Scaler configuration (?)
#define RENDER_PALETTE			0x005F8108 //Palette configuration
#define DISP_SYNCSTAT			0x005F810C //Raster beam position
#define FB_BURSTCTRL			0x005F8110 //PVR2 unknown register 3
#define FB_C_SOF				0x005F8114 //PVR2 unknown register 4
#define Y_COEFF					0x005F8118 //PVR2 unkown register 5
#define RENDER_ALPHA_REF		0x005F811C //PVR2 reference alpha
#define TA_TILEBASE				0x005F8124 //TA Tile matrix start
#define TA_POLYBASE				0x005F8128 //TA Polygon buffer start
#define TA_LISTEND				0x005F812C //TA Tile matrix end
#define TA_POLYEND				0x005F8130 //TA Polygon buffer end
#define TA_LISTPOS				0x005F8134 //TA Tile list position
#define TA_POLYPOS				0x005F8138 //TA Polygon buffer position
#define TA_TILESIZE				0x005F813C //TA Tile matrix size
#define TA_TILECFG				0x005F8140 //TA Tile matrix config
#define TA_INIT					0x005F8144 //TA Initialize
#define YUV_ADDR				0x005F8148 //YUV conversion address
#define YUV_CFG					0x005F814C //YUV configuration
#define YUV_COUNT				0x005F8150 //YUV conversion count
#define TA_REINIT				0x005F8160 //TA re-initialize
#define TA_LISTBASE				0x005F8164 //TA Tile list start
#define PVRUNK7					0x005F81A8 //PVR2 unknown register 7
#define PVRUNK8					0x005F81AC //PVR2 unknown register 8
#define RENDER_FOGTABLE			0x005F8200 //Start of fog table


#define PALETTE_RAM				0x005F9000 //0X00
#define PALETTE_RAM_END			0x005F9FFF
/*
#define PVR_SOFTRESET	 		0X005F8008
#define PVR_STARTRENDER 		0X005F8014
#define PVR_PARAM_BASE		 	0X005F8020
#define PVR_REGIONBASE		 	0X005F802C
#define PVR_FB_W_SOF1 			0X005F8060
#define PVR_FB_X_CLIP 			0X005F8068
#define PVR_FB_Y_CLIP 			0X005F806C
#define PVR_ISP_BACKGROUND_D 	0X005F8088
#define PVR_ISP_BACKGROUND_T 	0X005F808C
*/


//for DMA
#define SB_PDSTAP 0X005F7C00
#define SB_PDSTAR 0X005F7C04
#define SB_PDLEN  0X005F7C08
#define SB_PDDIR  0X005F7C0C
#define SB_PDTSEL 0X005F7C10
#define SB_PDDEN  0X005F7C14
#define SB_PDST   0X005F7C18

#define DC_SET_PDSTAP(_dma) \
		((unsigned int)_dma <<  5  )

#define DC_SET_PDSTAR(_dma) \
		((unsigned int)_dma <<  5  )

#define DC_SET_PDLEN(_dma) \
		((unsigned int)_dma <<  5  )

#define DC_SET_PDDIR(_dma) \
		((unsigned int)_dma <<  0  )

#define DC_SET_PDTSEL(_dma) \
		((unsigned int)_dma <<  0  )

#define DC_SET_PDDEN(_dma) \
		((unsigned int)_dma <<  0  )

#define DC_SET_PDST(_dma) \
		((unsigned int)_dma <<  0  )




#define u32 unsigned int
#define TA_PARAMETER_CONTROL_WORD(para,group,obj) \
							((u32)obj   <<   0  ) | \
							((u32)group <<  16  ) | \
							((u32)para  <<  24  )


#define TA_PARA_EOS (0x10<<24)

#define TA_PARA_EOL (0x00<<24)
#define TA_PARA_UTC (0x20<<24)
#define TA_PARA_OLS (0x40<<24)
#define TA_PARA_POLYGON (0x80<<24)
#define TA_PARA_SPRITE  (0xA0<<24)
#define TA_PARA_VERTEX  (0xE0<<24)

#define TA_PARA_OPOLY  (0x00<<24)
#define TA_PARA_OMVOL  (0x01<<24)
#define TA_PARA_TPOLY  (0x02<<24)
#define TA_PARA_TMVOL  (0x03<<24)
#define TA_PARA_PUNCHT (0x04<<24)

#define TA_GROUP_ENABLE  (0x80<<16)

#define TA_GROUP_STRIPLEN1  (0x00<<16)
#define TA_GROUP_STRIPLEN2  (0x04<<16)
#define TA_GROUP_STRIPLEN4  (0x08<<16)
#define TA_GROUP_STRIPLEN6  (0x0C<<16)

#define TA_GROUP_USERTILECLIP_DISABLE	(0x00<<16)
#define TA_GROUP_USERTILECLIP_ENABLEIN   (0x02<<16)
#define TA_GROUP_USERTILECLIP_ENABLEOUT  (0x03<<16)


#define TA_OBJ_16BITUV   0x01
#define TA_OBJ_GOURAUD   0x02
#define TA_OBJ_OFFSET	0x04
#define TA_OBJ_TEXTURE   0x08

#define TA_OBJ_COLTYPE_8BIT	0x00
#define TA_OBJ_COLTYPE_FLOAT   0x10
#define TA_OBJ_COLTYPE_MODE1   0x20
#define TA_OBJ_COLTYPE_MODE2   0x30
#define TA_OBJ_VOLUME	0x40
#define TA_OBJ_SHADOW	0x80


#define TA_ISP_DEPTH_NEVER	(0x00<<29)
#define TA_ISP_DEPTH_LESS	 (0x01<<29)
#define TA_ISP_DEPTH_EQUAL	(0x02<<29)
#define TA_ISP_DEPTH_LEQUAL   (0x03<<29)
#define TA_ISP_DEPTH_GREATER  (0x04<<29)
#define TA_ISP_DEPTH_NOTEQUAL (0x05<<29)
#define TA_ISP_DEPTH_GEQUAL   (0x06<<29)
#define TA_ISP_DEPTH_ALWAYS   (0x07<<29)


#define TA_ISP_CULLING_NO		 (0x00<<27)
#define TA_ISP_CULLING_SMALL	  (0x01<<27)
#define TA_ISP_CULLING_NEGATIVE   (0x02<<27)
#define TA_ISP_CULLING_POSITIVE   (0x03<<27)

#define TA_ISP_ZWRITE_DISABLE	 (0x01<<26)
#define TA_ISP_TEXTURE			(0x01<<25)
#define TA_ISP_OFFSET			 (0x01<<24)
#define TA_ISP_GOURAUD			(0x01<<23)
#define TA_ISP_16BITUV			(0x01<<22)
#define TA_ISP_CACHE			  (0x01<<21)
#define TA_ISP_DCAK			   (0x01<<20)

#define TA_TSP_SRCALPHA(intr)	 (intr<<29)
#define TA_TSP_DSTALPHA(intr)	 (intr<<26)
#define TSP_ZERO		   0x00
#define TSP_ONE			0x01
#define TSP_OCOLOR		 0x02
#define TSP_INV_OCOLOR	 0x03
#define TSP_SRC_ALPHA	  0x04
#define TSP_INV_SRC_ALPHA  0x05
#define TSP_DST_ALPHA	  0x06
#define TSP_INV_DST_ALPHA  0x07


#define TA_TSP_SRC_SELECT		  (0x01<<25)
#define TA_TSP_DST_SELECT		  (0x01<<24)

#define TA_TSP_FOG_TABLE1		  (0x00<<22)
#define TA_TSP_FOG_VERTEX		  (0x01<<22)
#define TA_TSP_FOG_DISABLE		 (0x02<<22)
#define TA_TSP_FOG_TABLE2		  (0x03<<22)

#define TA_TSP_COLORCLAMP		  (0x01<<21)
#define TA_TSP_USEALPHA			(0x01<<20)
#define TA_TSP_TEXALPHAIGN		 (0x01<<19)
#define TA_TSP_FLIPV			   (0x01<<17)
#define TA_TSP_FLIPU			   (0x02<<17)
#define TA_TSP_CLAMPV			  (0x01<<15)
#define TA_TSP_CLAMPU			  (0x02<<15)
#define TA_TSP_FILTER_POINT		(0x00<<13)
#define TA_TSP_FILTER_BILINEAR	 (0x01<<13)
#define TA_TSP_FILTER_PASSA		(0x02<<13)
#define TA_TSP_FILTER_PASSB		(0x03<<13)
#define TA_TSP_SUPERSAMPLE		 (0x01<<12)
#define TA_TSP_MIPMAPD(value)	  (value<<08)
#define TA_TSP_TEXTURE_DECAL	   (0x00<<06)
#define TA_TSP_TEXTURE_MODULATE	(0x01<<06)
#define TA_TSP_TEXTURE_ADECAL	  (0x02<<06)
#define TA_TSP_TEXTURE_AMODULATE   (0x03<<06)
#define TA_TSP_USIZE(size)		 (size<<03)
#define TA_TSP_VSIZE(size)		 (size<<00)

#define TEXTURE_CONTROL_WORD_RGB(mipmapd,vqcompressed,pixelformat,scan,stride,address) \
							((u32)address	   <<   0  ) | \
							((u32)stride		<<  25  ) | \
							((u32)scan		  <<  26  ) | \
							((u32)pixelformat   <<  27  ) | \
							((u32)vqcompressed  <<  30  ) | \
							((u32)mipmapd	   <<  31  )

#define TEXTURE_CONTROL_WORD_PAL(mipmapd,vqcompressed,pixelformat,pal,address) \
							((u32)address	   <<   0  ) | \
							((u32)pal		   <<  21  ) | \
							((u32)pixelformat   <<  27  ) | \
							((u32)vqcompressed  <<  30  ) | \
							((u32)mipmapd	   <<  31  )


#define TA_TEXTURE_RGB1555 0
#define TA_TEXTURE_RGB565  1
#define TA_TEXTURE_RGB4444 2
#define TA_TEXTURE_YUV422  3
#define TA_TEXTURE_BUMPMAP 4
#define TA_TEXTURE_4BPP	5
#define TA_TEXTURE_8BPP	6


#define TA_TEXTURE_NORMAL	0x00
#define TA_TEXTURE_TWILLED   0x10
#define TA_TEXTURE_VQ		 0x20
void DC_Init();
void DC_Finish();
void DC_InitClip();
void DC_Matrix_Load(float *matrix);
void DC_SwapBuffers();
float DC_Zorder(float z,int flag);
unsigned int DC_vram_allocate(int width, int height, int psm);
void DC_twiddle_encode(AURAE_Texture *texture);


void DC_DrawModel(AURAE_Model *model);
void DC_Init_Poly(AURAE_Texture *texture,int para);
