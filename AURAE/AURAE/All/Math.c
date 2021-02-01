#include <stdio.h>
#include <stdlib.h>

#include "AURAE/All/Math.h"

float AURAE_sinf(float x)
{
	int q = (x * 6.3661977236758138e-1f) +0.5f; //floor

	float t = x - q * 1.5707963267923333e+00f;
	t = t - q * 2.5633441515945189e-12f;

	float t2 = t * t;
	if (q & 1)
		t = ((-1.36058866707554670912e-03f * t2 + 4.16566258031673830195e-02f) * t2 - 4.99998875577611430384e-01f) * t2 + 9.99999980750852337007e-01f;
	else
		t = (((-1.94842039748219288187e-04f * t2 + 8.33179571459221545387e-03f) * t2 - 1.66666423796176028737e-01f) * t2 + 9.99999989793669848536e-01f) * t;

	if (q & 2) t = -t;

	return t;
}

double AURAE_sind(double x)
{
	int q = (x * 6.3661977236758138e-1) +0.5; //floor

	double t = x - q * 1.5707963267923333e+00;
	t = t - q * 2.5633441515945189e-12;

	double t2 = t * t;
	if (q & 1)
		t = ((-1.36058866707554670912e-03 * t2 + 4.16566258031673830195e-02) * t2 - 4.99998875577611430384e-01) * t2 + 9.99999980750852337007e-01;
	else
		t = (((-1.94842039748219288187e-04 * t2 + 8.33179571459221545387e-03) * t2 - 1.66666423796176028737e-01) * t2 + 9.99999989793669848536e-01) * t;

	if (q & 2) t = -t;

	return t;
}

float AURAE_cosf(float x)
{
	x += (float)PI/2.0f;
	int q = (x * 6.3661977236758138e-1f) +0.5f; //floor

	float t = x - q * 1.5707963267923333e+00f;
	t = t - q * 2.5633441515945189e-12f;

	float t2 = t * t;
	if (q & 1)
		t = ((-1.36058866707554670912e-03f * t2 + 4.16566258031673830195e-02f) * t2 - 4.99998875577611430384e-01f) * t2 + 9.99999980750852337007e-01f;
	else
		t = (((-1.94842039748219288187e-04f * t2 + 8.33179571459221545387e-03f) * t2 - 1.66666423796176028737e-01f) * t2 + 9.99999989793669848536e-01f) * t;

	if (q & 2) t = -t;

	return t;
}

double AURAE_cosd(double x)
{
	x += PI/2.0;
	int q = (x * 6.3661977236758138e-1) +0.5; //floor

	double t = x - q * 1.5707963267923333e+00;
	t = t - q * 2.5633441515945189e-12;

	double t2 = t * t;
	if (q & 1)
		t = ((-1.36058866707554670912e-03 * t2 + 4.16566258031673830195e-02) * t2 - 4.99998875577611430384e-01) * t2 + 9.99999980750852337007e-01;
	else
		t = (((-1.94842039748219288187e-04 * t2 + 8.33179571459221545387e-03) * t2 - 1.66666423796176028737e-01) * t2 + 9.99999989793669848536e-01) * t;

	if (q & 2) t = -t;

	return t;
}

/*
float AURAE_sqrtf3(float s)
{
	float n;


	//n = sqrt(s);
	return n;
}
__attribute__((optimize("-O1"), noinline))
*/

float  AURAE_sqrtf( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * ( long * ) &y; // evil floating point bit level hacking
	i = 0x5f375a86 - ( (i) >> 1 ); // what the fuck? original : 0x5f3759df , new : 0x5f375a86
	i = i&0x7FFFFFFF;
	y = * ( float * ) &i;

	y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
	y = y * ( threehalfs - ( x2 * y * y ) ); // 3 iteration, this can be removed


	y = 1.0f/y;

	return y;
}

double AURAE_sqrtd( double number )
{
	long i;
	double x2,y;
	float y2;
	const double threehalfs = 1.5;

	x2 = number * 0.5;
	y2 = number;
	i = * ( long * ) &y2; // evil floating point bit level hacking
	i = 0x5f375a86 - ( (i) >> 1 ); // what the fuck? original : 0x5f3759df , new : 0x5f375a86
	i = i&0x7FFFFFFF;
	y2 = * ( float * ) &i;
	y = y2;

	y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
	y = y * ( threehalfs - ( x2 * y * y ) ); // 3 iteration, this can be removed


	y = 1.0/y;

	return y;
}

/*
float AURAE_sqrtf(float s)
{
	float n;
	n = (1.0f + s)/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	return n;
}
*/

float AURAE_sqrtf2(unsigned int fsq2)
{
	float fsq1 = 1;

	while(fsq1 < fsq2)
	{
		fsq1 = fsq1*2;
		fsq2 = fsq2/2;
	}
	fsq1 = (fsq1+fsq2)/2;
	return fsq1;
}

unsigned int AURAE_sqrti2(unsigned int sq2)
{
	unsigned int sq1 = 1;

	while(sq1 < sq2)
	{
		sq1 = sq1<<1;
		sq2 = sq2>>1;
	}

	sq1 = (sq1+sq2)>>1;
	return sq1;
}

unsigned int AURAE_sqrti(const unsigned int s)
{
	unsigned int n,i;

	n = ( (1) + s)>>1;

	for(i = 0;i < 5;i++)
	{
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
	}
	return n;
}

unsigned int AURAE_sqrti4(const unsigned int s)
{
	unsigned int n,i;

	n = ( (1<<4) + s)>>1;

	for(i = 0;i < 5;i++)
	{
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
	}
	return n<<2;
}

unsigned int AURAE_sqrti8(const unsigned int s)
{
	unsigned int n,i;

	n = ( (1<<8) + s)>>1;

	for(i = 0;i < 5;i++)
	{
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
	}
	return n<<4;
}

unsigned int AURAE_sqrti12(const unsigned int s)
{
	unsigned int n,i;

	n = ( (1<<12) + s)>>1;

	for(i = 0;i < 5;i++)
	{
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
		n = (n + (s/n))>>1;
	}
	return n<<6;
}


//code Quake III
float invsqrtf( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * ( long * ) &y; // evil floating point bit level hacking
	i = 0x5f3759df - ( i >> 1 ); // what the fuck? original : 0x5f3759df , new : 0x5f375a86
	y = * ( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
// y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}
