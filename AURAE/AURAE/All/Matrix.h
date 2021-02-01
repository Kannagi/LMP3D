#ifndef __AURAE_Matrix__
#define __AURAE_Matrix__

void AURAE_MatrixIdentity(float* matrix);
void AURAE_MatrixRotateY(float* matrix, float angle);
void AURAE_MatrixRotateX(float* matrix, float angle);
void AURAE_MatrixRotateZ(float* matrix, float angle);
void AURAE_MatrixTranslate(float* matrix, float x, float y, float z);
void AURAE_MatrixScale(float* matrix, float xscale, float yscale, float zscale);
void AURAE_MatrixTS(float* matrix, float x, float y, float z, float xscale, float yscale, float zscale);
void AURAE_MatrixView(AURAE_Model *model,float * matrixFinal);

void AURAE_MatrixPerspective(float* matrix,float fovy, float aspect, float zNear, float zFar);
void AURAE_MatrixOrthogonal(float* matrix);
void AURAE_MatrixMultiply(float* dest,float* src1,float* src2);
float *AURAE_MatrixProjection_Get();

Vector4 AURAE_MatrixVecMultiplyFPU(float* matrix,Vector4 vec);
void AURAE_MatrixVec3MultiplyFPU(float* src1,float* src2);

void AURAE_MatrixRotateXS(short *matrix,short rotate);
void AURAE_MatrixRotateYS(short *matrix,short rotate);
void AURAE_MatrixRotateZS(short *matrix,short rotate);


Vector3 AURAE_TransformPerspectiveFPU(float *matrixFinal,Vector3 vec);


#endif
