#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <float.h>

#include "LMP3D/LMP3D.h"

inline Vector3 MNS_Vector3(float x,float y,float z)
{
	Vector3 vec3;

	vec3.x = x;
	vec3.y = y;
	vec3.z = z;
	return vec3;
}

//---------------------------------------



void LMP3D_Animation3D_TranslationRotation(LMP3D_Model *obj,Matrix3x2 *precalcul)
{
    int id,l;
    float tx,ty,tz,c,s;
    Vector3 p,*v,*va;

    v = (void*)obj->v;
    va = (void*)obj->va;

    for(l = 0;l < obj->nv;l++)
    {
    	id = obj->id[l];

        p.x = v[l].x - obj->defaut.skeleton[id].p.x;
        p.y = v[l].y - obj->defaut.skeleton[id].p.y;
        p.z = v[l].z - obj->defaut.skeleton[id].p.z;


        ty = p.y;
        tz = p.z;
        c = precalcul[id].p.x;
        s = precalcul[id].r.x;
        p.z = ( c * tz ) - ( s *ty);
        p.y = ( s * tz ) + ( c *ty);

        tx = p.x;
        ty = p.y;
        c = precalcul[id].p.y;
        s = precalcul[id].r.y;
        p.y = ( c * ty ) - ( s *tx);
        p.x = ( s * ty ) + ( c *tx);

        tx = p.x;
        tz = p.z;
        c = precalcul[id].p.z;
        s = precalcul[id].r.z;
        p.x = ( c * tx ) - ( s *tz);
        p.z = ( s * tx ) + ( c *tz);

        va[l].x = p.x + obj->nodes[id].x;
        va[l].y = p.y + obj->nodes[id].y;
        va[l].z = p.z + obj->nodes[id].z;


    }
}

static Vector3 calcul_skl(LMP3D_Model *obj,int i,Vector3 add,Matrix3x2 *precalcul)
{
    int id;
    Vector3 p;
    float tx,ty,tz,c,s;

    p.x = obj->defaut.skeleton[i].p.x + add.x;
    p.y = obj->defaut.skeleton[i].p.y + add.y;
    p.z = obj->defaut.skeleton[i].p.z + add.z;

    id = i;

    while(1)
    {
        p.x -= obj->defaut.skeleton[id].p.x;
        p.y -= obj->defaut.skeleton[id].p.y;
        p.z -= obj->defaut.skeleton[id].p.z;

        ty = p.y;
        tz = p.z;
        c = precalcul[id].p.x;
        s = precalcul[id].r.x;
        p.z = ( c * tz ) - ( s *ty);
        p.y = ( s * tz ) + ( c *ty);

        tx = p.x;
        ty = p.y;
        c = precalcul[id].p.y;
        s = precalcul[id].r.y;
        p.y = ( c * ty ) - ( s *tx);
        p.x = ( s * ty ) + ( c *tx);

        tx = p.x;
        tz = p.z;
        c = precalcul[id].p.z;
        s = precalcul[id].r.z;
        p.x = ( c * tx ) - ( s *tz);
        p.z = ( s * tx ) + ( c *tz);

        p.x += obj->defaut.skeleton[id].p.x;
        p.y += obj->defaut.skeleton[id].p.y;
        p.z += obj->defaut.skeleton[id].p.z;

        id = obj->bones[id];
        if(id == -1) break;
    }

    return p;
}
void LMP3D_Animation3D_PreCalcul(LMP3D_Model *obj,Matrix3x2 *precalcul)
{
    int ida,idas,id,f = obj->play;
    float r;
    Vector3 rot[100];

    for(id = 0;id < obj->nbones;id++)
    {

        ida = id +obj->nbones*obj->time;

        if(obj->time < obj->anim[f]->ntime -1)
            idas = ida + obj->nbones;
        else
			idas = id;

        r = obj->anim[f]->skeleton[idas].r.x - obj->anim[f]->skeleton[ida].r.x;
        r = r/(obj->anim[f]->ncadence+1);
        r = r* obj->cadence;
        r += obj->anim[f]->skeleton[ida].r.x;
        rot[id].x = -r;

        precalcul[id].p.x = cos(-r);
        precalcul[id].r.x = sin(-r);

        r = obj->anim[f]->skeleton[idas].r.y - obj->anim[f]->skeleton[ida].r.y;
        r = r/(obj->anim[f]->ncadence+1);
        r = r* obj->cadence;
        r += obj->anim[f]->skeleton[ida].r.y;
        rot[id].y = -r;

        precalcul[id].p.y = cos(-r);
        precalcul[id].r.y = sin(-r);

        r = obj->anim[f]->skeleton[idas].r.z - obj->anim[f]->skeleton[ida].r.z;
        r = r/(obj->anim[f]->ncadence+1);
        r = r* obj->cadence;
        r += obj->anim[f]->skeleton[ida].r.z;
        rot[id].z = r;

        precalcul[id].p.z = cos(r);
        precalcul[id].r.z = sin(r);
    }

    int i;
    Vector3 p,mr;
    float angle;

    for(i = 0;i < obj->nbones;i++)
    {
        p = calcul_skl(obj,i,MNS_Vector3(0,0,0),precalcul);

        mr = calcul_skl(obj,i,MNS_Vector3(0,1,0),precalcul);
        angle = LMP3D_Math_LawOfCosines(p.z,mr.z,p.z , p.y+1,mr.y,p.y);
        if(mr.z > p.z ) angle = -angle;
        rot[i].x = angle;

        mr = calcul_skl(obj,i,MNS_Vector3(1,0,0),precalcul);
        angle = LMP3D_Math_LawOfCosines(p.y,mr.y,p.y , p.x+1,mr.x,p.x);
        if(mr.y > p.y ) angle = -angle;
        rot[i].y = angle;

        mr = calcul_skl(obj,i,MNS_Vector3(1,0,0),precalcul);
        angle = LMP3D_Math_LawOfCosines(p.z,mr.z,p.z , p.x+1,mr.x,p.x);
        if(mr.z < p.z ) angle = -angle;
        rot[i].z = angle;

        obj->nodes[i].x = p.x;
        obj->nodes[i].y = p.y;
        obj->nodes[i].z = p.z;

        //printf("%.4f %.4f %.4f\n",p.x,p.y,p.z);
    }



    for(i = 0;i < obj->nbones;i++)
    {
        r = rot[i].x;
        precalcul[i].p.x = cos(r);
        precalcul[i].r.x = sin(r);

        r = rot[i].y;
        precalcul[i].p.y = cos(r);
        precalcul[i].r.y = sin(r);

        r = rot[i].z;
        precalcul[i].p.z = cos(r);
        precalcul[i].r.z = sin(r);
    }

}




void LMP3D_Animation3D(LMP3D_Model *obj)
{
	if(obj->anim == NULL) return;
	Matrix3x2 precalcul[200];


	LMP3D_Animation3D_PreCalcul(obj,precalcul);
	LMP3D_Animation3D_TranslationRotation(obj,precalcul);


}
