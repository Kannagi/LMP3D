#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"

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
    glLoadIdentity();
    gluPerspective(camera.angle,camera.ratio,camera.min,camera.max);
    glMatrixMode( GL_MODELVIEW );
}

void LMP3D_Camera_Ortho()
{
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

#endif

