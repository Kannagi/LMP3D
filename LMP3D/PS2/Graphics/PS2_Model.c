#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include "LMP3D/LMP3D.h"
void drawvu0(float* matrix,void *data,int n);
void drawvu1(float* matrix,LMP3D_Model *model);

void LookAt(float *projectionMatrix,float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz );


void LMP3D_Model_Draw(LMP3D_Model *model)
{
	float rotateMatrix[16],rotateMatrix2[16];
	float projectionMatrix[16],cameraMatrix[16];
/*
	LookAt(rotateMatrix,0, 0, z,
		0, 0,-1,
		0, 1,0 );*/
	int rotate = 1;

	model->rotate.x = PI/2;

	if(model->rotate.y != 0)
	{
		LMP3D_MatrixRotateY(rotateMatrix, model->rotate.y);

		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}

		if(model->rotate.z != 0)
		{
			LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
			LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
		}
	}else
	{
		if(model->rotate.x != 0)
		{
			LMP3D_MatrixRotateX(rotateMatrix, model->rotate.x);

			if(model->rotate.z != 0)
			{
				LMP3D_MatrixRotateZ(rotateMatrix2, model->rotate.x);
				LMP3D_MatrixMultiply(rotateMatrix, rotateMatrix2,rotateMatrix);
			}
		}else
		{
			if(model->rotate.z != 0)
				LMP3D_MatrixRotateZ(rotateMatrix, model->rotate.z);
			else rotate = 0;
		}
	}

	//T+S
	LMP3D_MatrixTS(cameraMatrix, model->position.x, model->position.y, model->position.z, model->scale.x, model->scale.y, model->scale.z);

	if(rotate == 1)
	{
		//T+S+R
		LMP3D_MatrixMultiply(cameraMatrix,rotateMatrix, cameraMatrix);
	}


	//final
	LMP3D_MatrixProjection(projectionMatrix);

	LMP3D_MatrixMultiply(projectionMatrix,cameraMatrix, projectionMatrix);

	drawvu1(projectionMatrix,model);
	//drawvu0(projectionMatrix,model->v,30);
}


#endif
