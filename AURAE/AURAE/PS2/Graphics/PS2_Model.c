#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include "AURAE/AURAE.h"
#include "AURAE/PS2/PS2.h"

void AURAE_Model_Draw(AURAE_Model *model)
{
	float matrixView[16];
	float matrixFinal[16];
	float *matrixProjection  = AURAE_MatrixProjection_Get();

	AURAE_MatrixView(model,matrixView);

	AURAE_MatrixMultiply(matrixFinal,matrixView,matrixProjection);

	PS2_VU_Draw(matrixFinal,model);
}


#endif
