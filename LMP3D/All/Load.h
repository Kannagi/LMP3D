#ifndef __LMP3D_Load__
#define __LMP3D_Load__


LMP3D_Texture* LMP3D_Load_Texture(char *filename,int offset,void *buffer,int size);
void LMP3D_Load_Texture_Array(LMP3D_Model *model,char *filename,char *folder,int offset,void *buffer,int size);

//Load Image Format
LMP3D_Texture *LMP3D_Load_png(char *filename,int offset,void *buffer,int size);
LMP3D_Texture *LMP3D_Load_pcx(char *filename,int offset,void *buffer,int size);
LMP3D_Texture *LMP3D_Load_bmp(char *filename,int offset,void *buffer,int size);

//Load Model Format
LMP3D_Model *LMP3D_Load_Model(char *filename,int offset,void *buffer,int size);
LMP3D_Model *LMP3D_Load_bcm(char *filename,int offset,void *buffer,int size);

LMP3D_Anim3D *LMP3D_Load_Anim3D(char *filename,int offset,void *buffer,int size);



#endif

