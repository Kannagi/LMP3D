#ifndef _TAR_
#define _TAR_

typedef struct
{
	unsigned char *buffer;
	int size,type,offset;
}LMP3D_TAR;

void LMP3D_Tar(LMP3D_TAR *tar,char *pathtar,char *filename,int option,void *buffer,int size);

#define LMP3D_TAR_DEBUG		0x01
#define LMP3D_TAR_INFO		0x02
#define LMP3D_TAR_BUFFER	0x04
#define LMP3D_TAR_OFFSET	0x08

#endif


