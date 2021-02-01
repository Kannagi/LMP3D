#ifndef __AURAE_Texture__
#define __AURAE_Texture__

typedef struct
{
	unsigned char bpp,format;
	unsigned short w,h;
	unsigned short pixelsize,palsize;
	unsigned short color,psm2;
	int size;
	unsigned int address,addresspal,psm;
	unsigned char *pixel,*palette;


}AURAE_Texture;

void AURAE_Texture_Load(AURAE_Texture *texture);	//Load Image to RAM
void AURAE_Texture_Upload(AURAE_Texture *texture);  //Alloc Texture VRAM and send RAM to VRAM
void AURAE_Texture_Upload_VRAM(AURAE_Texture *texture);  //RAM to VRAM
void AURAE_Texture_Setup(AURAE_Texture *texbuf);	//Select Texture

void AURAE_Texture_Free_Pixel(AURAE_Texture *texture); //Free only pixel and pal
void AURAE_Texture_Free(AURAE_Texture *texture);	 //Free

void AURAE_Texture_Format_Init(AURAE_Texture *texture);
void AURAE_Texture_Format_Convert(AURAE_Texture *texture);

int AURAE_Texture_Convert(AURAE_Texture *texture,int psmfinal);
int AURAE_Texture_Convert_Pal(AURAE_Texture *texture,int psmfinal);

unsigned short AURAE_Convert_Pixel(unsigned short pixel);
void AURAE_Texture_Convert_Internal(AURAE_Texture *texture);
unsigned int AURAE_Texture_Setup_Get();
unsigned int AURAE_TexturePal_Setup_Get();
int AURAE_Texture_Palette_Count_Get(AURAE_Texture *texture);


#define AURAE_FORMAT_RGBA8888	0x01
#define AURAE_FORMAT_RGBA1555	0x02

#define AURAE_FORMAT_RGB888		0x04
#define AURAE_FORMAT_RGB8888	0x05
#define AURAE_FORMAT_RGB565		0x06
#define AURAE_FORMAT_RGB555		0x07

#define AURAE_FORMAT_BGRA8888	0x81
#define AURAE_FORMAT_BGRA1555	0x82

#define AURAE_FORMAT_BGR888		0x84
#define AURAE_FORMAT_BGR8888	0x85
#define AURAE_FORMAT_BGR565		0x86
#define AURAE_FORMAT_BGR555		0x87

#define AURAE_FORMAT_8BPP	0x10
#define AURAE_FORMAT_4BPP	0x11
#define AURAE_FORMAT_2BPP	0x12

#define AURAE_FORMAT_LUM	0x20
#define AURAE_FORMAT_LUMA	0x21

#endif
