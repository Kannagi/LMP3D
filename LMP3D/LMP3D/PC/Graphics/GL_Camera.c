#include <stdio.h>
#include <stdlib.h>
#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"


void xgluPerspective(float fovy, float aspect, float zNear, float zFar)
{
	/*
    float sine, cotangent, deltaZ;
    float radians=(float)(fovy/2.0f)*(PI/180.0f);

    deltaZ=zFar-zNear;
    sine=LMP3D_sinf(radians);
    if ((deltaZ==0.0f) || (sine==0.0f) || (aspect==0.0f))
    {
        return;
    }

    cotangent=(LMP3D_cosf(radians)/sine);*/
    float deltaZ =zFar-zNear;

    float ps = 640.0f/256.0f;

	float matrix[16];

	matrix[(0<<2)+0] = ps;
	matrix[(0<<2)+1] = 0;
	matrix[(0<<2)+2] = 0;
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = 0;
	matrix[(1<<2)+1] = ps*1.50;
	matrix[(1<<2)+2] = 0;
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = 0;
	matrix[(2<<2)+1] = 0;
	matrix[(2<<2)+2] = -(zFar + zNear) / deltaZ;
	matrix[(2<<2)+3] = -1.0f;

	matrix[(3<<2)+0] = 0;
	matrix[(3<<2)+1] = 0;
	matrix[(3<<2)+2] = -(2.0f * zNear * zFar )/ deltaZ;
	matrix[(3<<2)+3] = 0;

	glLoadMatrixf(matrix);
}


void LMP3D_Camera_LookAt(LMP3D_Camera camera)
{
    gluLookAt(camera.position.x,camera.position.y,camera.position.z,
              camera.position.x+camera.vision.x,camera.position.y+camera.vision.y,camera.position.z+camera.vision.z,
              camera.top.x,camera.top.y,camera.top.z);
}

void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    xgluPerspective(camera.angle,camera.ratio,camera.min,camera.max);
    glMatrixMode( GL_MODELVIEW );
}

void LMP3D_Camera_Ortho2D()
{
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

#endif

