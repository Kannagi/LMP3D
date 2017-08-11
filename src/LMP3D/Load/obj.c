#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MNS.h"

#define TAILLE_MAX 1000

MNS_Model3D *MNS_load_3D_obj(char *nom_du_fichier)
{
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = "";
    char ch[100],chiffre[100];
    char cht[100],mtl[100];
    cht[0] = 0;
    int i;

    MNS_Model3D *obj;
    obj = malloc(sizeof(MNS_Model3D));


    MNS_Model_init(obj);
    obj->type = 0x00;
    obj->format = 0;

    fichier = fopen(nom_du_fichier, "r");

    int nv,nvt,nvn,nf,ng,nt;
    nv = 0;
    nvt = 0;
    nvn = 0;
    nf = 0;
    ng = 0;
    nt = 0;

    if(fichier != NULL)
    {
        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            MNS_lecture_chaine(chaine,ch,0);

            if(strcmp(ch,"v") == 0)
                nv++;

            if(strcmp(ch,"vt") == 0)
                nvt++;

            if(strcmp(ch,"vn") == 0)
                nvn++;

            if(strcmp(ch,"f") == 0)
                nf++;

            if(strcmp(ch,"g") == 0)
                ng++;

            if(strcmp(ch,"usemtl") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);

                if(strcmp(ch,cht) != 0)
                {
                    strcpy(cht,ch);
                    nt++;
                }
            }

            if(strcmp(ch,"mtllib") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);
                strcpy(mtl,ch);

            }
        }


        obj->nv = nf*3;
        obj->nf = nf;
        obj->v =  malloc(obj->nf * 9 * sizeof(float));
        obj->vt = malloc(obj->nf * 6 * sizeof(float));
        obj->vn = malloc(obj->nf * 9 * sizeof(float));

        obj->ngroupe = ng;
        obj->groupe = malloc(obj->ngroupe * sizeof(short));

        obj->ntexture = nt;
        obj->texture = malloc(obj->ntexture * sizeof(int));
        obj->texture_begin = malloc(obj->ntexture * sizeof(int));
        obj->name = malloc(obj->ntexture * sizeof(char));


        for(i = 0;i < obj->ntexture;i++)
		{
			obj->name[i]          = malloc(100 * sizeof(char));
		}

        cht[0] = 0;

        fseek(fichier, 0, SEEK_SET);

        float *v,*vt,*vn;
        v  =  malloc(nv  * 3 * sizeof(float));
        vt =  malloc(nvt * 2 * sizeof(float));
        vn =  malloc(nvn * 3 * sizeof(float));


        VEC3 *va = (void*)v;
        VEC3 *van = (void*)vn;
        VEC2 *vat = (void*)vt;

        int iv,ivt,ivn,ifc,ig,ib;
        iv = 0;
        ivt = 0;
        ivn = 0;
        ifc = 0;
        ig = 0;
        ib = 0;


        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            MNS_lecture_chaine(chaine,ch,0);

            if(strcmp(ch,"v") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);
                va[iv].x = atof(ch);

                MNS_lecture_chaine(chaine,ch,2);
                va[iv].y = atof(ch);

                MNS_lecture_chaine(chaine,ch,3);
                va[iv].z = atof(ch);

                iv++;
            }

            if(strcmp(ch,"vt") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);
                vat[ivt].x = atof(ch);

                MNS_lecture_chaine(chaine,ch,2);
                vat[ivt].y = atof(ch);

                ivt++;
            }

            if(strcmp(ch,"vn") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);
                van[ivn].x = atof(ch);

                MNS_lecture_chaine(chaine,ch,2);
                van[ivn].y = atof(ch);

                MNS_lecture_chaine(chaine,ch,3);
                van[ivn].z = atof(ch);

                ivn++;
            }
        }

        fseek(fichier, 0, SEEK_SET);

        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            MNS_lecture_chaine(chaine,ch,0);

            if(strcmp(ch,"g") == 0)
            {
                obj->groupe[ig] = ifc;
                ig++;
            }

            if(strcmp(ch,"usemtl") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);

                if(strcmp(ch,cht) != 0)
                {
                    strcpy(cht,ch);
                    strcpy(obj->name[ib],ch);
                    obj->texture_begin[ib] = ifc;
                    ib++;
                }
            }

            if(strcmp(ch,"f") == 0)
            {
                for(i = 0;i < 3;i++)
                {
                    MNS_lecture_chaine(chaine,ch,i+1);
                    MNS_lecture_chaine_ext(ch,chiffre,0,'/');
                    iv = atoi(chiffre) - 1;

                    obj->v[ifc*3 + 0] = va[iv].x;
                    obj->v[ifc*3 + 1] = va[iv].y;
                    obj->v[ifc*3 + 2] = va[iv].z;

                    MNS_lecture_chaine_ext(ch,chiffre,1,'/');
                    ivt = atoi(chiffre) - 1;

                    obj->vt[ifc*2 + 0] = vat[ivt].x;
                    obj->vt[ifc*2 + 1] = vat[ivt].y;

                    MNS_lecture_chaine_ext(ch,chiffre,2,'/');
                    ivn = atoi(chiffre) - 1;

                    obj->vn[ifc*3 + 0] = van[ivn].x;
                    obj->vn[ifc*3 + 1] = van[ivn].y;
                    obj->vn[ifc*3 + 2] = van[ivn].z;

                    ifc++;
                }
            }
        }

        free(v);
        free(vt);
        free(vn);

        fclose(fichier);
    }

    obj->va = obj->v;



    char dossier[100];
    MNS_dossier_out(nom_du_fichier,dossier);

    sprintf(chaine,"%s%s",dossier,mtl);
    MNS_load_3D_mtl(chaine,obj->name,obj->ntexture);

    MNS_load_texture_tab(obj,dossier);


    return obj;
}

void MNS_load_3D_mtl(char *nom_du_fichier,char **name,int n)
{
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = "";
    char ch[100];

    fichier = fopen(nom_du_fichier, "r");
    int i,select = 0;

    if(fichier != NULL)
    {

        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            MNS_lecture_chaine(chaine,ch,0);

            if(strcmp(ch,"newmtl") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);

                for(i = 0;i < n;i++)
                {
                    if(strcmp(ch,name[i]) == 0)
                    {
                        select = i;
                    }
                }
            }


            if(strcmp(ch,"map_Kd") == 0)
            {
                MNS_lecture_chaine(chaine,ch,1);
                strcpy(name[select],ch);
            }
        }

        fclose(fichier);
    }
}
