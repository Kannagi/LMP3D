#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include "LMP3D/LMP3D.h"
void drawvu1(float* matrix,LMP3D_Model *model);

void LMP3D_Model_Draw(LMP3D_Model *model)
{
	float rotateMatrix[16],rotateMatrix2[16];
	float translateMatrix[16];
	float scaleMatrix[16];

	float projectionMatrix[16],cameraMatrix[16];
	float finalMatrix[16];

	LMP3D_MatrixRotateX(rotateMatrix, 1.57 );
	LMP3D_MatrixRotateY(rotateMatrix2, 0 );
	LMP3D_MatrixMultiply(rotateMatrix,rotateMatrix, rotateMatrix2);
	LMP3D_MatrixRotateZ(rotateMatrix2, 0);
	LMP3D_MatrixMultiply(rotateMatrix,rotateMatrix, rotateMatrix2);



	//Translate
	LMP3D_MatrixIdentity(translateMatrix );
	LMP3D_MatrixTranslate(translateMatrix, model->position.x, model->position.y, model->position.z);

	//Scale
	LMP3D_MatrixIdentity(scaleMatrix );
	LMP3D_MatrixTranslate(scaleMatrix, model->scale.x, model->scale.y, model->scale.z);

	//T+S
	LMP3D_MatrixMultiply(cameraMatrix,scaleMatrix, translateMatrix);

	//T+S+R
	LMP3D_MatrixMultiply(cameraMatrix,rotateMatrix, cameraMatrix);




	//final
	LMP3D_MatrixProjection(projectionMatrix);
	LMP3D_MatrixMultiply(finalMatrix,cameraMatrix, projectionMatrix);

	//LMP3D_Texture_Setup(model->texture[0]);
	drawvu1(finalMatrix,model);

}


#endif
