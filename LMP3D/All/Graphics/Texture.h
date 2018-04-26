#ifndef __LMP3D_Texture__
#define __LMP3D_Texture__

typedef struct
{
	unsigned char bpp,format;
	unsigned short w,h;
	unsigned short pixelsize,palsize;
	unsigned char *pixel,*palette;
	int size;
	unsigned int address,addresspal,psm;

}LMP3D_Texture;


void LMP3D_Texture_Load(LMP3D_Texture *texture);    //Load Image to RAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture);  //Alloc Texture VRAM and send RAM to VRAM
void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture);  //RAM to VRAM
void LMP3D_Texture_Setup(LMP3D_Texture *texbuf);    //Select Texture


void LMP3D_Texture_Free_Pixel(LMP3D_Texture *texture); //Free only pixel and pal
void LMP3D_Texture_Free_RAM(LMP3D_Texture *image);     //Free Only RAM
void LMP3D_Texture_Free_VRAM(LMP3D_Texture *texture);  //Free VRAM texture

void LMP3D_Texture_Format_Init(LMP3D_Texture *texture);
void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture);

int LMP3D_Texture_Convert(LMP3D_Texture *texture,int psmfinal);
int LMP3D_Texture_Convert_Pal(LMP3D_Texture *texture,int psmfinal);


#define LMP3D_FORMAT_RGBA8888	0x01
#define LMP3D_FORMAT_RGBA1555	0x02

#define LMP3D_FORMAT_RGB888		0x04
#define LMP3D_FORMAT_RGB8888	0x05
#define LMP3D_FORMAT_RGB565		0x06
#define LMP3D_FORMAT_RGB555		0x07

#define LMP3D_FORMAT_BGRA8888	0x81
#define LMP3D_FORMAT_BGRA1555	0x82

#define LMP3D_FORMAT_BGR888		0x84
#define LMP3D_FORMAT_BGR8888	0x85
#define LMP3D_FORMAT_BGR565		0x86
#define LMP3D_FORMAT_BGR555		0x87

#define LMP3D_FORMAT_8BPP	0x10
#define LMP3D_FORMAT_4BPP	0x11
#define LMP3D_FORMAT_2BPP	0x12

#define LMP3D_FORMAT_LUM	0x20
#define LMP3D_FORMAT_LUMA	0x21

#endif
