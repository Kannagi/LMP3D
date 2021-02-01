#ifndef __AURAE_Load__
#define __AURAE_Load__


AURAE_Texture* AURAE_Load_Texture(char *filename,int offset,void *buffer,int size);
void AURAE_Load_Texture_Array(AURAE_Model *model,char *filename,char *folder,int offset,void *buffer,int size);

//Load Image Format
AURAE_Texture *AURAE_Load_png(char *filename,int offset,void *buffer,int size);
AURAE_Texture *AURAE_Load_pcx(char *filename,int offset,void *buffer,int size);
AURAE_Texture *AURAE_Load_bmp(char *filename,int offset,void *buffer,int size);

//Load Model Format
AURAE_Model *AURAE_Load_Model(char *filename,int offset,void *buffer,int size);
AURAE_Model *AURAE_Load_bcm(char *filename,int offset,void *buffer,int size);

AURAE_Anim3D *AURAE_Load_Anim3D(char *filename,int offset,void *buffer,int size);

#endif

