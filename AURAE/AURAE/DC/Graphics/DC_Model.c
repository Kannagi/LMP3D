#include <stdio.h>
#include <stdlib.h>

#ifdef DREAMCAST
#include <kos.h>

#include "AURAE/AURAE.h"
#include "AURAE/DC/DC.h"

void AURAE_Model_Draw(AURAE_Model *model)
{
	float matrixView[16];
	float matrixFinal[16];

	AURAE_MatrixView(model,matrixView);

	float *matrixProjection = AURAE_MatrixProjection_Get();
	AURAE_MatrixMultiply(matrixFinal,matrixView,matrixProjection);

	DC_Matrix_Load(matrixFinal);

	DC_DrawModel(model);
}

#endif

