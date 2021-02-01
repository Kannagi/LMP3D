#ifndef _TAR_
#define _TAR_

typedef struct
{
	unsigned char *buffer;
	int size,type,offset;
}AURAE_TAR;

void AURAE_Tar(AURAE_TAR *tar,char *pathtar,char *filename,int option,void *buffer,int size);

#define AURAE_TAR_DEBUG		0x01
#define AURAE_TAR_INFO		0x02
#define AURAE_TAR_BUFFER	0x04
#define AURAE_TAR_OFFSET	0x08

#endif


