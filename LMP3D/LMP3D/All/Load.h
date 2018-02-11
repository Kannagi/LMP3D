#ifndef __LMP3D_Load__
#define __LMP3D_Load__


LMP3D_Texture* LMP3D_Load_Texture(const char *filename,int offset,void *buffer);
void LMP3D_Load_Texture_Array(LMP3D_Model *model,const char *filename,char *folder,int offset,void *buffer);

//Load Image Format
LMP3D_Texture *LMP3D_Load_png(const char *filename,int offset,void *buffer);
LMP3D_Texture *LMP3D_Load_pcx(const char *filename,int offset,void *buffer);
LMP3D_Texture *LMP3D_Load_bmp(const char *filename,int offset,void *buffer);

//Load Model Format
LMP3D_Model *LMP3D_Load_Model(const char *filename,int offset,void *buffer);
LMP3D_Model *LMP3D_Load_bcm(const char *filename,int offset,void *buffer);

LMP3D_Anim3D *LMP3D_Load_Anim3D(const char *filename,int offset,void *buffer);



#endif

