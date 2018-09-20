#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "LMP3D/LMP3D.h"
#include "LMP3D/DC/DC.h"

void LMP3D_Model_Draw(LMP3D_Model *model)
{
	float matrixView[16];
	float matrixFinal[16];

	LMP3D_MatrixView(model,matrixView);

	float *matrixProjection = LMP3D_MatrixProjection_Get();
	LMP3D_MatrixMultiply(matrixFinal,matrixView,matrixProjection);

	DC_Matrix_Load(matrixFinal);

	DC_DrawModel(model);
}

#endif

