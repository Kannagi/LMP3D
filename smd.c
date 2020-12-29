#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "LMP3D/LMP3D.h"



typedef struct
{
	Vector3 p,r;
}MATRIX3x2;

typedef struct
{
	MATRIX3x2 *skeleton;
	short ntime,ncadence,n;
}MNS_Anim3D;


void MNS_lecture_chaine(char *chaine,char *ch,int nbr)
{
	int i,l = 0,n = 0;

	for(i = 0;i < strlen(chaine)+1;i++)
	{
		if(chaine[i] > 32)
		{
			ch[l] = chaine[i];
			l++;

		}else
		{
			if(l > 0)
			{

				ch[l] =  0;
				l = 0;
				if(n == nbr) return;

				n++;
			}
		}
	}

}

#define TAILLE_MAX 1000

void MNS_rotatation(float rotx,float roty,float rotz,float *x,float *y,float *z)
{
	float tx,ty,tz,c,s;

	//x axis
	ty = *y,tz = *z;

	c = cos(rotx);
	s = sin(rotx);
	*y = ( c * ty ) - ( s * tz );
	*z = ( s * ty ) + ( c * tz );

	//y axis
	tx = *x,tz = *z;

	c = cos(roty);
	s = sin(roty);
	*z = ( c * tz ) - ( s * tx );
	*x = ( s * tz ) + ( c * tx );

	//z axis
	tx = *x,ty = *y;

	c = cos(-rotz);
	s = sin(-rotz);
	*y = ( c * ty ) - ( s * tx );
	*x = ( s * ty ) + ( c * tx );
}


void MNS_SMD_create_skeleton(MNS_Anim3D *anim,int n,short *bones)
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

				MNS_rotatation(rx,ry,rz,&bone[i].x,&bone[i].y,&bone[i].z);

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

LMP3D_Anim3D MNS_load_3D_anim(char *nom_du_fichier)
{
	FILE* fichier = NULL;
	char chaine[TAILLE_MAX] = "";
	MNS_Anim3D *anim;
	anim = malloc(sizeof(MNS_Anim3D));

	int header = 0,k;
	char ch[100];

	int nbones = 0,l = 0;
	short bones[200];

	anim->n = 0;
	anim->ntime = 0;

	LMP3D_Anim3D animr;


	fichier = fopen(nom_du_fichier, "r");
	if(fichier != NULL)
	{

		while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
		{
			MNS_lecture_chaine(chaine,ch,0);

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

		anim->skeleton = malloc(nbones *anim->n * sizeof(MATRIX3x2));

		while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
		{
			MNS_lecture_chaine(chaine,ch,0);

			if(strcmp(ch,"end") == 0)
				header = 0;

			if(header == 1)
			{
				l = atoi(ch);
				MNS_lecture_chaine(chaine,ch,2);
				bones[l] = atoi(ch);
			}

			if(header == 2)
			{
				if(strcmp("time",ch) == 0)
				{
					MNS_lecture_chaine(chaine,ch,1);
					l = atoi(ch);
				}
				else
				{

					k = atoi(ch) + (l*nbones);

					MNS_lecture_chaine(chaine,ch,1);
					anim->skeleton[k].p.x = atof(ch);
					MNS_lecture_chaine(chaine,ch,2);
					anim->skeleton[k].p.y = atof(ch);
					MNS_lecture_chaine(chaine,ch,3);
					anim->skeleton[k].p.z = atof(ch);

					MNS_lecture_chaine(chaine,ch,4);
					anim->skeleton[k].r.x = atof(ch);
					MNS_lecture_chaine(chaine,ch,5);
					anim->skeleton[k].r.y = atof(ch);
					MNS_lecture_chaine(chaine,ch,6);
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


	MNS_SMD_create_skeleton(anim,nbones,bones);


	animr.n = anim->n;
	animr.ncadence = anim->ncadence;
	animr.ntime = anim->ntime;

	animr.skeleton = malloc(nbones *anim->ntime * sizeof(MATRIX4x4));

	float rotateMatrix[16],rotateMatrix2[16],matrixFinal[16];


	int i,n = nbones *anim->ntime;

	printf("%d\n",anim->ntime);

/*
	for(i = 0;i < nbones;i++)
		printf("%f %f %f\n",anim->skeleton[i].p.x, anim->skeleton[i].p.y,anim->skeleton[i].p.z);
*/

	for(i = 0;i < n;i++)
	{
		LMP3D_MatrixIdentity(rotateMatrix);
		LMP3D_MatrixIdentity(matrixFinal);


		//if(i == 3)
		//LMP3D_MatrixRotateX(rotateMatrix, -anim->skeleton[i].r.x);

		/*
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);

		LMP3D_MatrixRotateX(rotateMatrix2, -anim->skeleton[i].r.x);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);

		LMP3D_MatrixRotateZ(rotateMatrix2, -anim->skeleton[i].r.z);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
*/
		//T+S
		LMP3D_MatrixTS(matrixFinal, anim->skeleton[i].p.x, -anim->skeleton[i].p.z,anim->skeleton[i].p.y, 1, 1,1);
		LMP3D_MatrixMultiply(animr.skeleton[i].matrix,rotateMatrix, matrixFinal);



		//LMP3D_MatrixTS(animr.skeleton[i].matrix, anim->skeleton[i].p.x, anim->skeleton[i].p.y,anim->skeleton[i].p.z, 1, 1,1);


		//LMP3D_MatrixIdentity(animr.skeleton[i].matrix);
		//LMP3D_MatrixRotateY(animr.skeleton[i].matrix, PI/4);

		if(i == 0)
			anim->skeleton[i].r.x = 0;

		printf("%f %f %f\n",anim->skeleton[i].r.x,anim->skeleton[i].r.y,anim->skeleton[i].r.z);

		animr.skeleton[i].matrix[0] = anim->skeleton[i].r.x;
		animr.skeleton[i].matrix[1] = anim->skeleton[i].r.y;
		animr.skeleton[i].matrix[2] = anim->skeleton[i].r.z;


	}

	for(i = 0;i < 16;i++)
		printf("%f\n",animr.skeleton[0].matrix[i]);

	return animr;
}

