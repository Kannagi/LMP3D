

#define DMA_SET_STAT(CIS,SIS,MEIS,BEIS,CIM,SIM,MEIM) \
	(u32)((CIS)  & 0x000003FF) <<  0  | (u32)((SIS)  & 0x00000001) <<  13 | \
	(u32)((MEIS) & 0x00000001) <<  14 | (u32)((BEIS) & 0x00000001) <<  15 | \
	(u32)((CIM)  & 0x000003FF) <<  16 | (u32)((SIM)  & 0x00000001) <<  29 | \
	(u32)((MEIM) & 0x00000001) <<  30


#define GS_SET_DMATAG(qwc,pce,id,irq,addr,spr) \
		((u64)qwc  <<  0  ) |	\
		((u64)pce  <<  26 ) |	\
		((u64)id   <<  28 ) |	\
		((u64)irq  <<  31 ) |	\
		((u64)addr <<  32 ) |	\
		((u64)spr  <<  63 )


#define GS_SET_TRXPOS(ssax,ssay,dsax,dsay,dir) \
		((u64)ssax <<  0  ) |	\
		((u64)ssay <<  16 ) |	\
		((u64)dsax <<  32 ) |	\
		((u64)dsay <<  48 ) |	\
		((u64)dir  <<  59 )

#define GS_SET_TRXREG(rrw,rrh) \
		((u64)rrw <<  0  ) |   \
		((u64)rrh <<  32 )

#define GS_SET_TRXDIR(xdir) \
		((u64)xdir <<  0  )

#define GS_SET_TEX0(tbp0, tpw,psm,tw,th,tcc,tfx,cbp,cpsm,csm,csa,cld) \
		((unsigned long)tbp0 <<  0 ) |	\
		((unsigned long)tpw  << 14 ) |	\
		((unsigned long)psm  << 20 ) |	\
		((unsigned long)tw   << 26 ) |	\
		((unsigned long)th   << 30 ) |	\
		((unsigned long)tcc  << 34 ) |	\
		((unsigned long)tfx  << 35 ) |	\
		((unsigned long)cbp  << 37 ) |	\
		((unsigned long)cpsm << 51 ) |	\
		((unsigned long)csm  << 55 ) |	\
		((unsigned long)csa  << 56 ) |	\
		((unsigned long)cld  << 61 )


#define GS_SET_TEX1(lcm,mxl,mmag,mmin,mtba,l,k) \
		((unsigned long)lcm   <<  0 ) |	\
		((unsigned long)mxl   <<  2 ) |	\
		((unsigned long)mmag  <<  5 ) |	\
		((unsigned long)mmin  <<  6 ) |	\
		((unsigned long)mtba  <<  9 ) |	\
		((unsigned long)l     << 19 ) |	\
		((unsigned long)k     << 32 )

#define GS_SET_TEX2(psm,cbp,cpsm,csm,csa,cld) \
		((unsigned long)psm  << 20 ) |	\
		((unsigned long)cbp  << 37 ) |	\
		((unsigned long)cpsm << 51 ) |	\
		((unsigned long)csm  << 55 ) |	\
		((unsigned long)csa  << 56 ) |	\
		((unsigned long)cld  << 61 )


#define GS_SET_TEXCLUT(cbw,cou,cov) \
		((u64)cbw  <<   0  ) | \
		((u64)cou  <<   6  ) | \
		((u64)cov  <<   12 )

#define GS_SET_TEXA(ta0,aem,ta1) \
		((u64)ta0  <<   0  ) | \
		((u64)aem  <<   15 ) | \
		((u64)ta1  <<   32 )

#define GS_SET_BITBLTBUF(sbp,sbw,spsm,dbp,dbw,dpsm) \
		((unsigned long)sbp  <<  0 ) |	\
		((unsigned long)sbw  << 16 ) |	\
		((unsigned long)spsm << 24 ) |	\
		((unsigned long)dbp  << 32 ) |	\
		((unsigned long)dbw  << 48 ) |	\
		((unsigned long)dpsm << 56 )


#define GS_SET_TRXREG(rrw,rrh) \
		((u64)rrw <<  0  ) |   \
		((u64)rrh <<  32 )

#define GS_SET_XYOFFSET(x,y) \
		((u64)x   <<  0 ) |	\
		((u64)y   << 32 )

#define GS_SET_CLAMPX(wms, wmt,minu,maxu,minv,maxv) \
		((u64)wms   <<  0 ) |	\
		((u64)wmt   <<  2 ) |	\
		((u64)minu  <<  4 ) |	\
		((u64)maxu  << 14 ) | \
		((u64)minv  << 24 ) |	\
		((u64)maxv  << 34 )

#define GS_SET_DISPLAYX(display_x, display_y,magnify_h,magnify_v,display_w,display_h) \
		((u64)display_x <<  0 ) |	\
		((u64)display_y << 12 ) |	\
		((u64)magnify_h << 23 ) |	\
		((u64)magnify_v << 27 ) |	\
		((u64)display_w << 32 ) |	\
		((u64)display_h << 44 )


#define GS_SET_SCISSORX(upper_x,lower_x,upper_y,lower_y) \
		((u64)upper_x   <<  0 ) |	\
		((u64)lower_x   << 16 ) |	\
		((u64)upper_y   << 32 ) |	\
		((u64)lower_y   << 48 )

#define GS_SET_FRAMEX(fbp, fbw,psm,fbmsk) \
		((u64)fbp   <<  0 ) |	\
		((u64)fbw   << 16 ) |	\
		((u64)psm   << 24 ) |	\
		((u64)fbmsk << 32 )


#define GS_SET_ZBUFX(zbp,psm,zmsk) \
		((u64)zbp   <<  0 ) |	\
		((u64)psm   << 24 ) |	\
		((u64)zmsk  << 32 )

#define GS_SET_PRMODECONT(ac) \
		((u64)ac    <<  0 )

#define GS_SET_DISPFBX(address, width, psm, x, y) \
		(u64)((address	) & 0x000001FF) <<  0 | \
		(u64)((width		) & 0x0000003F) <<  9 | \
		(u64)((psm	) & 0x0000001F) << 15 | \
		(u64)((x			) & 0x000007FF) << 32 | \
		(u64)((y			) & 0x000007FF) << 43


#define GS_SET_DISPFB1(address, width, psm, x, y) \
		(u64)((address	) & 0x000001FF) <<  0 | \
		(u64)((width		) & 0x0000003F) <<  9 | \
		(u64)((psm	) & 0x0000001F) << 15 | \
		(u64)((x			) & 0x000007FF) << 32 | \
		(u64)((y			) & 0x000007FF) << 43

#define GS_SET_DISPFB2(address, width, psm, x, y) \
		(u64)((address	) & 0x000001FF) <<  0 | \
		(u64)((width		) & 0x0000003F) <<  9 | \
		(u64)((psm	) & 0x0000001F) << 15 | \
		(u64)((x			) & 0x000007FF) << 32 | \
		(u64)((y			) & 0x000007FF) << 43

#define GS_SET_BGCOLOR(r,g,b) \
		(u64)((r) & 0x000000FF) <<  0 | \
		(u64)((g) & 0x000000FF) <<  8 | \
		(u64)((b) & 0x000000FF) << 16

#define GS_SET_GIFTAG(nloop, eop,pre,prim,flg,nreg) \
		((u64)nloop <<  0 ) |	\
		((u64)eop   << 15 ) |	\
		((u64)pre   << 46 ) |	\
		((u64)prim  << 47 ) |	\
		((u64)flg   << 58 ) |	\
		((u64)nreg  << 60 )

#define GS_SET_TEST(ate, atst,aref,afail,date,datm,zte,ztst) \
		((u64)ate   <<   0 ) |	\
		((u64)atst  <<   1 ) |	\
		((u64)aref  <<   4 ) |	\
		((u64)afail <<  12 ) |	\
		((u64)date  <<  14 ) |	\
		((u64)datm  <<  15 ) |	\
		((u64)zte   <<  16 ) |	\
		((u64)ztst  <<  17 )

#define GS_SET_PRIM(prim, iip,tme,fge,abe,aa1,fst,ctxt,fix) \
		((u64)prim <<  0 ) | \
		((u64)iip  <<  3 ) | \
		((u64)tme  <<  4 ) | \
		((u64)fge  <<  5 ) | \
		((u64)abe  <<  6 ) | \
		((u64)aa1  <<  7 ) | \
		((u64)fst  <<  8 ) | \
		((u64)ctxt <<  9 ) | \
		((u64)fix  <<  10 )


#define GS_SET_RGBAQ(R,G,B,A,Q) \
		((u64)R  <<   0 ) | \
		((u64)G  <<   8 ) | \
		((u64)B  <<  16 ) | \
		((u64)A  <<  24 ) | \
		((u64)Q  <<  32 )


#define GS_SET_UV(U,V) \
		((u64)U  <<    0 ) | \
		((u64)V  <<   16 )

#define GS_SET_XYZ(X,Y,Z) \
		((u64)X  <<   0  ) | \
		((u64)Y  <<   16 ) | \
		((u64)Z  <<   32 )

#define GS_SET_ST(S,T) \
		((u64)S  <<    0 ) | \
		((u64)T  <<   32 )

#define GS_SET_UV(U,V) \
		((u64)U  <<    0 ) | \
		((u64)V  <<   16 )



#define EE_SET_CHCR(dir,mod,asp,tte,tie,str,tag) \
		(unsigned int)(dir <<  0 ) /*to,from*/ |  \
		(unsigned int)(mod <<  2 ) /*Normal,Chain,Interleave*/| \
		(unsigned int)(asp <<  4 ) /*noadress,1adress,2adress call tag*/| \
		(unsigned int)(tte <<  6 ) /*not,transfert DMATAG*/| \
		(unsigned int)(tie <<  7 ) /*disable,enable irq*/| \
		(unsigned int)(str <<  8 ) /*Stop,start DMA*/| \
		(unsigned int)(tag << 16 ) /*TAG*/



#define EE_SET_ADR(addr,spr) \
		(unsigned int)((unsigned int)addr <<  0 ) | \
		(unsigned int)(spr <<  31 )

#define GS_SET_PMODE(EN1,EN2,CRTMD,MMOD,AMOD,SLBG,ALP) \
		(u32)(EN1   <<  0 ) /*read C1 OFF/ON */ |  \
		(u32)(EN2   <<  1 ) /*read C2OFF/ON */| \
		(u32)(CRTMD <<  2 ) /*CRT OUTPUT 1 */| \
		(u32)(MMOD  <<  5 ) /*Alpha Value RC1 , ALP reg*/| \
		(u32)(AMOD  <<  6 ) /*Alphe Value RC,Alphe Value RC2*/| \
		(u32)(SLBG  <<  7 ) /*Blended RC2 , Background*/| \
		(u32)(ALP   <<  8 ) /*Value Alpha*/

#define GS_SET_CSR(signal_evnt,finish_evnt,hsync_intrupt,vsync_intrupt,write_terminate,flush,reset,nfield,current_field,fifo_status,gs_rev_number,gs_id) \
			*(volatile u64 *)GS_CSR =				\
		(u64)((signal_evnt	) & 0x00000001) <<  0 | \
		(u64)((finish_evnt	) & 0x00000001) <<  1 | \
		(u64)((hsync_intrupt	) & 0x00000001) <<  2 | \
		(u64)((vsync_intrupt	) & 0x00000001) <<  3 | \
		(u64)((write_terminate) & 0x00000001) <<  4 | \
		(u64)((flush			) & 0x00000001) <<  8 | \
		(u64)((reset			) & 0x00000001) <<  9 | \
		(u64)((nfield			) & 0x00000001) << 12 | \
		(u64)((current_field	) & 0x00000001) << 13 | \
		(u64)((fifo_status	) & 0x00000003) << 14 | \
		(u64)((gs_rev_number	) & 0x000000FF) << 16 | \
		(u64)((gs_id			) & 0x000000FF) << 24


//PS2 Register
#define T0_COUNT 	0X10000000
#define T0_MODE 	0X10000010
#define T0_COMP	 	0X10000020
#define T0_HOLD 	0X10000030

#define T1_COUNT 	0X10000800
#define T1_MODE 	0X10000810
#define T1_COMP	 	0X10000820
#define T1_HOLD 	0X10000830

#define T2_COUNT 	0X10001000
#define T2_MODE 	0X10001010
#define T2_COMP	 	0X10001020

#define T3_COUNT 	0X10001800
#define T3_MODE 	0X10001810
#define T3_COMP	 	0X10001820

#define IPU_CMD 	0X10002000
#define IPU_CTRL 	0X10002010
#define IPU_BP	 	0X10002020
#define IPU_TOP	 	0X10002030

#define GIF_CTRL 	0X10003000
#define GIF_MODE 	0X10003010
#define GIF_STAT	0X10003020

#define GIF_TAG0 	0X10003040
#define GIF_TAG1 	0X10003050
#define GIF_TAG2	0X10003060
#define GIF_TAG3 	0X10003070
#define GIF_CNT 	0X10003080
#define GIF_P3CNT	0X10003090
#define GIF_P3TAG	0X100030A0

//VIF0 VIF1 FIFO
#define VIF1_MASK	0x10003C70

#define VIF1_TOPS	0x10003CC0
#define VIF1_ITOP	0x10003CD0
#define VIF1_TOP	0x10003CE0

#define GIF_FIFO	0X10006000

#define D0_CHCR 	0X10008000
#define D0_MADR 	0X10008010
#define D0_QWC	 	0X10008020
#define D0_TADR 	0X10008030
#define D0_ASR0 	0X10008040
#define D0_ASR1 	0X10008050
#define D0_SADR 	0X10008080

#define D1_CHCR 	0X10009000
#define D1_MADR 	0X10009010
#define D1_QWC	 	0X10009020
#define D1_TADR 	0X10009030
#define D1_ASR0 	0X10009040
#define D1_ASR1 	0X10009050
#define D1_SADR 	0X10009080

#define D2_CHCR 	0X1000A000
#define D2_MADR 	0X1000A010
#define D2_QWC	 	0X1000A020
#define D2_TADR 	0X1000A030
#define D2_ASR0 	0X1000A040
#define D2_ASR1 	0X1000A050
#define D2_SADR 	0X1000A080


#define D3_CHCR  	0x1000b000
#define D3_MADR  	0x1000b010
#define D3_QWC   	0x1000b020

#define D4_CHCR  	0x1000b400
#define D4_MADR  	0x1000b410
#define D4_QWC   	0x1000b420
#define D4_TADR  	0x1000b430

#define D5_CHCR  	0x1000c000
#define D5_MADR  	0x1000c010
#define D5_QWC   	0x1000c020

#define D6_CHCR  	0x1000c400
#define D6_MADR  	0x1000c410
#define D6_QWC   	0x1000c420
#define D6_TADR  	0x1000c430

#define D7_CHCR  	0x1000c800
#define D7_MADR  	0x1000c810
#define D7_QWC   	0x1000c820

#define D8_CHCR 	0X1000D000
#define D8_MADR 	0X1000D010
#define D8_QWC	 	0X1000D020
#define D8_TADR 	0X1000D030
#define D8_ASR0 	0X1000D040
#define D8_ASR1 	0X1000D050
#define D8_SADR 	0X1000D080

#define D9_CHCR 	0X1000D400
#define D9_MADR 	0X1000D410
#define D9_QWC	 	0X1000D420
#define D9_TADR 	0X1000D430
#define D9_ASR0 	0X1000D440
#define D9_ASR1 	0X1000D450
#define D9_SADR 	0X1000D480

#define D_CTRL	 	0X1000E000
#define D_STAT	 	0X1000E010
#define D_PCR	 	0X1000E020
#define D_SQWC	 	0X1000E030
#define D_RBSR	 	0X1000E040
#define D_RBOR 		0X1000E050
#define D_STADR 	0X1000E060

#define I_STAT 		0X1000F000
#define i_MASK 		0X1000F010

#define SB_SMFLG 	0X1000F230


#define D_ENABLER 	0X1000F520
#define D_ENABLEW 	0X1000F590

#define VU0_MicroMem 	0x11000000
#define VU0_Mem			0x11004000

#define VU1_MicroMem 	0x11008000
#define VU1_Mem			0x1100C000




#define GS_PMODE 	0X12000000
#define GS_SMODE1 	0X12000010
#define GS_SMODE2 	0X12000020
#define GS_SRFSH 	0X12000030
#define GS_SYNCH1 	0X12000040
#define GS_SYNCH2 	0X12000050
#define GS_SYNCV 	0X12000060
#define GS_DISPFB1 	0X12000070
#define GS_DISPLAY1 0X12000080
#define GS_DISPFB2 	0X12000090
#define GS_DISPLAY2 0X120000A0
#define GS_EXTBUF	0X120000B0
#define GS_EXTDATA 	0X120000C0
#define GS_EXTWRITE 0X120000D0
#define GS_BGCOLOR 	0X120000E0

#define GS_CSR 		0x12001000
#define GS_IMR 		0x12001010

#define GS_BUSDIR 	0x12001040

#define GS_SIGBLID 	0x12001080


//Defines PSM
#define GS_PSMCT32		0x00
#define GS_PSMCT24		0x01
#define GS_PSMCT16		0x02
#define GS_PSMCT16S 	0x0A

#define GS_PSMT8		0x13
#define GS_PSMT4		0x14
#define GS_PSMT8H		0x1B
#define GS_PSMT4HL		0x24
#define GS_PSMT4HH		0x2C

#define GS_PSMZ32		0x30
#define GS_PSMZ24		0x31
#define GS_PSMZ16		0x32
#define GS_PSMZ16S		0x3A


#define GS_PRIM_POINT 			0X00
#define GS_PRIM_LINE 			0x01
#define GS_PRIM_LINE_STRIP 		0X02
#define GS_PRIM_TRIANGLE	 	0x03
#define GS_PRIM_TRIANGLE_STRIP 	0x04
#define GS_PRIM_TRIANGLE_FAN 	0x05
#define GS_PRIM_SPRITE 			0X06


#define GS_IIP_FLAT		0X00
#define GS_IIP_GOURAUD	0X01

#define GS_TME_TEXTURE_OFF	0X00
#define GS_TME_TEXTURE_ON	0X01

#define GS_FGE_FOGGING_OFF	0X00
#define GS_FGE_FOGGING_ON	0X01

#define GS_ABE_ALPHA_OFF	0X00
#define GS_ABE_ALPHA_ON		0X01

#define GS_AA1_ANTIALIASING_OFF	0X00
#define GS_AA1_ANTIALIASING_ON	0X01

#define GS_FST_STQ	0X00
#define GS_FST_UV	0X01

#define GS_CTXT_0	0X00
#define GS_CTXT_1	0X01

#define GS_FIX_0	0X00
#define GS_FIX_1	0X01

//Defines GS Reg
#define GS_REG_PRIM		 	0x00
#define GS_REG_RGBAQ		0x01
#define GS_REG_ST			0x02
#define GS_REG_UV			0x03
#define GS_REG_XYZF2		0x04
#define GS_REG_XYZ2			0x05
#define GS_REG_TEX0_1		0x06
#define GS_REG_TEX0_2		0x07
#define GS_REG_CLAMP_1	 	0x08
#define GS_REG_CLAMP_2	 	0x09
#define GS_REG_FOG			0X0A
#define GS_REG_XYZF3		0x0C
#define GS_REG_XYZ3			0x0D
#define GS_REG_AD			0X0E
#define GS_REG_NOP			0X0F

#define	GS_REG_TEX1_1		0x14
#define	GS_REG_TEX1_2		0x15
#define	GS_REG_TEX2_1		0x16
#define	GS_REG_TEX2_2		0x17
#define GS_REG_XYOFFSET_1 	0x18
#define GS_REG_XYOFFSET_2 	0x19
#define	GS_REG_PRMODECONT	0x1A
#define	GS_REG_PRMODE		0x1B
#define	GS_REG_TEXCLUT		0x1C

#define	GS_REG_SCANMSK		0x22

#define	GS_REG_MIPTBP1_1	0x34
#define	GS_REG_MIPTBP1_2	0x35
#define	GS_REG_MIPTBP2_1	0x36
#define	GS_REG_MIPTBP2_2	0x37

#define	GS_REG_TEXA			0x3B

#define	GS_REG_FOGCOL		0x3D

#define	GS_REG_TEXFLUSH		0x3F
#define GS_REG_SCISSOR_1 	0x40
#define GS_REG_SCISSOR_2 	0x41
#define GS_REG_ALPHA_1		0x42
#define GS_REG_ALPHA_2		0x43
#define	GS_REG_DIMX			0x44
#define	GS_REG_DTHE			0x45
#define GS_REG_COLCLAMP		0x46
#define	GS_REG_TEST_1		0x47
#define	GS_REG_TEST_2		0x48
#define	GS_REG_PABE			0x49
#define	GS_REG_FBA_1		0x4A
#define	GS_REG_FBA_2		0x4B
#define	GS_REG_ZBUF_1		0x4E
#define	GS_REG_ZBUF_2		0x4F
#define GS_REG_FRAME_1	 	0x4C
#define GS_REG_FRAME_2	 	0x4D

#define	GS_REG_BITBLTBUF	0x50
#define	GS_REG_TRXPOS		0x51
#define	GS_REG_TRXREG		0x52
#define	GS_REG_TRXDIR		0x53
#define	GS_REG_HWREG		0x54

#define	GS_REG_SIGNAL		0x60
#define	GS_REG_FINISH		0x61
#define	GS_REG_LABEL		0x62





// do we need these? (grabbed from GS user manual)
#define	GS_REG_BGCOLOR		0x0e
#define	GS_REG_BUSDIR		0x44
#define	GS_REG_CSR		0x40
#define	GS_REG_DISPFB1		0x07
#define	GS_REG_DISPFB2		0x09
#define	GS_REG_DISPLAY1		0x08
#define	GS_REG_DISPLAY2		0x0a
#define	GS_REG_EXTBUF		0x0b
#define	GS_REG_EXTDATA		0x0c
#define	GS_REG_EXTWRITE		0x0d
#define	GS_REG_IMR		0x41
#define	GS_REG_PMODE		0x00
#define	GS_REG_SIGBLID		0x48
#define	GS_REG_SMODE2		0x02



#define DRAW_DISABLE 	0
#define DRAW_ENABLE 	1


#define ATEST_KEEP_FRAMEBUFFER 	2
#define ZTEST_METHOD_ALLPASS 	1

#define ATEST_METHOD_EQUAL 	4
#define ATEST_METHOD_GREATER_EQUAL 5
#define ZTEST_METHOD_GREATER_EQUAL 2
#define ZTEST_METHOD_GREATER 3

//DMA
#define DMA_CALL_TAG(Addr, iQuadCount)	((((u64)(Addr)) << 32) | (0x5 << 28) | iQuadCount)
#define DMA_RET_TAG(iQuadCount)					((((u64)(0x6)) << 28) | iQuadCount)
#define DMA_REF_TAG(ADDR, COUNT) ((((unsigned long)ADDR) << 32) | (0x3 << 28) | COUNT )
#define DMA_CNT_TAG(COUNT)       (((unsigned long)(0x1) << 28) | COUNT)
#define DMA_END_TAG(COUNT)       (((unsigned long)(0x7) << 28) | COUNT)
#define DMA_CHCR(DIR,MOD,ASP,TTE,TIE,STR) ( (((u32)DIR)<<0) | (((u32)MOD)<<2) | \
                                            (((u32)ASP)<<4) | (((u32)TTE)<<6) | \
                                            (((u32)TIE)<<7) | (((u32)STR)<<8) )

//VIF
#define VIF_NOP			0x00
#define VIF_STCYL		0x01
#define VIF_OFFSET		0x02
#define VIF_BASE		0x03
#define VIF_ITOP        0x04
#define VIF_STMOD		0x05
#define VIF_MSKPATH3	0x06
#define VIF_MARK		0x07

#define VIF_FLUSHE		0x10
#define VIF_FLUSH		0x11
#define VIF_FLUSHA		0x13
#define VIF_MSCAL		0x14
#define VIF_MSCALF		0x15
#define VIF_MSCNT		0x17

#define VIF_STMASK		0x20
#define VIF_STROW		0x30
#define VIF_STCOL		0x31

#define VIF_MPG			0x4A
#define VIF_DIRECT  	0x50
#define VIF_DIRECTHL	0x51

///////////////////////////////////////////////////////////////////////////////
// Unpack related defines
///////////////////////////////////////////////////////////////////////////////

#define VIF_UNPACK			0x60
#define VIF_UNPACK_MASK		0x10

#define VIF_UNPACK_DBLBUF	0x8000

#define VIF_UNPACK_S_32		(VIF_UNPACK|0x00)
#define VIF_UNPACK_S_16		(VIF_UNPACK|0x01)
#define VIF_UNPACK_S_8		(VIF_UNPACK|0x02)
#define VIF_UNPACK_V2_32	(VIF_UNPACK|0x04)
#define VIF_UNPACK_V2_16	(VIF_UNPACK|0x05)
#define VIF_UNPACK_V2_8		(VIF_UNPACK|0x06)
#define VIF_UNPACK_V3_32	(VIF_UNPACK|0x08)
#define VIF_UNPACK_V3_16	(VIF_UNPACK|0x09)
#define VIF_UNPACK_V3_8		(VIF_UNPACK|0x0A)
#define VIF_UNPACK_V4_32	(VIF_UNPACK|0x0C)
#define VIF_UNPACK_V4_16	(VIF_UNPACK|0x0D)
#define VIF_UNPACK_V4_8		(VIF_UNPACK|0x0E)
#define VIF_UNPACK_V4_5		(VIF_UNPACK|0x0F)

///////////////////////////////////////////////////////////////////////////////
// Unpack mode defines
///////////////////////////////////////////////////////////////////////////////

#define VIF_MODE_NORMAL     0x00
#define VIF_MODE_OFFSET     0x01
#define VIF_MODE_DIFFERENCE 0x02

///////////////////////////////////////////////////////////////////////////////
// Misc defines
///////////////////////////////////////////////////////////////////////////////

#define VIF_CODE(CMD,NUM,IMMEDIATE) ((((unsigned int)(CMD))<<24) | \
                                     (((unsigned int)(NUM))<<16) | \
                                      ((unsigned int)(IMMEDIATE)))

#define VIF_STAT_PROGRAM_RUNNING  0x04
#define VIF_STAT_UNPACKING		  0x03
#define VIF_STAT_GIF_TRANSFERING  0x08

//Structure
typedef struct
{
    int calculation,max_level;
    int mag_filter,min_filter,mipmap_select;
    int l,k;

}PS2_lod;

typedef struct
{
    int storage_mode,start;
    int psm;
    int load_method,address;

}PS2_clut;




void PS2_Pad_Init();
int PS2_vram_allocate(int width, int height, int psm);
void PS2_VU_Init();
void PS2_Graphic_Setup(LMP3D_Buffer buffer);
void PS2_Graphic_Init(int width, int height, int psm,int fbaddr);
void PS2_Init_Pad();
void PS2_VU_Draw(float* matrix,LMP3D_Model *model);


#define CACHED_SEG(x)   ( (void*) ( (unsigned int)(x) | (0xC0000000) ) )

