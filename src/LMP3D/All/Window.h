#ifndef __LMP3D_Window__
#define __LMP3D_Window__

typedef struct
{
    int begin,end;

}LMP3D_FPS;

typedef struct
{
    int width,height;
    int fpsm,zpsm;
    int faddress1,faddress2,zaddress;
    int switchBuffer;

}LMP3D_Buffer;


LMP3D_Buffer LMP3D_Window(char *name);
int LMP3D_VBlank();
void LMP3D_FlipBuffer(LMP3D_Buffer *buffer);

#endif


