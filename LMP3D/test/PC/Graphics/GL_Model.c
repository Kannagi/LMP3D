#include <stdio.h>
#include <stdlib.h>

#ifdef PC
#include <math.h>
#include <GL/gl.h>

#include "LMP3D/LMP3D.h"

void LMP3D_MatrixView(LMP3D_Model *model,float * matrixFinal);
Vector3 LMP3D_MatrixVecTransformPerspectiveFPU(float* matrix,Vector3 vec);
float *LMP3D_MatrixProjection_Get();

float *LMP3D_Camera_LookAt_Get();

void LMP3D_Model_Draw(LMP3D_Model *model)
{
	//glEnable(GL_CULL_FACE);

	float matrixView[16];
	LMP3D_MatrixView(model,matrixView);
	glLoadMatrixf(matrixView);
	/*
	float *matrix = LMP3D_MatrixProjection_Get();
	LMP3D_MatrixMultiply(matrix,matrixView,matrix);

	Vector3 vec;
	vec.x = model->position.x;
	vec.y = model->position.y;
	vec.z = model->position.z;

	vec = LMP3D_TransformPerspectiveFPU(matrix,vec);
	*/
	int i,end,begin,sizeindex = GL_UNSIGNED_SHORT;

	if(model->flag & LMP3D_MODEL_INDEX_U32) sizeindex = GL_UNSIGNED_INT;

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(3,GL_FLOAT,0,model->v);

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

			//end = 800*3;

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
}

#endif
