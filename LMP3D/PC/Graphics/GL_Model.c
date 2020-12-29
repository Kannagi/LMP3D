#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PC
#include <math.h>
#include <GL/gl.h>

#include "LMP3D/LMP3D.h"

void LMP3D_MatrixView(LMP3D_Model *model,float * matrixFinal);
Vector3 LMP3D_MatrixVecTransformPerspectiveFPU(float* matrix,Vector3 vec);
float *LMP3D_MatrixProjection_Get();

float *LMP3D_Camera_LookAt_Get();
void MNS_draw_skeleton_anim(LMP3D_Model *model,int j)
{
	if(model->nbones == 0) return;

	glBindTexture(GL_TEXTURE_2D,0);
	int i,l,k;
	float x,y,z;
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(i = 0;i < model->nbones;i++)
	{
		l = i + (j*model->nbones);
		k = i*16;
/*
		x = model->anim.skeleton[l].matrix[0+12] - model->matrix_bones[k+12+0] ;
		y = -(model->anim.skeleton[l].matrix[1+12] + model->matrix_bones[k+12+1]) ;
		z = (model->anim.skeleton[l].matrix[2+12] + model->matrix_bones[k+12+2]) ;
		*/

		x = model->anim.skeleton[l].matrix[0+12];
		y = -model->anim.skeleton[l].matrix[1+12];
		z = -model->anim.skeleton[l].matrix[2+12];

		glBegin(GL_POINTS);
		glVertex3f(x,y,z);
		glEnd();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	return 0;
}

void MNS_draw_skeleton(LMP3D_Model *model)
{
	if(model->nbones == 0) return;

	glBindTexture(GL_TEXTURE_2D,0);
	int i,l;
	float x,y,z;
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(i = 0;i < model->nbones;i++)
	{
		l = i*16;

		x = model->matrix_bones[l+12+0];
		y = -model->matrix_bones[l+12+1];
		z = -model->matrix_bones[l+12+2];

		glBegin(GL_POINTS);
		glVertex3f(x,y,z);
		glEnd();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	return;

	glColor3f(0.0f, 1.0f, 0.0f);
	int id;
	for(i = 0;i < model->nbones;i++)
	{
		l = i*16;

		x = model->matrix_bones[l+3+0];
		y = -model->matrix_bones[l+3+4];
		z = model->matrix_bones[l+3+8];

		glBegin(GL_LINES);
		glVertex3f( x,y,z);

		i++;
		l = i*16;

		x = model->matrix_bones[l+3+0];
		y = -model->matrix_bones[l+3+4];
		z = model->matrix_bones[l+3+8];

		glVertex3f( x,y,z);
		glEnd();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void LMP3D_Model_Draw_Test(LMP3D_Model *model,float *matrixView,float *buffer_vertex)
{
	int i;
	float matrix[16];
	glMatrixMode(GL_PROJECTION);
	LMP3D_MatrixIdentity(matrix);
	glLoadMatrixf(matrix);
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(matrix);


	float *matrixProjection  = LMP3D_MatrixProjection_Get();
	LMP3D_MatrixMultiply(matrix,matrixView,matrixProjection);

	memcpy(buffer_vertex,model->v,model->nv*3*4);

	Vector3 vec;
	for(i = 0;i < model->nv*3;i+=3)
	{
		vec.x = buffer_vertex[i+0];
		vec.y = buffer_vertex[i+1];
		vec.z = buffer_vertex[i+2];
		vec = LMP3D_TransformPerspectiveFPU(matrix,vec);
		buffer_vertex[i+0] = vec.x;
		buffer_vertex[i+1] = vec.y;
		buffer_vertex[i+2] = vec.z;
	}
}

void LMP3D_Model_Draw_Test2(LMP3D_Model *model,float *buffer_vertex)
{
	int i;
	memcpy(buffer_vertex,model->v,model->nv*3*4);

	static int n = 0;
	static int tempo = 0;

	tempo++;
	if(tempo > 60)
	{
		tempo = 0;
		n++;
		if(n >= model->nbones) n = 0;
	}
	int j;

//printf("%d , %d : %d /%d\n",model->nbones,n ,model->bones[n],model->bones[n+1]);
	for(i = model->bones[n];i < model->bones[n+1];i++)
	{
		j = i*3;
		buffer_vertex[j] += 50;
	}
}




void LMP3D_Model_Draw_TestAnim(LMP3D_Model *model,float *buffer_vertex)
{
	int i,l,k;
	memcpy(buffer_vertex,model->v,model->nv*3*4);

	static int n = 0;
	static int tempo = 0;

	tempo++;
	if(tempo > 60)
	{
		tempo = 0;
		n++;
		if(n >= model->anim.ntime) n = 0;
	}
	int j = 0;

//printf("%d , %d : %d /%d\n",model->nbones,n ,model->bones[n],model->bones[n+1]);
/*
	for(l = 0;l < model->nbones;l++)
	{
		k = l*16;
		printf("%d : %f %f\n",l,model->matrix_bones[k+3+4],model->anim.skeleton[0].matrix[k+3+4]);
	}
*/
	float rotateMatrix[16],rotateMatrix2[16],matrixFinal[16];
	/*
	printf("---------\n");
	for(i = 0;i < 16;i++)
		printf("%f\n",model->matrix_bones[i]);

			printf("---------\n");
	for(i = 0;i < 16;i++)
		printf("%f\n",model->anim.skeleton[0].matrix[i]);
	*/

	for(l = 0;l < model->nbones;l++)
	{
		LMP3D_MatrixIdentity(rotateMatrix);
		k = l*16;

		float x,y,z;

		x = model->anim.skeleton[l].matrix[0+12];
		y = model->anim.skeleton[l].matrix[1+12];
		z = model->anim.skeleton[l].matrix[2+12];


		float px,py,pz;
		px = model->matrix_bones[k+12+0];
		py = model->matrix_bones[k+12+1];
		pz = model->matrix_bones[k+12+2];

		//LMP3D_MatrixTS(rotateMatrix2,x,y,z, 1, 1,1);

		//LMP3D_MatrixRotateX(rotateMatrix, model->anim.skeleton[l].matrix[0+3]);
		//LMP3D_MatrixMultiply(rotateMatrix2, rotateMatrix,rotateMatrix2);
		//LMP3D_MatrixMultiply(rotateMatrix2,model->anim.skeleton[l].matrix,rotateMatrix2);

		//rotateMatrix2[3+0] = -rotateMatrix2[3+0];
		//rotateMatrix2[3+8] = -100;
		//rotateMatrix2[3+4] = -rotateMatrix2[3+4];

		//LMP3D_MatrixRotate(rotateMatrix, (float)l/20.0);
		//LMP3D_MatrixMultiply(rotateMatrix2, rotateMatrix,rotateMatrix2);


		float rx,ry,rz;

		rx = model->anim.skeleton[l].matrix[0];
		ry = model->anim.skeleton[l].matrix[1];
		rz = model->anim.skeleton[l].matrix[2];
		LMP3D_MatrixRotateZ(rotateMatrix, rz);

		LMP3D_MatrixRotateX(rotateMatrix2, rx);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);

		LMP3D_MatrixRotateY(rotateMatrix2, ry);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);

		if(l == 16)
		{
			//printf("%f %f %f\n",rx,ry,rz);
		}

		for(i = model->bones[l];i < model->bones[l+1];i++)
		{
			j = i*3;
			//buffer_vertex[j] += 50;

			k = l;

			buffer_vertex[j+0] += px;
			buffer_vertex[j+1] += py;
			buffer_vertex[j+2] += pz;

			//

			LMP3D_MatrixVec3MultiplyFPU(&buffer_vertex[j+0],rotateMatrix);

			if(l == 16)
				buffer_vertex[j+0] += 50;
			buffer_vertex[j+0] += x;
			buffer_vertex[j+1] -= y;
			buffer_vertex[j+2] -= z;
		}
	}



/*
		vec.x = buffer_vertex[j+0];
		vec.y = buffer_vertex[j+1];
		vec.z = buffer_vertex[j+2];

		vec = LMP3D_MatrixVecMultiplyFPU(model->anim.skeleton[k].matrix,vec);

		buffer_vertex[j+0] = vec.x;
		buffer_vertex[j+1] = vec.y;
		buffer_vertex[j+2] = vec.z;

*/

	/*
	Vector4 vec;
	vec.w = 0;
	for(l = 0;l < model->nbones;l++)
	for(i = model->bones[l];i < model->bones[l+1];i++)
	{
		j = i*3;
		//buffer_vertex[j] += 50;

		k = l*16;
		x = model->matrix_bones[k+3+0]-model->anim.skeleton[0].matrix[k+3+0];
		buffer_vertex[j+0] += x;
		vec.x = x;



		x = model->matrix_bones[k+3+4]-model->anim.skeleton[0].matrix[k+3+4];
		buffer_vertex[j+1] += x;
		vec.y = x;


		x = model->anim.skeleton[0].matrix[k+3+8]-model->matrix_bones[k+3+8];
		buffer_vertex[j+2] += x;
		vec.z = x;
		//buffer_vertex[j+1] = model->anim.skeleton[0].matrix[k+3+4];
		//buffer_vertex[j+2] = model->anim.skeleton[0].matrix[k+3+8];


		LMP3D_MatrixRotateY(rotateMatrix, model->anim.skeleton[0].r.y);

		LMP3D_MatrixRotateX(rotateMatrix2, model->anim.skeleton[0].r.x);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);

		LMP3D_MatrixRotateZ(rotateMatrix2, model->anim.skeleton[0].r.z);
		LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);


		vec = LMP3D_MatrixVecMultiplyFPU(rotateMatrix,vec);

		buffer_vertex[j+0] += vec.x;
		buffer_vertex[j+0] += vec.y;
		buffer_vertex[j+0] += vec.z;


	}

	*/
}


float buffer_vertex[0x10000];
void LMP3D_Model_Draw(LMP3D_Model *model)
{
	//glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	float matrixView[16];
	LMP3D_MatrixView(model,matrixView);
	glLoadMatrixf(matrixView);

	float *vertex = model->v;
/*
	if(model->nbones != 0)
	{
		LMP3D_Model_Draw_TestAnim(model,buffer_vertex);
		vertex = buffer_vertex;
	}*/

	int i,end,begin,sizeindex = GL_UNSIGNED_SHORT;
	if(model->flag & LMP3D_MODEL_INDEX_U32) sizeindex = GL_UNSIGNED_INT;

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(3,GL_FLOAT,0,vertex);

	if(model->flag & LMP3D_MODEL_TEXTCOORD)
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2,GL_FLOAT,0,model->vt);
	}


	if(model->flag & LMP3D_MODEL_NORMAL)
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer(GL_FLOAT,0,model->vn);
	}

	//model->ntexture = 0;
	if((model->flag & LMP3D_MODEL_TEXTUREDISABLE) || model->ntexture == 0)
	{
		if(model->flag & LMP3D_MODEL_TEXTUREDISABLE)
			glBindTexture(GL_TEXTURE_2D,0);

		if(model->flag & LMP3D_MODEL_INDEX)
			glDrawElements(GL_TRIANGLES,model->nf*3,sizeindex,model->index);
		else
			glDrawArrays(GL_TRIANGLES,0,model->nf*3);
	}
	else
	{
		void *index;
		for(i = 0;i < model->ntexture;i++)
		{
			glBindTexture(GL_TEXTURE_2D,model->texture[i]->address);

			begin = model->texture_begin[i];

			if(i+1 < model->ntexture)
				end = model->texture_begin[i+1];
			else
				end = model->nf*3;

			end -= begin;

			index = model->index;
			index += begin<<1;

			if(model->flag & LMP3D_MODEL_INDEX)
				glDrawElements(GL_TRIANGLES,end,sizeindex,index);
			else
				glDrawArrays( GL_TRIANGLES, begin,end);
		}
	}

	if(model->flag & LMP3D_MODEL_NORMAL)
		glDisableClientState( GL_NORMAL_ARRAY );

	if(model->flag & LMP3D_MODEL_TEXTCOORD)
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_CULL_FACE);

	//MNS_draw_skeleton(model);
	//MNS_draw_skeleton_anim(model,0);
}



#endif
