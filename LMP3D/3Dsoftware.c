#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"
#include <stdint.h>

//#define
inline uint64_t rdtsc()
{
	register uint64_t a = 0, d = 0;
	//asm volatile("mfence");
#if defined(USE_RDTSCP) && defined(__x86_64__)
	asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
#elif defined(USE_RDTSCP) && defined(__i386__)
	asm volatile("rdtscp" : "=A"(a), :: "ecx");
#elif defined(__x86_64__)
	asm volatile("rdtsc" : "=a"(a), "=d"(d));
#elif defined(__i386__)
	asm volatile("rdtsc" : "=A"(a));
#endif
	a = (d << 32) | a;
	//asm volatile("mfence");
	return a;
}

inline void trianglesort3(int triarray[8],int *tri)
{
	if(tri[1] < tri[3])
	{
		if(tri[1] < tri[5])
		{

			triarray[0] = tri[0];
			triarray[1] = tri[1];

			if(tri[3] < tri[5])
			{
				triarray[2] = tri[2];
				triarray[3] = tri[3];

				triarray[4] = tri[4];
				triarray[5] = tri[5];

				return;
			}else
			{
				triarray[2] = tri[4];
				triarray[3] = tri[5];

				triarray[4] = tri[2];
				triarray[5] = tri[3];

				return;
			}

		}else
		{
			triarray[0] = tri[4];
			triarray[1] = tri[5];

			triarray[2] = tri[0];
			triarray[3] = tri[1];

			triarray[4] = tri[2];
			triarray[5] = tri[3];

			return;
		}
	}else
	{
		if(tri[3] < tri[5])
		{
			triarray[0] = tri[2];
			triarray[1] = tri[3];

			if(tri[1] < tri[5])
			{
				triarray[2] = tri[0];
				triarray[3] = tri[1];

				triarray[4] = tri[4];
				triarray[5] = tri[5];

				return;
			}else
			{
				triarray[2] = tri[4];
				triarray[3] = tri[5];

				triarray[4] = tri[0];
				triarray[5] = tri[1];

				return;
			}
		}else
		{
			triarray[0] = tri[4];
			triarray[1] = tri[5];

			triarray[2] = tri[2];
			triarray[3] = tri[3];

			triarray[4] = tri[0];
			triarray[5] = tri[1];

			return;
		}


	}
}

inline void triangleinit3(int triarray[8],int *tri)
{
	triarray[6] = triarray[2]-triarray[0];
	triarray[7] = triarray[3]-triarray[1];

	triarray[2] = triarray[4]-triarray[2];
	triarray[3] = triarray[5]-triarray[3];

	triarray[4] = triarray[4]-triarray[0];
	triarray[5] = triarray[5]-triarray[1];

	if(triarray[7] == 0) triarray[7] = 1;
	if(triarray[3] == 0) triarray[3] = 1;
	if(triarray[5] == 0) triarray[5] = 1;
/*
	int test;

	test = triarray[0] -triarray[2];*/

}


void LMP3D_Draw_Triangles_Texture(int *tri,int ntri,LMP3D_Texture *texture,int *trivt)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();
	unsigned int *tpixels = texture->pixel;

	int y,n,chg;
	int l,l2,lp,lp2,tmp;
	int xfixe,xfixe2,i;

	int triarray[8];

	uint64_t start = rdtsc();

	for(i=0;i<1;i++)
	{
		if(i != 0)continue;
		trianglesort3(triarray,tri);
		triangleinit3(triarray,tri);

		xfixe = (triarray[6]<<4)/triarray[7];
		xfixe2 = (triarray[4]<<4)/triarray[5];

		l = (triarray[0] + (triarray[1]<<8))<<4;
		l2 = l;

		chg = 0;
		if(xfixe < xfixe2)
		{
			tmp = xfixe;
			xfixe = xfixe2;
			xfixe2 = tmp;
			chg++;
		}

		int nvt,nl=0;

		lp2 = 0;

		//printf("%x\n",xfixe);

		for(y=0;y<triarray[7];y++)
		{
			l += (256<<4) + xfixe;
			l2 += (256<<4) + xfixe2;
			n = l>>5;

			lp2 = y<<7;
			nl = 0;

			for(lp=l2>>5;lp<n;lp++)
			{
				nvt = lp2 + ( (nl>>4) << 7);
				pixels[lp]  = tpixels[nvt];
				++lp2;

				nl += -xfixe-4;
			}
		}

		if(chg == 0)
			xfixe = (triarray[2]<<4)/triarray[3];
		else
			xfixe2 = (triarray[2]<<4)/triarray[3];

		lp2 = 0;



		for(y=0;y<triarray[5];y++)
		{
			l += (256<<4) + xfixe;
			l2 += (256<<4) + xfixe2;
			n = l>>5;


			//nvt = (l-l2)>>5;

			nvt = y<<7;
			nl = 0;


			for(lp=l2>>5;lp<n+1;lp++)
			{
				pixels[lp]  = tpixels[++nvt];


			}
//printf("%d %d\n",nl>>4,nl);




		}

	}

	uint64_t end = rdtsc()-start;

	printf("%d\n",end);

}

typedef struct
{
	int y,n,chg;
	int l,l2,lp,tmp;
	int xfixe,xfixe2,i;
	int triarray[8];
}TMP_TRI;

TMP_TRI tmp_tri;

#define maccess(ptr) asm volatile("mov (%0), %0\n" : : "r"(ptr));

void LMP3D_Draw_Triangles_Fill(int *tri,int ntri,int color)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();

	int y,n,chg;
	int l,l2,lp,tmp;
	int xfixe,xfixe2,i;
	int triarray[8];
	int ocolor =  color;


	//int test = 0;


	//printf("%p %p %p %p\n",&y,&n,&i,triarray);



	uint64_t start = rdtsc();


	//maccess(&y);

	for(i=0;i<1;i++)
	{

		trianglesort3(triarray,tri);
		triangleinit3(triarray,tri);

		xfixe = (triarray[6]<<8)/triarray[7];
		xfixe2 = (triarray[4]<<8)/triarray[5];

		l = (triarray[0] + (triarray[1]<<8))<<8;
		l2 = l;

		chg = 0;
		if(xfixe < xfixe2)
		{
			tmp = xfixe;
			xfixe = xfixe2;
			xfixe2 = tmp;
			chg++;
		}

		for(y=0;y<triarray[7];y++)
		{
			l += (256<<8) + xfixe;
			l2 += (256<<8) + xfixe2;
			n = l>>9;
			lp=l2>>9;


			for(;lp<n;lp++)
			{

				pixels[lp]  = ocolor;

				if(lp+3<n)
				{
					pixels[lp+1]  = ocolor;
					pixels[lp+2]  = ocolor;
					pixels[lp+3]  = ocolor;
					lp +=3;
				}else
				{
					if(lp+2<n)
					{
						pixels[lp+1]  = ocolor;
						pixels[lp+2]  = ocolor;
						lp +=2;
					}else
					{
						if(lp+1<n)
						{
							pixels[++lp]  = ocolor;
						}
					}
				}

			}
		}

		if(chg == 0)
			xfixe = (triarray[2]<<8)/triarray[3];
		else
			xfixe2 = (triarray[2]<<8)/triarray[3];

		for(y=0;y<triarray[5];y++)
		{
			l += (256<<8) + xfixe;
			l2 += (256<<8) + xfixe2;
			n = l>>9;
			lp=l2>>9;


			for(;lp<n;lp++)
			{

				pixels[lp]  = ocolor;

				if(lp+3<n)
				{
					pixels[lp+1]  = ocolor;
					pixels[lp+2]  = ocolor;
					pixels[lp+3]  = ocolor;
					lp +=3;
				}else
				{
					if(lp+2<n)
					{
						pixels[lp+1]  = ocolor;
						pixels[lp+2]  = ocolor;
						lp +=2;
					}else
					{
						if(lp+1<n)
						{
							pixels[++lp]  = ocolor;
						}
					}
				}

			}
		}

	}

	uint64_t end = rdtsc()-start;

	printf("%d\n",end);

	i = tri[0] + (tri[1]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;


	i = tri[2] + (tri[3]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;

	i = tri[4] + (tri[5]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;


}


/*

	unsigned int   imatrix[8];
	unsigned short *smatrix = &imatrix;

	unsigned int   ivector[8];
	unsigned short *svector = &ivector;


	unsigned int ivec[4];

	unsigned int vecx,vecy,vecz,vecw;
	imatrix[0] = 0x00000010;
	imatrix[1] = 0x00000000;

	imatrix[2] = 0x00100000;
	imatrix[3] = 0x00000000;

	imatrix[4] = 0x00000000;
	imatrix[5] = 0x00000010;

	imatrix[6] = 0x00000000;
	imatrix[7] = 0x00200000;

	ivec[0] = 0x00000005;
	ivec[1] = 0x00000004;
	ivec[2] = 0x00000009;
	ivec[3] = 0x00000001;

	for(i=0;i<ntri>>1;i++)
	{
		ivector[0] = imatrix[0] * ivec[0]; //(A/E * X)
		ivector[1] = imatrix[1] * ivec[0]; //(I/M * X)

		ivector[2] = imatrix[2] * ivec[1]; //(B/F * Y)
		ivector[3] = imatrix[3] * ivec[1]; //(J/N * Y)

		ivector[4] = imatrix[4] * ivec[2]; //(C/G * Z)
		ivector[5] = imatrix[5] * ivec[2]; //(K/O * Z)

		ivector[6] = imatrix[6] * ivec[3]; //(D/H * W)
		ivector[7] = imatrix[7] * ivec[3]; //(L/P * W)

		vecx = svector[0+0] + svector[4+0] + svector[8+0] + svector[12+0];
		vecy = svector[0+1] + svector[4+1] + svector[8+1] + svector[12+1];
		vecz = svector[0+2] + svector[4+2] + svector[8+2] + svector[12+2];
		vecw = svector[0+3] + svector[4+3] + svector[8+3] + svector[12+3];

		vecw = 0x100/vecw;

		vecx = vecx * vecw;
		vecy = vecy * vecw;


		§§printf("vec %d %d %d %d\n",vecx>>8,vecy>>8,vecz,vecw);
/*


		for(i=0;i<4;i++)
			printf("%d %d %d %d\n",svector[0+i] , svector[4+i] , svector[8+i] , svector[12+i]);


		printf("matrix :\n");
		for(i=0;i<16;i+=4)
			printf("%d %d %d %d\n",smatrix[0+i] , smatrix[1+i] , smatrix[2+i] , smatrix[3+i]);
	}
		*/
