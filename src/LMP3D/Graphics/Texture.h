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


void LMP3D_Texture_Load(LMP3D_Texture *texture);    //Load Image to RAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture);  //Alloc Texture and send RAM to VRAM
void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture);  //RAM to VRAM
void LMP3D_Texture_Setup(LMP3D_Texture *texbuf);    //Select Texture



void LMP3D_Texture_Free_pixel(LMP3D_Texture *image); //Free Only pixel RAM
void LMP3D_Texture_Free(LMP3D_Texture *texture);     //Free Pixel and texture
void LMP3D_Texture_Delete(LMP3D_Texture *texture);   //Free VRAM texture



#endif
