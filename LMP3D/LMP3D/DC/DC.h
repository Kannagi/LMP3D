
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

#define UNCACHED_P2(x) ( (void*) ( (unsigned int)(x) | (0xA0000000) ) )
#define CACHED_P3(x)   ( (void*) ( (unsigned int)(x) | (0xC0000000) ) )
#define UNCACHED_P4(x) ( (void*) ( (unsigned int)(x) | (0xE0000000) ) )

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


#define DC_P4 0xA0000000

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


//for Palette
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


#define RW_REGISTER_FLOAT(REG) 	*((volatile float *)(REG))
#define RW_REGISTER_U8(REG) 	*((volatile u8  *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile u16 *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile int *)(REG))
#define RW_REGISTER_U64(REG) 	*((volatile u64 *)(REG))
