
#define u32 unsigned int


#define GP0_CMD_COLOR(cmd,r,g,b) \
		(r	<<  0  ) |	\
		(g	<<  8  ) |	\
		(b	<<  16 ) |	\
		(cmd  <<  24 )

#define GP0_VERTEX(x,y) \
		(x	<<  0  ) |	\
		(y	<<  16 )

#define GP0_TEXCOORD(x,y,arg) \
		(x	<<  0  ) |	\
		(y	<<  8  ) |	\
		(arg  <<  16 )

#define GP0_TEXPAGE(tpx,tpy,sm,tpc,dbit,dda,td,trx,try) \
		(tpx  <<  0  ) |	\
		(tpy  <<  4  ) |	\
		(sm   <<  5  ) |	\
		(tpc  <<  7  ) |	\
		(dbit <<  9  ) |	\
		(dda  <<  10 ) |	\
		(td   <<  11 ) |	\
		(trx  <<  12 ) |	\
		(try  <<  13 ) |	\
		(0xE1 <<  24 )


#define DMA_CHCR_SET(td,mas,ce,ts,dmaws,cpuws,dmast,smst,unk1,unk2) \
		(td	<<  0  ) |	\
		(mas   <<  1  ) |	\
		(ce	<<  8  ) |	\
		(ts	<<  9  ) |	\
		(dmaws <<  16 ) |	\
		(cpuws <<  20 ) |	\
		(dmast <<  24 ) |	\
		(smst  <<  28 ) |	\
		(unk1  <<  29 ) |	\
		(unk2  <<  30 )

#define SCRATCHPAD	0x1F800000

//Interrupt Control
#define I_STAT		0x1F801070
#define I_MASK		0x1F801074

//DMA Registers
#define D0_MADR		0x1F801080
#define D0_BCR		0x1F801084
#define D0_CHCR		0x1F801088

#define D1_MADR		0x1F801090
#define D1_BCR		0x1F801094
#define D1_CHCR		0x1F801098

#define D2_MADR		0x1F8010A0
#define D2_BCR		0x1F8010A4
#define D2_CHCR		0x1F8010A8

#define D3_MADR		0x1F8010B0
#define D3_BCR		0x1F8010B4
#define D3_CHCR		0x1F8010B8

#define D4_MADR		0x1F8010C0
#define D4_BCR		0x1F8010C4
#define D4_CHCR		0x1F8010C8

#define D5_MADR		0x1F8010D0
#define D5_BCR		0x1F8010D4
#define D5_CHCR		0x1F8010D8

#define D6_MADR		0x1F8010E0
#define D6_BCR		0x1F8010E4
#define D6_CHCR		0x1F8010E8

#define DPCR		0x1F8010F0
#define DICR		0x1F8010F4

#define TIMERV0		0x1F801100
#define TIMERM0		0x1F801104

#define TIMERV1		0x1F801110
#define TIMERM1		0x1F801114

#define TIMERV2		0x1F801120
#define TIMERM2		0x1F801124

//GPU Registers
#define GP0			0x1F801810
#define GP1			0x1F801814

// GPU COMMANDS
//M = Monochrome , T=Textured , S = Shaded
//O = Opaque ,N = semi-transparent
//R = Raw-texture , B = texture-Blending

#define GP0_TRIANGLE_MO			0x20
#define GP0_TRIANGLE_MN			0x22
#define GP0_TRIANGLE_TOB		0x24
#define GP0_TRIANGLE_TOR		0x25
#define GP0_TRIANGLE_TNB		0x26
#define GP0_TRIANGLE_TNR		0x27

#define GP0_TRIANGLE_STRIP_MO	0x28
#define GP0_TRIANGLE_STRIP_MN	0x2A
#define GP0_TRIANGLE_STRIP_TOB	0x2C
#define GP0_TRIANGLE_STRIP_TOR	0x2D
#define GP0_TRIANGLE_STRIP_TNB	0x2E
#define GP0_TRIANGLE_STRIP_TNR	0x2F

#define GP0_TRIANGLE_SO			0x30
#define GP0_TRIANGLE_SN			0x32
#define GP0_TRIANGLE_STOB		0x34
#define GP0_TRIANGLE_STNB		0x36

#define GP0_TRIANGLE_STRIP_SO	0x38
#define GP0_TRIANGLE_STRIP_MSN	0x3A
#define GP0_TRIANGLE_STRIP_STOB	0x3C
#define GP0_TRIANGLE_STRIP_STNB	0x3E

#define GP0_RECTANGLE_MO		0x60
#define GP0_RECTANGLE_MN		0x62
#define GP0_RECTANGLE_TOB		0x64
#define GP0_RECTANGLE_TOR		0x65
#define GP0_RECTANGLE_TNB		0x66
#define GP0_RECTANGLE_TNR		0x67

#define GP0_RECTANGLE_MO1		0x68
#define GP0_RECTANGLE_MN1		0x6A
#define GP0_RECTANGLE_TOB1		0x6C
#define GP0_RECTANGLE_TOR1		0x6D
#define GP0_RECTANGLE_TNB1		0x6E
#define GP0_RECTANGLE_TNR1		0x6F

#define GP0_RECTANGLE_MO8		0x70
#define GP0_RECTANGLE_MN8		0x72
#define GP0_RECTANGLE_TOB8		0x74
#define GP0_RECTANGLE_TOR8		0x75
#define GP0_RECTANGLE_TNB8		0x76
#define GP0_RECTANGLE_TNR8		0x77

#define GP0_RECTANGLE_MO16		0x78
#define GP0_RECTANGLE_MN16		0x7A
#define GP0_RECTANGLE_TOB16		0x7C
#define GP0_RECTANGLE_TOR16		0x7D
#define GP0_RECTANGLE_TNB16		0x7E
#define GP0_RECTANGLE_TNR16		0x7F

#define PS1_PSM4	0x00
#define PS1_PSM8	0x01
#define PS1_PSM16	0x02


#define D_CHCR_TO_RAM		   0
#define D_CHCR_FROM_RAM		 (1 << 0)
#define D_CHCR_FORWARD		  0
#define D_CHCR_BACKWARD		 (1 << 1)
#define D_CHCR_NO_CHOP		  0
#define D_CHCR_CHOP			 (1 << 8)
#define D_CHCR_SYNC_IMM		 0
#define D_CHCR_SYNC_DEV		 (1 << 9) /* transfer blocks, device-controlled */
#define D_CHCR_SYNC_LL		  (2 << 9) /* transfer linked list, device-controlled */
#define D_CHCR_CHOP_DMA_SIZE(x) (((x) & 7) << 16) /* size of dma window in words */
#define D_CHCR_CHOP_CPU_SIZE(x) (((x) & 7) << 20) /* size of cpu window in clock cycles (33MHz?) */
#define D_CHCR_START			(1 << 24) /* "master" trigger for transfers */
#define D_CHCR_BUSY			 (1 << 24) /* gets reset once transfer is complete */
#define D_CHCR_IMM_TRIG		 (1 << 28) /* trigger for immediate transfers, combine with START */
#define D_CHCR_IMM_PAUSE		(1 << 29) /* pause for imm transfers? not verified, only makes sense with CHOP */

#define DICR_CH_ENABLE_OFFSET   16
#define DICR_MASTER_ENABLE	  (1 << 23)
#define DICR_CH_FLAG_OFFSET	 24

#define GPUREAD GP0
#define GPUSTAT GP1

#define GPUSTAT_CMD_READY	  (1 << 26)
#define GPUSTAT_DMA_READY	  (1 << 28)

#define GP0_COPY_CPU_TO_VRAM 0xA0

#define GP1_RESET_CMD_BUFFER   0x01
#define GP1_DISPLAY_AREA_START 0x05
#define GP1_HRANGE			 0x06
#define GP1_VRANGE			 0x07
#define GP1_DISPLAY_MODE	   0x08

#define EQU_GTE  ".equ RTPS  ,0x0180001\n	" \
				 ".equ RTPT  ,0x0280030\n	" \
				 ".equ NCLIP ,0x1400006\n	" \
				 ".equ AVSZ3 ,0x158002D\n	" \
				 ".equ AVSZ4 ,0x168002E\n	" \
				 ".equ MVMVA ,0x0400012\n	" \
				 ".equ SQR   ,0x0A00428\n	" \
				 ".equ OP	,0x0170000\n	" \
				 ".equ CMD_SF ,1<<19\n	" \
				 ".equ CMD_MMR ,0<<17\n	" \
				 ".equ CMD_MML ,1<<17\n	" \
				 ".equ CMD_MMC ,2<<17\n	" \
				 ".equ CMD_MV0 ,0<<15\n	" \
				 ".equ CMD_MV1 ,1<<15\n	" \
				 ".equ CMD_MV2 ,2<<15\n	" \
				 ".equ CMD_MVIR ,3<<15\n	" \
				 ".equ CMD_TVTR ,0<<13\n	" \
				 ".equ CMD_TVBK ,1<<13\n	" \
				 ".equ CMD_TVFC ,2<<13\n	" \
				 ".equ CMD_LM ,1<<10\n	" \
				 ".equ VXY0 ,$0 \n	" \
				 ".equ VZ0  ,$1 \n	" \
				 ".equ VXY1 ,$2 \n	" \
				 ".equ VZ1  ,$3 \n	" \
				 ".equ VXY2 ,$4 \n	" \
				 ".equ VZ2  ,$5 \n	" \
				 ".equ RGB  ,$6 \n	" \
				 ".equ OTZ  ,$7 \n	" \
				 ".equ IR0  ,$8 \n	" \
				 ".equ IR1  ,$9 \n	" \
				 ".equ IR2  ,$10 \n	" \
				 ".equ IR3  ,$11 \n	" \
				 ".equ SXY0 ,$12 \n	" \
				 ".equ SXY1 ,$13 \n	" \
				 ".equ SXY2 ,$14 \n	" \
				 ".equ SXYP ,$15 \n	" \
				 ".equ SZ0  ,$16 \n	" \
				 ".equ SZ1  ,$17 \n	" \
				 ".equ SZ2  ,$18 \n	" \
				 ".equ SZ3  ,$19 \n	" \
				 ".equ RGB0 ,$20 \n	" \
				 ".equ RGB1 ,$21 \n	" \
				 ".equ RGB2 ,$22 \n	" \
				 ".equ RES1 ,$23 \n	" \
				 ".equ MAC0 ,$24 \n	" \
				 ".equ MAC1 ,$25 \n	" \
				 ".equ MAC2 ,$26 \n	" \
				 ".equ MAC3 ,$27 \n	" \
				 ".equ IRGB ,$28 \n	" \
				 ".equ ORGB ,$29 \n	" \
				 ".equ LZCS ,$30 \n	" \
				 ".equ LZVR ,$31 \n	" \
				 ".equ R11R12 ,$0 \n	" \
				 ".equ R13R21 ,$1 \n	" \
				 ".equ R22R23 ,$2 \n	" \
				 ".equ R31R32 ,$3 \n	" \
				 ".equ R33	,$4 \n	" \
				 ".equ TRX	,$5 \n	" \
				 ".equ TRY	,$6 \n	" \
				 ".equ TRZ	,$7 \n	" \
				 ".equ L11L12 ,$8 \n	" \
				 ".equ L13L21 ,$9 \n	" \
				 ".equ L22L23 ,$10 \n	" \
				 ".equ L31L32 ,$11 \n	" \
				 ".equ L33	,$12 \n	" \
				 ".equ RBK	,$13 \n	" \
				 ".equ GBK	,$14 \n	" \
				 ".equ BBK	,$15 \n	" \
				 ".equ LR1LR2 ,$16 \n	" \
				 ".equ LR3LG1 ,$17 \n	" \
				 ".equ LG2LG3 ,$18 \n	" \
				 ".equ LB1LB2 ,$19 \n	" \
				 ".equ LB3	,$20 \n	" \
				 ".equ RFC	,$21 \n	" \
				 ".equ GFC	,$22 \n	" \
				 ".equ BFC	,$23 \n	" \
				 ".equ OFX	,$24 \n	" \
				 ".equ OFY	,$25 \n	" \
				 ".equ H	  ,$26 \n	" \
				 ".equ DQA	,$27 \n	" \
				 ".equ DQB	,$28 \n	" \
				 ".equ ZSF3   ,$29 \n	" \
				 ".equ ZSF4   ,$30 \n	" \
				 ".equ FLAG   ,$31 \n	" \
				 ".equ C0_STATUS   ,$12 \n	"

#define RW_REGISTER_U32(REG) 	*((volatile u32 *)(REG))
#define RW_REGISTER_32(REG) 	*((volatile int *)(REG))

void PS1_GTE_Init();
void PS1_GTE_Perspective_Set();
void PS1_GTE_MulMat(void *m1,void *m2,void *mr);
void PS1_GTE_ModelView_Set(void *rotate,void *translate);
unsigned int PS1_vram_allocate(int width, int height, int psm);
void PS1_BufferDraw();
void PS1_GsSetDispEnv(int x,int y);
void PS1_GsSetDrawEnv(int x,int y,int w,int h);
void PS1_Buffer_Init();
