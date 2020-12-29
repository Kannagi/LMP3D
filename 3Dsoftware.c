#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/LMP3D.h"
#include <stdint.h>
/*
void *LMP3D_FrameBuffer_WIN95()
{
	return malloc(100000);
}*/
//#define
uint64_t rdtsc2()
{
	register uint64_t a = 0, d = 0;
	asm volatile("mfence");
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
	asm volatile("mfence");
	return a;
}

uint64_t rdtsc(void) {
  register uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}

/*

	public static double[] getBarycentricValue2D(
			final double ax, final double ay,
			final double bx, final double by,
			final double cx, final double cy,
			final double px, final double py){
		final double v0x = bx - ax;
		final double v0y = by - ay;
		final double v1x = cx - ax;
		final double v1y = cy - ay;
		final double v2x = px - ax;
		final double v2y = py - ay;
		final double d00 = v0x*v0x + v0y*v0y;
		final double d01 = v0x*v1x + v0y*v1y;
		final double d11 = v1x*v1x + v1y*v1y;
		final double d20 = v2x*v0x + v2y*v0y;
		final double d21 = v2x*v1x + v2y*v1y;
		final double denom = d00 * d11 - d01 * d01;
		final double v = (d11 * d20 - d01 * d21) / denom;
		final double w = (d00 * d21 - d01 * d20) / denom;
		final double u = 1.0f - v - w;
		return new double[]{u, v, w};
	}



*/

/*
	int i;


	//Matrix *Matrix
	float x,y,z,w;

	for(i = 0;i <4;i++)
	{
		x = src2[0+i];
		y = src2[4+i];
		z = src2[8+i];
		w = src2[12+i];

		dest[0+i]  = (src1[0+0]*x) + (src1[0+1]*y) + (src1[0+2]*z) + (src1[0+3]*w);
		dest[4+i]  = (src1[4+0]*x) + (src1[4+1]*y) + (src1[4+2]*z) + (src1[4+3]*w);
		dest[8+i]  = (src1[8+0]*x) + (src1[8+1]*y) + (src1[8+2]*z) + (src1[8+3]*w);
		dest[12+i] = (src1[12+0]*x) + (src1[12+1]*y) + (src1[12+2]*z) + (src1[12+3]*w);
	}


	MatrixPerspective
	float x,y,z;
	float tx,ty,tz,tw;

	x = src2[0];
	y = src2[1];
	z = src2[2];

	tx = (src1[0+0]*x) + (src1[0+1]*y) + (src1[0+2]*z) + (src1[0+3]);
	ty = (src1[4+0]*x) + (src1[4+1]*y) + (src1[4+2]*z) + (src1[4+3]);
	tz = (src1[8+0]*x) + (src1[8+1]*y) + (src1[8+2]*z) + (src1[8+3]);
	tw = (src1[12+0]*x) + (src1[12+1]*y) + (src1[12+2]*z) + (src1[12+3]);

	tw = 1.0f/tw;

	tx *= tw;
	ty *= tw;
	tz *= tw;
*/

void trianglesort3(int triarray[8],int *tri)
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

void triangleinit3(int triarray[8])
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
}

void LMP3D_Draw_Triangles_Texture(int *tri,int ntri,LMP3D_Texture *texture,int *trivt)
{

	unsigned short *pixels = LMP3D_FrameBuffer_WIN95();
	unsigned short *tpixels = texture->pixel;

	int y,n,m,chg,surface = 1;
	int xRight,xLeft,x,tmp;
	int ixRight,ixLeft,i;
	int pitch = (256<<8);

	int triarray[8];

	uint64_t start = rdtsc();

	int vt1x,vt1y;
	int vt2x,vt2y;
	int vt3x,vt3y;

	for(i=0;i<ntri;i++)
	{
		trianglesort3(triarray,tri);
		triangleinit3(triarray);

		ixRight = (triarray[6]<<8)/triarray[7];
		ixLeft  = (triarray[4]<<8)/triarray[5];

		xLeft = xRight = (triarray[0] + (triarray[1]<<8))<<8;

		chg = 0;
		vt1x = 0;
		vt1y = (0<<16);
		vt2x = 0;
		vt2y = 128<<8;
		vt3x = 255<<8;
		vt3y = 128<<8;
		int itx,ity;

		//printf("%d\n",triarray[0]);

		if(ixRight < ixLeft)
		{
			tmp = ixRight;
			ixRight = ixLeft;
			ixLeft = tmp;
			chg++;

			itx = triarray[7];
			ity = triarray[5];
		}else
		{
			itx = triarray[5];
			ity = triarray[7];
		}
		if(itx == 0) itx = 1;
		vt2y = vt2y/itx;
		tmp  = (pitch + ixRight)*ity;
		tmp -= (pitch + ixLeft )*ity;

		int xmax = tmp>>8;
		vt3x = vt3x/xmax;

		tmp = vt2y*ity;
		if(xmax == 0) xmax = 1;
		vt3y = (vt3y-tmp)/xmax;
		vt2y = vt2y<<8;

		int limx1 = triarray[0]<<8;
		int limx2 = triarray[0]<<8;

		for(y=0;y<triarray[7];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			limx1 += ixLeft;
			limx2 += ixRight;
			n = xRight>>8;
			m = xLeft>>8;

			vt1y += vt2y;
			vt1x += vt2x;

			itx = (vt1y&0xFF0000) + vt1x;
			ity = 0;

			if( (limx1) < 0 )
			{
				int m1 = (m&0xFFFF00)+0x100;
				tmp = m1-m;
				itx+=vt3x*tmp;
				ity+=vt3y*tmp;

				m = m1;
			}

			for(x = m;x < n;x++)
			{
				int ivt = (itx>>8) + (ity&0xFFFFFF00);
				pixels[x] = tpixels[ivt];
				itx+=vt3x;
				ity+=vt3y;
			}
			surface += (n-m);


		}

		if(chg == 0)
		{
			ixRight = (triarray[2]<<8)/triarray[3];
		}else
		{
			ixLeft = (triarray[2]<<8)/triarray[3];
			tmp = triarray[5]-triarray[7];
			vt2x  = (256<<8)/tmp;
			vt2y = 0;
		}

		for(;y<triarray[5];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			n = xRight>>8;
			m = xLeft>>8;

			vt1y += vt2y;
			vt1x += vt2x;
			itx = (vt1y&0xFFFF0000) + vt1x;
			ity = 0;

			for(x = m;x < n;x++)
			{
				int ivt = (itx>>8) + (ity&0xFFFFFF00);
				pixels[x]  = tpixels[ivt];
				itx+=vt3x;
				ity+=vt3y;
			}
			surface += (n-m);
		}

	}




	uint64_t end = rdtsc()-start;

	//printf("%d / %d = %d\n",end,surface,end/surface);

}


void LMP3D_Draw_Triangles_Fill(int *tri,int ntri,int color)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();

	int y,n,m,chg,surface = 1;
	int x,xRight,xLeft,tmp;
	int ixRight,ixLeft,i;
	int triarray[8];
	int ocolor =  color;
	int pitch = (256<<8);

	uint64_t start = rdtsc();



	for(i=0;i<ntri;i++)
	{
		trianglesort3(triarray,tri);
		triangleinit3(triarray);

		ixRight = (triarray[6]<<8)/triarray[7];
		ixLeft  = (triarray[4]<<8)/triarray[5];

		xLeft = xRight = (triarray[0] + (triarray[1]<<8))<<8;

		chg = 0;
		if(ixRight < ixLeft)
		{
			tmp = ixRight;
			ixRight = ixLeft;
			ixLeft = tmp;
			chg++;
		}

		for(y=0;y<triarray[7];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			n = xRight>>9;
			m = xLeft>>9;

			for(x = m;x < n;x++)
			{
				pixels[x]  = ocolor;
			}
			surface += (n-m);
		}

		if(chg == 0)
			ixRight = (triarray[2]<<8)/triarray[3];
		else
			ixLeft = (triarray[2]<<8)/triarray[3];

		for(;y<triarray[5];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			n = xRight>>9;
			m = xLeft>>9;

			for(x = m;x < n;x++)
			{
				pixels[x]  = ocolor;
			}
			surface += (n-m);
		}

	}

	uint64_t end = rdtsc()-start;

	surface = surface<<1;
	//printf("%d / %d = %d\n",end,surface,end/surface);

	i = tri[0] + (tri[1]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;

	i = tri[2] + (tri[3]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;

	i = tri[4] + (tri[5]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;
}




void LMP3D_Draw_Line(int *line,int color)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();

	int y,n;
	int l,x,tmp;
	int ixRight,i,tx,ty;
	int ocolor =  color;

	int hx = 0;
	int hy = 1;

	tx = line[2]-line[0];
	ty = line[3]-line[1];

	if(line[1] > line[3])
	{
		hx += 2;
		hy += 2;
	}

	if(ty == 0) ty = 1;
	if(tx == 0) tx = 1;


	ixRight = (tx<<8)/(ty);

	l = (line[hx] + (line[hy]<<8))<<8;

	int ny = line[1] - line[3];
	if(ny < 0) ny = -ny;


	for(y=0;y<ny;y++)
	{
		l += (256<<8) + ixRight;
		x=l>>9;


		pixels[x]  = ocolor;

	}




	i = line[0] + (line[1]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;


	i = line[2] + (line[3]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;



}

//---------------------------------------------------------------------------------------------------
void trianglesort3f(float triarray[8],int *tri)
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

void triangleinit3f(float triarray[8])
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
}

void LMP3D_Draw_Triangles_Fill_Float(int *tri,int ntri,int color)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();

	int y,n,m,chg,surface = 1;
	int x,tmp,i;
	float triarray[8];
	int ocolor =  color;


	int h1,h2;

	uint64_t start = rdtsc();

	float fxRight,fxLeft,xRight,xLeft,ftmp;
	float pitch = (256>>1);



	//for(i=0;i<ntri;i++)
	{
		trianglesort3f(triarray,tri);
		triangleinit3f(triarray);

		fxRight = (triarray[6]/2)/triarray[7];
		fxLeft  = (triarray[4]/2)/triarray[5];




		ftmp = ( (triarray[0]/2) + (triarray[1]*pitch));


		xLeft = xRight = ftmp;

		chg = 0;
		if(fxRight < fxLeft)
		{
			ftmp = fxRight;
			fxRight = fxLeft;
			fxLeft = ftmp;
			chg++;
		}
		h1 = triarray[7];
		h2 = triarray[5];

		//printf("%d\n",h1);

		float limx1 = triarray[0]/2;
		float limx2 = limx1;
		int h = (triarray[1]*pitch);


		for(y=0;y<h1;y++)
		{
			xRight += pitch + fxRight;
			xLeft  += pitch + fxLeft;
			h += pitch;

			limx1 += fxLeft;
			limx2 += fxRight;

			n = xRight;
			m = xLeft;


			if((int)limx1 <= 0)
				m = h;

			for(x = m;x < n;x++)
			{
				pixels[x]  = ocolor;
			}


			surface += (n-m);
		}

		if(chg == 0)
			fxRight = (triarray[2]/2)/triarray[3];
		else
			fxLeft = (triarray[2]/2)/triarray[3];

		for(;y<h2;y++)
		{
			xRight += pitch + fxRight;
			xLeft  += pitch + fxLeft;
			n = xRight;
			m = xLeft;

			h += pitch;

			limx1 += fxLeft;
			limx2 += fxRight;

			if((int)limx1 <= 0)
				m = h;

			for(x = m;x < n;x++)
			{
				pixels[x]  = ocolor;
			}
			surface += (n-m);
		}

	}

	uint64_t end = rdtsc()-start;

	surface = surface<<1;
	//printf("%d / %d = %d\n",end,surface,end/surface);

	i = tri[0] + (tri[1]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;

	i = tri[2] + (tri[3]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;

	i = tri[4] + (tri[5]<<8);
	pixels[i>>1]  = 0xFFFFFFFF;
}

/*
		for(y=0;y<triarray[7];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			n = xRight>>9;

			itx = (vt1y&0xFF0000) + vt1x;
			ity = 0;

			for(x = xLeft>>9;x < n;x++)
			{
				int ivt = (itx>>9) + (ity&0xFFFFFF00);
				pixels[x] = tpixels[ivt];
				itx+=vt3x;
				ity+=vt3y;
			}

			vt1y += vt2y;
			vt1x += vt2x;
		}


		if(chg == 0)
			ixRight = (triarray[2]<<8)/triarray[3];
		else
			ixLeft = (triarray[2]<<8)/triarray[3];

		if(chg != 0)
		{
			tmp = triarray[5]-triarray[7];
			vt2x  = (0x10000)/tmp;
			vt2y = 0;
		}

		for(;y<triarray[5];y++)
		{
			xRight += pitch + ixRight;
			xLeft  += pitch + ixLeft;
			n = xRight>>9;

			int itx = (vt1y&0xFF0000) + vt1x;
			int ity = 0;

			for(x = xLeft>>9;x < n;x++)
			{
				int ivt = (itx>>9) + (ity&0xFFFFFF00);
				pixels[x]  = tpixels[ivt];
				itx+=vt3x;
				ity+=vt3y;
			}

			//printf("%d %d\n",(itx&0xFFFF)>>8,ity>>8);

			vt1y += vt2y;
			vt1x += vt2x;
		}




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
