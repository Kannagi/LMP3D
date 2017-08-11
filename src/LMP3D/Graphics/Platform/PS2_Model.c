#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2


#include "LMP3D/LMP3D.h"
void drawvu1(float* matrix,LMP3D_Model *model);

void PS2_MatrixRotateY(float* matrix, float angle);
void PS2_MatrixRotateX(float* matrix, float angle);
void PS2_MatrixRotateZ(float* matrix, float angle);
void PS2_MatrixTranslate(float* matrix, float x, float y, float z);
void PS2_MatrixScale(float* matrix, float xscale, float yscale, float zscale);
void PS2_MatrixProjection(float* matrix);
void PS2_MatrixMultiply(float* dest,const float* src1,const float* src2);

void PS2_MatrixIdentity(float* matrix);
void GsAlpha();

void LMP3D_Draw_Model_GL_VA(LMP3D_Model *model)
{
	//GsAlpha();
	float cameraMatrix[16],temprotMatrix[16],viewscreenMatrix[16];
	float finalMatrix[16] __attribute__((aligned(16)));;

	PS2_MatrixIdentity(cameraMatrix );
	PS2_MatrixRotateX(&cameraMatrix, 1.57 );




	PS2_MatrixIdentity(temprotMatrix );
	PS2_MatrixTranslate(temprotMatrix, model->position.x, model->position.y, model->position.z);


	PS2_MatrixMultiply(cameraMatrix,cameraMatrix, temprotMatrix);




	PS2_MatrixIdentity(viewscreenMatrix );
	PS2_MatrixProjection(viewscreenMatrix );

	PS2_MatrixMultiply(finalMatrix,cameraMatrix, viewscreenMatrix);


	drawvu1(finalMatrix,model);

}


#endif
