#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION1
#include <psx.h>

#include "LMP3D/LMP3D.h"

#include "LMP3D/PS1/PS1.h"
asm(EQU_GTE);

void PS1_GTE_Init()
{
	asm  (

	// init
	"mfc0 $t0,C0_STATUS\n	"
	"li	$t1,0x40000000\n	"
	"or	$t0,$t0,$t1\n	"

	"mtc0 $t0,C0_STATUS\n	"

	"li	$t0,0\n	"


	"mtc2 $t0,VXY0\n	"
	"mtc2 $t0,VXY1\n	"
	"mtc2 $t0,VXY2\n	"
	"mtc2 $t0,VZ0\n	"
	"mtc2 $t0,VZ1\n	"
	"mtc2 $t0,VZ2\n	"

	"mtc2 $t0,IR0\n	"
	"mtc2 $t0,IR1\n	"
	"mtc2 $t0,IR2\n	"
	"mtc2 $t0,IR3\n	"


	"ctc2 $t0,TRX\n	"
	"ctc2 $t0,TRY\n	"
	"ctc2 $t0,TRZ\n	"

	"ctc2 $t0,RBK\n	"
	"ctc2 $t0,GBK\n	"
	"ctc2 $t0,BBK\n	"


	"li	$t0,1<<12\n	"

	"ctc2 $t0,ZSF3\n	"
	"ctc2 $t0,ZSF4\n	"

	 );
}

//int extPerspective[5];
void __attribute__((optimize("-O0"), noinline))PS1_GTE_Perspective_Set()
{
	int perspective[5];
	perspective[0] = (320/2)<<16;
	perspective[1] = (240/2)<<16;
	perspective[2] = 0<<8;
	perspective[3] = 0<<24;
	perspective[4] = 1<<9;
/*

	perspective[2] = extPerspective[2];
	perspective[3] = extPerspective[3];
/*
	perspective[0] = extPerspective[0];
	//perspective[1] = extPerspective[1];
	perspective[2] = extPerspective[2];
	perspective[3] = extPerspective[3];*/
	//perspective[4] = extPerspective[4];

	asm (

	//Perspective
	"lw	$t0,0(%0)\n	"
	"lw	$t1,4(%0)\n	"
	"lw	$t2,8(%0)\n	"
	"lw	$t3,12(%0)\n	"
	"lw	$t4,16(%0)\n	"
	"ctc2 $t0,OFX\n	"
	"ctc2 $t1,OFY\n	"
	"ctc2 $t2,DQA\n	"
	"ctc2 $t3,DQB\n	"
	"ctc2 $t4,H\n	"

	 :: "r"(perspective) :"memory" );
}


void PS1_GTE_MulMat(void *m1,void *m2,void *mr)
{
	asm (

	//MATRIX A
	"lw	$t0,0(%0)\n	"
	"lw	$t1,4(%0)\n	"
	"lw	$t2,8(%0)\n	"
	"lw	$t3,12(%0)\n	"
	"lw	$t4,16(%0)\n	"
	"ctc2 $t0,R11R12\n	"
	"ctc2 $t1,R13R21\n	"
	"ctc2 $t2,R22R23\n	"
	"ctc2 $t3,R31R32\n	"
	"ctc2 $t4,R33\n	"


	//MATRIX B
	"lwc2	VXY0,0(%1)\n	"
	"lwc2	VZ0,4(%1)\n	"

	"lwc2	VXY1,6(%1)\n	"
	"lwc2	VZ1,10(%1)\n	"

	"lwc2	VXY2,12(%1)\n	"
	"lwc2	VZ2,16(%1)\n	"

	"cop2 MVMVA+CMD_MMR+CMD_TVTR+CMD_MV0+CMD_SF\n	"

	"swc2	IR1,0(%2)\n	"
	"swc2	IR2,2(%2)\n	"
	"swc2	IR3,4(%2)\n	"

	"cop2 MVMVA+CMD_MMR+CMD_TVTR+CMD_MV1+CMD_SF\n	"

	"swc2	IR1,6(%2)\n	"
	"swc2	IR2,8(%2)\n	"
	"swc2	IR3,10(%2)\n	"

	"cop2 MVMVA+CMD_MMR+CMD_TVTR+CMD_MV2+CMD_SF\n	"

	"swc2	IR1,12(%2)\n	"
	"swc2	IR2,14(%2)\n	"
	"swc2	IR3,16(%2)\n	"

	 :: "r"(m1),"r"(m2),"r"(mr) );

}

void PS1_GTE_ModelView_Set(void *rotate,void *translate)
{

	asm volatile (

	//Rotate
	"lw	$t0,0(%0)\n	"
	"lw	$t1,4(%0)\n	"
	"lw	$t2,8(%0)\n	"
	"lw	$t3,12(%0)\n	"
	"lw	$t4,16(%0)\n	"
	"ctc2 $t0,R11R12\n	"
	"ctc2 $t1,R13R21\n	"
	"ctc2 $t2,R22R23\n	"
	"ctc2 $t3,R31R32\n	"
	"ctc2 $t4,R33\n	"

	//Translate
	"lw	$t0,0(%1)\n	"
	"lw	$t1,4(%1)\n	"
	"lw	$t2,8(%1)\n	"
	"ctc2 $t0,TRX\n	"
	"ctc2 $t1,TRY\n	"
	"ctc2 $t2,TRZ\n	"

	 :: "r"(rotate),"r"(translate));

}

#endif

