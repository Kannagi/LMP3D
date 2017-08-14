#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"


LMP3D_Model *LMP3D_Load_Model_bcm(char *nom_du_fichier)
{
    FILE* fichier = NULL;
    int i,l;

    LMP3D_Model *obj;
    obj = malloc(sizeof(LMP3D_Model));

    LMP3D_Model_Init(obj);
    obj->format = 3;

    fichier = fopen(nom_du_fichier, "rb");
    int octet = 0;

    if(fichier != NULL)
    {
        //signature
        fgetc(fichier);
        fgetc(fichier);
        fgetc(fichier);
        fgetc(fichier);

        //version / info
        fgetc(fichier);
        obj->type = fgetc(fichier);
        //obj->type = 0x01;

        //offset
        fread(&octet,1,sizeof(short),fichier);


        //nombre de vertex/texture/normal
        fread(&obj->nv,1,sizeof(int),fichier);
        obj->v  = malloc(obj->nv * 3 * sizeof(float));
        obj->vt = malloc(obj->nv * 2 * sizeof(float));
        obj->vn = malloc(obj->nv * 3 * sizeof(float));

        obj->va = obj->v;

        if(obj->type & 0x02)
        {
            obj->id = malloc(obj->nv * sizeof(short));
            obj->va = malloc(obj->nv * 3 * sizeof(float));
        }


        for(i = 0;i < obj->nv;i++)
        {
        	fread(&obj->v[i*3 + 0],sizeof(float),3,fichier);
        	fread(&obj->vn[i*3 + 0],sizeof(float),3,fichier);
        	fread(&obj->vt[i*2 + 0],sizeof(float),2,fichier);

            if(obj->type & 0x02)
            {
                obj->va[i*3 + 0] = obj->v[i*3 + 0];
                obj->va[i*3 + 1] = obj->v[i*3 + 1];
                obj->va[i*3 + 2] = obj->v[i*3 + 2];


                fread(&obj->id[i],1,sizeof(short),fichier);

            }

        }

        //nombre de face /index
        fread(&obj->nf,1,sizeof(int),fichier);

        obj->f = malloc(obj->nf * 3 * sizeof(short));
		fread(obj->f,sizeof(short),obj->nf*3,fichier);


        //texture
        fread(&obj->ntexture,1,sizeof(short),fichier);

        obj->name = malloc(obj->ntexture * sizeof(char*));
        obj->texture = malloc(obj->ntexture * sizeof(int));
        obj->texture_begin = malloc(obj->ntexture * sizeof(int));

        for(i = 0;i < obj->ntexture;i++)
        {

			obj->name[i] = malloc(21 * sizeof(char));
			obj->name[i][20] = 0;

			fread(obj->name[i],sizeof(char),20,fichier);

            fread(&obj->texture_begin[i],1,sizeof(int),fichier);
        }

        if(obj->type & 0x02)
        {
            //skeleton nodes
            fread(&obj->nbones,1,sizeof(short),fichier);

            obj->bones           = malloc(obj->nbones * sizeof(short));
            obj->nodes           = malloc(obj->nbones * sizeof(Vector3));

            for(i = 0;i < obj->nbones;i++)
            {
                for(l = 0;l < 20;l++)
                    fgetc(fichier);

                fread(&obj->bones[i],1,sizeof(short),fichier);
            }

            //Animation
            fread(&obj->defaut.ntime,1,sizeof(short),fichier);

            obj->defaut.n = obj->defaut.ntime * obj->nbones;
            obj->defaut.skeleton = malloc(obj->defaut.n * sizeof(Matrix3x2));

            for(i = 0;i < obj->defaut.n;i++)
            {
				fread(&obj->defaut.skeleton[i].p.x,sizeof(float),6,fichier);

            }
        }

        fclose(fichier);
    }


#ifdef PLAYSTATION2



		int tv,l2;

		free(obj->vn);
		free(obj->v);

		obj->v  = malloc(obj->nf*3 * 8 * sizeof(u16));

		u16 * uv = (u16 *)obj->v;
		for(i = 0;i < obj->nf*3;i++)
		{
			tv = i*8;
			l = obj->f[i]*3;
			l2 = obj->f[i]*2;

			uv[tv + 0] = ftoi4(obj->va[l+0]);
			uv[tv + 1] = ftoi4(obj->va[l+1]);
			uv[tv + 2] = ftoi4(obj->va[l+2]);
			uv[tv + 3] = ftoi4(1.0f);


			uv[tv + 4] = ftoi15(obj->vt[l2+0]);
			uv[tv + 5] = ftoi15(obj->vt[l2+1]);
			uv[tv + 6] = ftoi15(1.0f);
			uv[tv + 7] = ftoi15(1.0f);



		}
		free(obj->va);
		free(obj->vt);
		free(obj->f);
#endif


    char dossier[100];

    LMP3D_Folder_Out(nom_du_fichier,dossier);
#ifdef PC
    LMP3D_Load_Texture_Array(obj,dossier);
#endif
    return obj;
}
/*
void MNS_write_bcm(LMP3D_Model *model,char *dst)
{
    FILE* fichier = NULL;
    int i,l;
    int ok,n = 0;


    fichier = fopen(dst, "wb");

    if(fichier != NULL)
    {
        //signature
        fputs("BCM",fichier);
        fputc(0,fichier);

        //info version et info
        fputc(0,fichier);
        fputc(model->type | 0x01,fichier);

        //offset
        write_type_short(fichier,8);

        int tp = 0;

        if(model->type & 0x01)
            tp = 1;
        else
            tp = 0;

        //vertex
        Vector3 *v = (void*)model->v;
        Vector3 *vn = (void*)model->vn;
        VEC2 *vt = (void*)model->vt;
        if(tp == 0)
        {
            unsigned short *f = malloc(model->nf * 3 * sizeof(short));

            Vector3 *va = malloc(model->nv * sizeof(Vector3));
            Vector3 *van = malloc(model->nv * sizeof(Vector3));
            VEC2 *vat = malloc(model->nv * sizeof(VEC2));
            short *id = malloc(model->nv * sizeof(short));

            for(i = 0;i < model->nv;i++)
            {
                ok = 0;
                for(l = 0;l < i;l++)
                {
                    if(
                       v[i].x == v[l].x && v[i].y == v[l].y && v[i].z == v[l].z &&
                       vt[i].x == vt[l].x && vt[i].y == vt[l].y &&
                       vn[i].x == vn[l].x && vn[i].y == vn[l].y && vn[i].z == vn[l].z
                       )
                    {
                        ok = 1;
                        l = i;
                    }
                }

                if(ok == 0)
                {
                    va[n].x = v[i].x;
                    va[n].y = v[i].y;
                    va[n].z = v[i].z;

                    van[n].x = vn[i].x;
                    van[n].y = vn[i].y;
                    van[n].z = vn[i].z;

                    vat[n].x = vt[i].x;
                    vat[n].y = vt[i].y;

                    if(model->type & 0x02)
                    id[n] = model->id[i];

                    f[i] = n;
                    n++;

                }else
                {
                    for(l = 0;l < n;l++)
                    {
                        if(
                           v[i].x == va[l].x && v[i].y == va[l].y && v[i].z == va[l].z &&
                           vt[i].x == vat[l].x && vt[i].y == vat[l].y &&
                           vn[i].x == van[l].x && vn[i].y == van[l].y && vn[i].z == van[l].z
                           )
                        {
                            ok = l;
                            l = i;
                        }
                    }
                    f[i] = ok;
                }

            }

            //nombre de vertex
            write_type_long(fichier,n);

            for(i = 0;i < n;i++)
            {
                write_type_float(fichier,va[i].x);
                write_type_float(fichier,va[i].y);
                write_type_float(fichier,va[i].z);

                write_type_float(fichier,van[i].x);
                write_type_float(fichier,van[i].y);
                write_type_float(fichier,van[i].z);

                write_type_float(fichier,vat[i].x);
                write_type_float(fichier,vat[i].y);

                if(model->type & 0x02)
                    write_type_short(fichier,id[i]);
            }

            //nombre de face
            write_type_long(fichier,model->nf);
            for(i = 0;i < model->nf*3;i++)
            {
                write_type_short(fichier,f[i]);
            }

            free(van);
            free(vat);
            free(va);
            free(f);

            if(model->type & 0x02)
                free(id);




        }
        else
        {

            //nombre de vertex
            write_type_long(fichier,model->nv);

            for(i = 0;i < model->nv;i++)
            {
                write_type_float(fichier,v[i].x);
                write_type_float(fichier,v[i].y);
                write_type_float(fichier,v[i].z);

                write_type_float(fichier,vn[i].x);
                write_type_float(fichier,vn[i].y);
                write_type_float(fichier,vn[i].z);

                write_type_float(fichier,vt[i].x);
                write_type_float(fichier,vt[i].y);

                if(model->type & 0x02)
                write_type_short(fichier,model->id[i]);
            }
            //nombre de face
            write_type_long(fichier,model->nf);

            for(i = 0;i < model->nf*3;i++)
            {
                if(model->f != NULL)
                write_type_short(fichier,model->f[i]);
                else
                write_type_short(fichier,i);
            }

        }

        //texture
        write_type_short(fichier,model->ntexture);
        for(i = 0;i < model->ntexture;i++)
        {

            fputs(model->name[i],fichier);
            n = 20 - strlen(model->name[i]);
            for(l = 0;l < n;l++)
                fputc(0,fichier);

            write_type_long(fichier,model->texture_begin[i]);
        }

        if(model->type & 0x02)
        {
            //Skeleton nodes
            write_type_short(fichier,model->nbones);

            for(i = 0;i < model->nbones;i++)
            {
                for(l = 0;l < 20;l++)
                    fputc(0,fichier);

                write_type_short(fichier,model->bones[i]);
            }

            //Animation
            write_type_short(fichier,model->defaut.ntime);
            for(i = 0;i < model->nbones;i++)
            {
                write_type_float(fichier,model->defaut.skeleton[i].p.x);
                write_type_float(fichier,model->defaut.skeleton[i].p.y);
                write_type_float(fichier,model->defaut.skeleton[i].p.z);
                write_type_float(fichier,model->defaut.skeleton[i].r.x);
                write_type_float(fichier,model->defaut.skeleton[i].r.y);
                write_type_float(fichier,model->defaut.skeleton[i].r.z);
            }
        }



        fclose(fichier);
    }
}
*/
