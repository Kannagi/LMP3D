#ifndef __LMP3D_Texture__
#define __LMP3D_Texture__

typedef struct
{
  unsigned short w;
  unsigned short h;
  short bpp;
  short pixelformat;
  unsigned char *pixel,*palette;
  int size;
  int pixelsize;

  //for OpenGL
  int format;
  int internalformat;
  int id;

  //for PS2
  int address,psm;


}LMP3D_Texture;


void LMP3D_Texture_Load(LMP3D_Texture *image);
void LMP3D_Texture_Bind(LMP3D_Texture *image);
void LMP3D_Texture_Set(LMP3D_Texture *image);
void LMP3D_Texture_Free(LMP3D_Texture *image);
void LMP3D_Texture_Delete(int id);


#endif
