#include <stdio.h>
#include <stdlib.h>

#include "LMP3D/Math.h"

float LMP3D_sinf(float x)
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

double LMP3D_sind(double x)
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

float LMP3D_cosf(float x)
{
	x += PI/2;
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

double LMP3D_cosd(double x)
{
	x += PI/2;
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

float LMP3D_sqrtf(float s)
{
	float n;
	n = (1.0f + s)/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	n = (n + (s/n))/2.0f;
	return n;
}


//code Quake III
float isqrtf( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * ( long * ) &y; // evil floating point bit level hacking
	i = 0x5f375a86 - ( i >> 1 ); // what the fuck? original : 0x5f3759df , new : 0x5f375a86
	y = * ( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
// y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}
