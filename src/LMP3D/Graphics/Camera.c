#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "LMP3D/LMP3D.h"


LMP3D_Camera LMP3D_Camera_Init()
{
    LMP3D_Camera camera;
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 0;
    camera.vision.x = 0;
    camera.vision.y = 0;
    camera.vision.z = -1;
    camera.top.x = 0;
    camera.top.y = 1;
    camera.top.z = 0;
    camera.angle = 72;
    camera.min = 1.0f;
    camera.max = 10000;
    camera.ratio = 640.0/480.0;

    return camera;
}




void LMP3D_Camera_vue_obj(LMP3D_Camera *camera,Vector3 *p,Vector3 *a,float v,float vit,int *touche)
{
    if(touche[0] == 1)
        a->x += v;

    if(touche[0] == 2)
        a->x -= v;

    if(touche[1] == 1)
        a->y += v;

    if(touche[1] == 2)
        a->y -= v;

    if(a->x > (PI*2)) a->x = a->x - (PI*2);
    if(a->x < 0)      a->x = (PI*2) + a->x;
    if(a->y > (PI*2)) a->y = a->y - (PI*2);
    if(a->y < 0)      a->y = (PI*2) + a->y;

    if(a->y >= PI) camera->top.y = -1;
    else
    camera->top.y = 1;

    if(touche[2] == 1) a->z += v;
    if(touche[2] == 2) a->z -= v;

    camera->vision.x = -cos(a->x);
    camera->vision.z =  sin(a->x);
    if(touche[3] == 1)
    {
        p->x += camera->vision.x*vit;
        p->z += camera->vision.z*vit;
    }

    if(touche[3] == 2)
    {
        p->x -= camera->vision.x*vit;
        p->z -= camera->vision.z*vit;
    }

    if(touche[4] == 1)
    {
        camera->vision.x = cos(a->x-PI/2);
        camera->vision.z = -sin(a->x-PI/2);
        p->x += camera->vision.x*vit;
        p->z += camera->vision.z*vit;
    }

    if(touche[4] == 2)
    {
        camera->vision.x = cos(a->x-PI/2);
        camera->vision.z = -sin(a->x-PI/2);
        p->x -= camera->vision.x*vit;
        p->z -= camera->vision.z*vit;
    }

    camera->position.x = cos(a->x)*sin(a->y);
    camera->position.y = cos(a->y);
    camera->position.z = -sin(a->x)*sin(a->y);

    camera->position.x = (camera->position.x*a->z) + p->x;
    camera->position.y = (camera->position.y*a->z) + p->y;
    camera->position.z = (camera->position.z*a->z) + p->z;

    camera->vision.x  = p->x - camera->position.x;
    camera->vision.y  = p->y - camera->position.y;
    camera->vision.z  = p->z - camera->position.z;

    if(touche[4] == 2)
        a->x -= v;

    if(touche[4] == 1)
        a->x += v;

}

void LMP3D_Camera_vue_sub(LMP3D_Camera *camera,Vector3 *a,float v,int *touche)
{
    if(touche[0] == 1)
        a->x += v;

    if(touche[0] == 2)
        a->x -= v;

    if(touche[2] == 1)
        a->y += v;

    if(touche[2] == 2)
        a->y -= v;

    camera->vision.x = cos(a->x)*sin(a->y);
    camera->vision.y = cos(a->y);
    camera->vision.z = -sin(a->x)*sin(a->y);

    if(touche[1] == 1)
    {
        camera->position.z =  camera->position.z + (a->z *camera->vision.z);
        camera->position.x =  camera->position.x + (a->z *camera->vision.x);
        camera->position.y =  camera->position.y + (a->z *camera->vision.y);
    }

    if(touche[1] == 2)
    {
        camera->position.z =  camera->position.z - (a->z *camera->vision.z);
        camera->position.x =  camera->position.x - (a->z *camera->vision.x);
        camera->position.y =  camera->position.y - (a->z *camera->vision.y);
    }
}
