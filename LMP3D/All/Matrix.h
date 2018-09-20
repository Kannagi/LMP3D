#ifndef __LMP3D_Matrix__
#define __LMP3D_Matrix__

void LMP3D_MatrixIdentity(float* matrix);
void LMP3D_MatrixRotateY(float* matrix, float angle);
void LMP3D_MatrixRotateX(float* matrix, float angle);
void LMP3D_MatrixRotateZ(float* matrix, float angle);
void LMP3D_MatrixTranslate(float* matrix, float x, float y, float z);
void LMP3D_MatrixScale(float* matrix, float xscale, float yscale, float zscale);
void LMP3D_MatrixTS(float* matrix, float x, float y, float z, float xscale, float yscale, float zscale);
void LMP3D_MatrixView(LMP3D_Model *model,float * matrixFinal);

void LMP3D_MatrixPerspective(float* matrix,float fovy, float aspect, float zNear, float zFar);
void LMP3D_MatrixOrthogonal(float* matrix);
void LMP3D_MatrixMultiply(float* dest,float* src1,float* src2);
float *LMP3D_MatrixProjection_Get();

void LMP3D_MatrixRotateXS(short *matrix,short rotate);
void LMP3D_MatrixRotateYS(short *matrix,short rotate);
void LMP3D_MatrixRotateZS(short *matrix,short rotate);


Vector3 LMP3D_TransformPerspectiveFPU(float *matrixFinal,Vector3 vec);


#endif
