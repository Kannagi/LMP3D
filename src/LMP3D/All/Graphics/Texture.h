#ifndef __LMP3D_Texture__
#define __LMP3D_Texture__

typedef struct
{
  unsigned short w;
  unsigned short h;
  short bpp;
  short pixelformat;
  unsigned char *pixel,*palette;
  int size,palsize;
  int pixelsize,type;

  //for OpenGL
  int format;

  unsigned int address,paladresse,psm;

}LMP3D_Texture;


void LMP3D_Texture_Load(LMP3D_Texture *texture);    //Load Image to RAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture);  //Alloc Texture VRAM and send RAM to VRAM
void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture);  //RAM to VRAM
void LMP3D_Texture_Setup(LMP3D_Texture *texbuf);    //Select Texture


void LMP3D_Texture_Free_Pixel(LMP3D_Texture *texture);  //Free only pixel and pal
void LMP3D_Texture_Free_RAM(LMP3D_Texture *image);     //Free Only RAM
void LMP3D_Texture_Free_VRAM(LMP3D_Texture *texture);  //Free VRAM texture

#define LMP3D_FORMAT_RGBA	0x00
#define LMP3D_FORMAT_RGBA16	0x01

#define LMP3D_FORMAT_RGB	0x02
#define LMP3D_FORMAT_RGB15	0x04

#define LMP3D_FORMAT_8BPP	0x08
#define LMP3D_FORMAT_4BPP	0x10
#define LMP3D_FORMAT_2BPP	0x20

#define LMP3D_FORMAT_LUM	0x40
#define LMP3D_FORMAT_LUMA	0x80

#endif
