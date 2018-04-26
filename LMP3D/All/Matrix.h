#ifndef __LMP3D_Matrix__
#define __LMP3D_Matrix__

void LMP3D_MatrixIdentity(float* matrix);
void LMP3D_MatrixRotateY(float* matrix, float angle);
void LMP3D_MatrixRotateX(float* matrix, float angle);
void LMP3D_MatrixRotateZ(float* matrix, float angle);
void LMP3D_MatrixTranslate(float* matrix, float x, float y, float z);
void LMP3D_MatrixScale(float* matrix, float xscale, float yscale, float zscale);
void LMP3D_MatrixTS(float* matrix, float x, float y, float z, float xscale, float yscale, float zscale);

void LMP3D_MatrixProjection(float* matrix);
void LMP3D_MatrixOrthogonal(float* matrix);
void LMP3D_MatrixMultiply(float* dest,float* src1,float* src2);

#endif
