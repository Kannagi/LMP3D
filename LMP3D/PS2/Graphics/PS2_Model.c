#include <stdio.h>
#include <stdlib.h>

#ifdef PLAYSTATION2

#include "LMP3D/LMP3D.h"
#include "LMP3D/PS2/PS2.h"

void LMP3D_Model_Draw(LMP3D_Model *model)
{
	float matrixView[16];
	float matrixFinal[16];
	float *matrixProjection  = LMP3D_MatrixProjection_Get();

	LMP3D_MatrixView(model,matrixView);

	LMP3D_MatrixMultiply(matrixFinal,matrixView,matrixProjection);

	PS2_VU_Draw(matrixFinal,model);
}


#endif
