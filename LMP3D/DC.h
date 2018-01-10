
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
//OXOOO0 0000 - 0X00FF FFFF AICA-Wave Memory 2/8MB

//OXOOO5 F7C0 - 0X005F 70FF Ext Device 16MB
//OXOOO5 F800 - 0X03FF FFFF Image Area 32MB

//OXO400 0000 - 0X047F FFFF Tex Mem 64bit Acc 8MB
//OXO500 0000 - 0X057F FFFF Tex Mem 32bit Acc 8MB

//OXOC00 0000 - 0X0C7F FFFF System Memory 16MB

//OX1000 0000 - 0X107F FFFF TA FIFO Polygon Cnv 8MB
//OX1080 0000 - 0X10FF FFFF TA FIFO YUV Conv  8MB
//OX1100 0000 - 0X11FF FFFF Tex Mem 32/64Bit Acc 16 MB

#define FRQCR 0XFFC00000
#define STBCR 0XFFC00004



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




#define RW_REGISTER_U32(REG) 	*((volatile unsigned int *)(REG))
