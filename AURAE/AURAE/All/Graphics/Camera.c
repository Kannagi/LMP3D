#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "AURAE/AURAE.h"


AURAE_Camera AURAE_Camera_Init()
{
	AURAE_Camera camera;
	camera.position = AURAE_Type_Vector3(0,0,0);
	camera.vision   = AURAE_Type_Vector3(0,0,-1);
	camera.top	  = AURAE_Type_Vector3(0,1,0);
	camera.angle	= AURAE_Type_Vector3(PI/2,PI/2,-400);
	camera.viewangle = 72;
	camera.min = 1.0f;
	camera.max = 10000;
	camera.ratio = 640.0f/480.0f;
	camera.speed = 0.01f;
	camera.speedz = 3.01f;

	return camera;
}

void AURAE_Math_NormalizeFPU(float *normal)
{
	register float mag = AURAE_sqrtf( (normal[0]*normal[0]) + (normal[1]*normal[1]) + (normal[2]*normal[2]) );

	if(mag != 1.0f)
	{
		mag = 1.0f/mag;
		normal[0] *= mag;
		normal[1] *= mag;
		normal[2] *= mag;

	}
}

float AURAE_Math_Dot3FPU(float *x,float *y)
{
	return (x[0]*y[0]) + (x[1]*y[1]) + (x[2]*y[2]);
}

float AURAE_Math_Dot4FPU(float *x,float *y)
{
	return  (x[0]*y[0]) + (x[1]*y[1]) + (x[2]*y[2]) + (x[3]*y[3]);
}

void AURAE_Math_CrossFPU(float *r,float *x,float *y)
{
	register float x0,x1,x2;
	x0 = x[0];
	x1 = x[1];
	x2 = x[2];
	r[0] =  (x1*y[2]) - (x2*y[1]);
	r[1] =  (x2*y[0]) - (x0*y[2]);
	r[2] =  (x0*y[1]) - (x1*y[0]);
}


void AURAE_Camera_LookAt(float *MatrixLookAt,Vector3 eye,Vector3 center,Vector3 up)
{
	float x[3], y[3], z[3];

	float teye[3];
	teye[0] = eye.x;
	teye[1] = eye.y;
	teye[2] = eye.z;

	// Z = Eye - Center;
	z[0] = eye.x - center.x;
	z[1] = eye.y - center.y;
	z[2] = eye.z - center.z;

	//Z normalize
	AURAE_Math_NormalizeFPU(z);

	/*  Y = Up;*/
	y[0] = up.x;
	y[1] = up.y;
	y[2] = up.z;

	/* X vector = Y cross Z */
	AURAE_Math_CrossFPU(x,y,z);

	/* Recompute Y = Z cross X */
	AURAE_Math_CrossFPU(y,z,x);

	AURAE_Math_NormalizeFPU(x);
	AURAE_Math_NormalizeFPU(y);

	MatrixLookAt[(0<<2)+0] = x[0];
	MatrixLookAt[(0<<2)+1] = y[0];
	MatrixLookAt[(0<<2)+2] = z[0];
	MatrixLookAt[(0<<2)+3] = 0;

	MatrixLookAt[(1<<2)+0] = x[1];
	MatrixLookAt[(1<<2)+1] = y[1];
	MatrixLookAt[(1<<2)+2] = z[1];
	MatrixLookAt[(1<<2)+3] = 0;

	MatrixLookAt[(2<<2)+0] = x[2];
	MatrixLookAt[(2<<2)+1] = y[2];
	MatrixLookAt[(2<<2)+2] = z[2];
	MatrixLookAt[(2<<2)+3] = 0;

	MatrixLookAt[(3<<2)+0] = -AURAE_Math_Dot3FPU(x,teye);
	MatrixLookAt[(3<<2)+1] = -AURAE_Math_Dot3FPU(y,teye);
	MatrixLookAt[(3<<2)+2] = -AURAE_Math_Dot3FPU(z,teye);
	MatrixLookAt[(3<<2)+3] = 1.0f;
}

void AURAE_Camera_ViewObj(AURAE_Camera *camera,Vector3 *position)
{
	if(camera->key[0] == 2)
		camera->angle.x += camera->speed;

	if(camera->key[1] == 2)
		camera->angle.x -= camera->speed;

	if(camera->key[2] == 2)
		camera->angle.y += camera->speed;

	if(camera->key[3] == 2)
		camera->angle.y -= camera->speed;

	if(camera->angle.x > (PI*2)) camera->angle.x = camera->angle.x - (PI*2);
	if(camera->angle.x < 0)	  camera->angle.x = (PI*2) + camera->angle.x;
	if(camera->angle.y > (PI*2)) camera->angle.y = camera->angle.y - (PI*2);
	if(camera->angle.y < 0)	  camera->angle.y = (PI*2) + camera->angle.y;

	if(camera->angle.y >= PI) camera->top.y = -1;
	else camera->top.y = 1;

	if(camera->key[4] == 2) camera->angle.z += camera->speedz;
	if(camera->key[5] == 2) camera->angle.z -= camera->speedz;

	float fsin = AURAE_sinf(camera->angle.y);

	camera->vision.x = AURAE_cosf(camera->angle.x)*fsin;
	camera->vision.y = AURAE_cosf(camera->angle.y);
	camera->vision.z = -AURAE_sinf(camera->angle.x)*fsin;


	if(camera->angle.z > 0)
	{
		camera->angle.z = -camera->angle.z;
	}

	float distance = camera->angle.z;

	camera->position.x =  (distance * camera->vision.x) + position->x;
	camera->position.y =  (distance * camera->vision.y) + position->y;
	camera->position.z =  (distance * camera->vision.z) + position->z;

	camera->vision.x += camera->position.x;
	camera->vision.y += camera->position.y;
	camera->vision.z += camera->position.z;

	return;
}

void  AURAE_Camera_ViewSub(AURAE_Camera *camera)
{
	if(camera->key[0] == 2)
		camera->angle.x += camera->speed;

	if(camera->key[1] == 2)
		camera->angle.x -= camera->speed;

	if(camera->key[2] == 2)
		camera->angle.y += camera->speed;

	if(camera->key[3] == 2)
		camera->angle.y -= camera->speed;

	float fsin = AURAE_sinf(camera->angle.y);

	camera->vision.x = AURAE_cosf(camera->angle.x)*fsin;
	camera->vision.y = AURAE_cosf(camera->angle.y);
	camera->vision.z = -AURAE_sinf(camera->angle.x)*fsin;

	float speedz = camera->speedz;

	if(camera->key[4] == 2)
	{
		camera->position.z +=  (speedz * camera->vision.z);
		camera->position.x +=  (speedz * camera->vision.x);
		camera->position.y +=  (speedz * camera->vision.y);
		camera->angle.z -= speedz*(camera->vision.x+camera->vision.y+camera->vision.z);
	}

	if(camera->key[5] == 2)
	{
		camera->position.z -=  (speedz * camera->vision.z);
		camera->position.x -=  (speedz * camera->vision.x);
		camera->position.y -=  (speedz * camera->vision.y);
		camera->angle.z += speedz*(camera->vision.x+camera->vision.y+camera->vision.z);
	}

	camera->vision.x += camera->position.x;
	camera->vision.y += camera->position.y;
	camera->vision.z += camera->position.z;
}

/*
	template<typename T>
	Vector3<T> Quaternion<T>::operator*(const Vector3<T>& vec) const
	{
		Vector3<T> quatVec(x, y, z);
		Vector3<T> uv = quatVec.CrossProduct(vec);
		Vector3<T> uuv = quatVec.CrossProduct(uv);
		uv *= F(2.0) * w;
		uuv *= F(2.0);

		return vec + uv + uuv;
	}
*/
