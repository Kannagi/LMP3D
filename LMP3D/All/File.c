#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"


typedef struct
{
	int mode,i,endian,size;
	FILE *file;
	void *buffer;

}LMP3D_FILE;


void *LMP3D_fopen(char *path,char *mode,void *buffer,int size)
{
	LMP3D_FILE *internalfile = malloc(sizeof(LMP3D_FILE));

	internalfile->endian = LMP3D_ENDIANNESS;
	internalfile->size = size;

	if(buffer == NULL)
	{
		internalfile->mode = 0;

		char srtfile[100];
		LMP3D_File_Path(path,srtfile);

		internalfile->file = fopen(srtfile,mode);
		if(internalfile->file == NULL)
		{
			free(internalfile);
			return NULL;
		}

	}else
	{
		internalfile->mode = 1;
		internalfile->i = 0;
		internalfile->buffer = buffer;
	}



	return internalfile;
}

int LMP3D_fseek(void *file,int offset,int whence)
{
	LMP3D_FILE *internalfile = file;
	if(internalfile->mode == 0)
	{
		return fseek(internalfile->file, offset, whence);
	}else
	{
		if(whence == SEEK_SET)
			internalfile->i = offset;

		if(whence == SEEK_CUR)
			internalfile->i += offset;

		if(whence == SEEK_END)
			internalfile->i = internalfile->size-offset;

		//printf("seek %d %d	%d %d %d\n",internalfile->i,offset ,SEEK_SET ,SEEK_CUR ,SEEK_END );
	}

	return 0;
}

int LMP3D_ftell(void *file)
{
	LMP3D_FILE *internalfile = file;
	if(internalfile->mode == 0)
	{
		return ftell(internalfile->file);
	}else
	{
		return internalfile->i;
	}

	return 0;
}

int LMP3D_fclose(void *file)
{
	LMP3D_FILE *internalfile = file;
	if(internalfile->mode == 0)
	{
		return fclose(internalfile->file);
	}else
	{

	}

	free(internalfile);


	return 0;
}

int LMP3D_fread(void *ptr,int size,int nmemb,void *file)
{
	LMP3D_FILE *internalfile = file;
	int i,l,j = 0,ii = internalfile->i;
	char *buffer = internalfile->buffer;
	char *cptr = ptr;
	if(internalfile->mode == 0)
	{
		return fread(ptr,size,nmemb,internalfile->file);
	}else
	{
		//if(internalfile->i+nmemb >= internalfile->size) nmemb -= (internalfile->size-internalfile->i);
		for(i = 0;i < nmemb;i++)
		{
			for(l = 0;l < size;l++)
			{
				cptr[j] = buffer[ii];
				ii++;
				j++;
			}

		}
	}
	internalfile->i = ii;

	return 0;
}

int LMP3D_fgetc(void *file)
{
	LMP3D_FILE *internalfile = file;
	int n = 0;
	unsigned char *buffer = internalfile->buffer;

	if(internalfile->mode == 0)
	{
		return fgetc(internalfile->file);
	}else
	{
		if(internalfile->i >= internalfile->size) return -1;
		n = buffer[internalfile->i];
		internalfile->i++;
	}


	return n;
}
