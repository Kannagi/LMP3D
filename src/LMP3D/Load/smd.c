#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>




#include "LMP3D/LMP3D.h"

#define TAILLE_MAX 1000




LMP3D_Model *LMP3D_Load_Model_smd(char *nom_du_fichier)
{
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = "";

    LMP3D_Model *obj;
    obj = malloc(sizeof(LMP3D_Model));

    char chiffre[100];
    char ch[100],dossier[100],material[100];
    material[0] = 0;

    int header = 0,lecture = 0,k,l = 0,ifc = 0,ib = 0,v = 0,i;
    dossier[0] = 0;

    LMP3D_Model_Init(obj);
    obj->type = 0x02;
    obj->format = 1;



    fichier = fopen(nom_du_fichier, "r");
    if(fichier != NULL)
    {
        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            LMP3D_Script_Read(chaine,ch,0);

            if(strcmp(ch,"end") == 0)
                header = 0;

            if(header == 1)
                obj->nbones++;

            if(header == 2)
            {
                if(strcmp("time",ch) == 0)
                    obj->defaut.ntime++;
                else
                    obj->defaut.n++;
            }

            if(header == 3)
            {
                if(lecture == 0)
                {
                    if(strcmp(material,ch) != 0)
                    {
                        strcpy(material,ch);
                        obj->ntexture++;
                    }
                    obj->nf++;
                }

                if(lecture == 3)
                    lecture = -1;

                lecture++;
            }

            if(strcmp(ch,"nodes") == 0)
                header = 1;

            if(strcmp(ch,"skeleton") == 0)
                header = 2;

            if(strcmp(ch,"triangles") == 0)
                header = 3;

        }


        obj->nv = 3*obj->nf;
        obj->v  = malloc(obj->nv * 3 * sizeof(float));
        obj->vt = malloc(obj->nv * 2 * sizeof(float));
        obj->vn = malloc(obj->nv * 3 * sizeof(float));
        obj->va = malloc(obj->nv * 3 * sizeof(float));

        obj->texture       = malloc(obj->ntexture * sizeof(int));
        obj->texture_begin = malloc(obj->ntexture * sizeof(int));
        obj->name          = malloc(obj->ntexture * sizeof(char));


        obj->id              = malloc(obj->nv * 1 * sizeof(short));
        obj->bones           = malloc(obj->nbones * 1 * sizeof(short));
        obj->nodes           = malloc(obj->nbones * 1 * sizeof(Vector3));
        obj->defaut.skeleton = malloc(obj->defaut.n * 1 * sizeof(Matrix3x2));

        fseek(fichier, 0, SEEK_SET);
        material[0] = 0;

		for(i = 0;i < obj->ntexture;i++)
		{
			obj->name[i]          = malloc(100 * sizeof(char));
		}

        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            LMP3D_Script_Read(chaine,ch,0);

            if(strcmp(ch,"end") == 0)
                header = 0;

            if(header == 1)
            {
                l = atoi(ch);
                LMP3D_Script_Read(chaine,ch,2);
                obj->bones[l] = atoi(ch);
            }

            if(header == 2)
            {
                if(strcmp("time",ch) == 0)
                {
                    LMP3D_Script_Read(chaine,ch,1);
                    l = atoi(ch);
                }
                else
                {
                    k = atoi(ch) + (l*obj->nbones);

                    LMP3D_Script_Read(chaine,ch,1);
                    obj->defaut.skeleton[k].p.x = atof(ch);
                    LMP3D_Script_Read(chaine,ch,2);
                    obj->defaut.skeleton[k].p.y = atof(ch);
                    LMP3D_Script_Read(chaine,ch,3);
                    obj->defaut.skeleton[k].p.z = atof(ch);

                    LMP3D_Script_Read(chaine,ch,4);
                    obj->defaut.skeleton[k].r.x = atof(ch);
                    LMP3D_Script_Read(chaine,ch,5);
                    obj->defaut.skeleton[k].r.y = atof(ch);
                    LMP3D_Script_Read(chaine,ch,6);
                    obj->defaut.skeleton[k].r.z = atof(ch);
                }

            }

            if(header == 3)
            {
                if(lecture == 0)
                {
                    if(strcmp(material,ch) != 0)
                    {
                        strcpy(material,ch);
                        strcpy(obj->name[ib],ch);
                        obj->texture_begin[ib] = ifc*3;

                        ib++;
                    }
                    ifc++;
                }

                if(lecture >= 1)
                {
                    LMP3D_Script_Read(chaine,chiffre,1);
                    obj->v[v*3 + 0] = atof(chiffre);
                    LMP3D_Script_Read(chaine,chiffre,2);
                    obj->v[v*3 + 1] = atof(chiffre);
                    LMP3D_Script_Read(chaine,chiffre,3);
                    obj->v[v*3 + 2] = atof(chiffre);


                    obj->va[v*3 + 0] = obj->v[v*3 + 0];
                    obj->va[v*3 + 1] = obj->v[v*3 + 1];
                    obj->va[v*3 + 2] = obj->v[v*3 + 2];

                    LMP3D_Script_Read(chaine,chiffre,4);
                    obj->vn[v*3 + 0] = atof(chiffre);
                    LMP3D_Script_Read(chaine,chiffre,5);
                    obj->vn[v*3 + 1] = atof(chiffre);
                    LMP3D_Script_Read(chaine,chiffre,6);
                    obj->vn[v*3 + 2] = atof(chiffre);

                    LMP3D_Script_Read(chaine,chiffre,7);
                    obj->vt[v*2 + 0] = atof(chiffre);
                    LMP3D_Script_Read(chaine,chiffre,8);
                    obj->vt[v*2 + 1] = atof(chiffre)*-1;

                    LMP3D_Script_Read(chaine,chiffre,0);
                    obj->id[v] = atoi(chiffre);

                    v++;

                    if(lecture == 3)
                        lecture = -1;
                }

                lecture++;
            }

            if(strcmp(ch,"nodes") == 0)
                header = 1;

            if(strcmp(ch,"skeleton") == 0)
                header = 2;

            if(strcmp(ch,"triangles") == 0)
                header = 3;
        }

        fclose(fichier);
    }

    LMP3D_Folder_Out(nom_du_fichier,dossier);
    LMP3D_Load_Texture_Array(obj,dossier);

    LMP3D_SMD_Create_Skeleton(&obj->defaut,obj->nbones,obj->bones);

    return obj;
}


LMP3D_Anim3D *LMP3D_Load_Anim3D(char *nom_du_fichier)
{
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = "";
    LMP3D_Anim3D *anim;
    anim = malloc(sizeof(LMP3D_Anim3D));

    int header = 0,k;
    char ch[100];

    int nbones = 0,l = 0;
    short bones[200];

    anim->n = 0;
    anim->ntime = 0;


    fichier = fopen(nom_du_fichier, "r");
    if(fichier != NULL)
    {

        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            LMP3D_Script_Read(chaine,ch,0);

            if(strcmp(ch,"end") == 0)
                header = 0;

            if(header == 1)
                nbones++;

            if(header == 2)
            {
                if(strcmp("time",ch) == 0)
                    anim->ntime++;
                else
                    anim->n++;
            }

            if(strcmp(ch,"skeleton") == 0)
                header = 2;

            if(strcmp(ch,"nodes") == 0)
                header = 1;
        }

        fseek(fichier, 0, SEEK_SET);

        anim->skeleton = malloc(anim->n * sizeof(Matrix3x2));

        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            LMP3D_Script_Read(chaine,ch,0);

            if(strcmp(ch,"end") == 0)
                header = 0;

            if(header == 1)
            {
                l = atoi(ch);
                LMP3D_Script_Read(chaine,ch,2);
                bones[l] = atoi(ch);
            }

            if(header == 2)
            {
                if(strcmp("time",ch) == 0)
                {
                    LMP3D_Script_Read(chaine,ch,1);
                    l = atoi(ch);
                }
                else
                {

                    k = atoi(ch) + (l*nbones);

                    LMP3D_Script_Read(chaine,ch,1);
                    anim->skeleton[k].p.x = atof(ch);
                    LMP3D_Script_Read(chaine,ch,2);
                    anim->skeleton[k].p.y = atof(ch);
                    LMP3D_Script_Read(chaine,ch,3);
                    anim->skeleton[k].p.z = atof(ch);

                    LMP3D_Script_Read(chaine,ch,4);
                    anim->skeleton[k].r.x = atof(ch);
                    LMP3D_Script_Read(chaine,ch,5);
                    anim->skeleton[k].r.y = atof(ch);
                    LMP3D_Script_Read(chaine,ch,6);
                    anim->skeleton[k].r.z = atof(ch);
                }

            }

            if(strcmp(ch,"skeleton") == 0)
                header = 2;

            if(strcmp(ch,"nodes") == 0)
                header = 1;
        }

        fclose(fichier);
    }


    LMP3D_SMD_Create_Skeleton(anim,nbones,bones);

    return anim;
}


void LMP3D_SMD_Create_Skeleton(LMP3D_Anim3D *anim,int n,short *bones)
{
    Vector3 bone[200],p[200];
    int id,i,l;
    float rx,ry,rz;




    for(l = 0;l < anim->ntime;l++)
    {
        for(i = 0;i < n;i++)
        {
            id = i;
            bone[i].x = 0;
            bone[i].y = 0;
            bone[i].z = 0;

            while(1)
            {

                bone[i].x += anim->skeleton[id + l*n].p.x;
                bone[i].y += anim->skeleton[id + l*n].p.y;
                bone[i].z += anim->skeleton[id + l*n].p.z;

                id = bones[id];

                if(id == -1) break;
            }

            p[i].x = bone[i].x;
            p[i].y = bone[i].y;
            p[i].z = bone[i].z;
        }


        for(i = 0;i < n;i++)
        {
            id = i;

            while(1)
            {
                bone[i].x -= p[id].x;
                bone[i].y -= p[id].y;
                bone[i].z -= p[id].z;

                rx = anim->skeleton[id +  l*n].r.x;
                ry = anim->skeleton[id +  l*n].r.y;
                rz = anim->skeleton[id +  l*n].r.z;

                LMP3D_Math_Rotation3D(rx,ry,rz,&bone[i].x,&bone[i].y,&bone[i].z);

                bone[i].x += p[id].x;
                bone[i].y += p[id].y;
                bone[i].z += p[id].z;

                id = bones[id];

                if(id == -1) break;
            }

            anim->skeleton[i +  l*n].p.x = bone[i].x;
            anim->skeleton[i +  l*n].p.y = bone[i].y;
            anim->skeleton[i +  l*n].p.z = bone[i].z;

        }
    }
}
