#ifndef __AURAE_Window__
#define __AURAE_Window__

typedef struct
{
	int width,height;
	int fpsm,zpsm;
	int faddress1,faddress2,zaddress;
	int switchBuffer;

}AURAE_Buffer;

int AURAE_VBlank(void);
AURAE_Buffer AURAE_Window(char *name);
void AURAE_FlipBuffer(AURAE_Buffer *buffer);

#endif


