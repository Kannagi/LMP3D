#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

typedef struct
{							  /* byte offset */
	char name[100];			   /*   0 */
	char mode[8];				 /* 100 */
	char uid[8];				  /* 108 */
	char gid[8];				  /* 116 */
	char size[12];				/* 124 */
	char mtime[12];			   /* 136 */
	char chksum[8];			   /* 148 */
	char typeflag;				/* 156 */
	char linkname[100];		   /* 157 */
	char magic[6];				/* 257 */
	char version[2];			  /* 263 */
	char uname[32];			   /* 265 */
	char gname[32];			   /* 297 */
	char devmajor[8];			 /* 329 */
	char devminor[8];			 /* 337 */
	char prefix[155];			 /* 345 */
	char padding[12];			  /* 500-512 */
}Tar_posix_header;

void LMP3D_Tar(LMP3D_TAR *tar,char *pathtar,char *filename,int option,void *buffer,int size)
{
	int i,offset = 0,tmp;
	void *file;

	tar->buffer = NULL;
	tar->size = 0;
	tar->type = 0;
	tar->offset = 0;

	file = LMP3D_fopen(pathtar,"rb",buffer,size);

	if(file == NULL) return;

	Tar_posix_header tarph;

	LMP3D_fseek(file,0,SEEK_END);
	int endfile = LMP3D_ftell(file);
	LMP3D_fseek(file,0,SEEK_SET);



	while(1)
	{
/*
		LMP3D_fread(tarph.name,1,100,file); //Name
		LMP3D_fseek(file,24,SEEK_CUR);
		LMP3D_fread(tarph.size,1,12,file); //Size
		tar->size = 0;

		for(i = 0;i < 11;i++)
			tar->size += (tarph.size[10-i] - '0') << (i*3);

		//LMP3D_fseek(file,20,SEEK_CUR);

		//tar->type = LMP3D_fgetc(file) - '0';
		LMP3D_fseek(file,121 + 255,SEEK_CUR);
*/

		LMP3D_fread(&tarph,1,sizeof(Tar_posix_header),file);
		tar->size = 0;

		for(i = 0;i < 11;i++)
			tar->size += (tarph.size[10-i] - '0') << (i*3);

		tar->type = tarph.typeflag;

		offset += 512;

		if(option & LMP3D_TAR_DEBUG) printf("%s %d %d %x\n",tarph.name,tar->size,tar->type,offset);
		if(strcmp(filename,tarph.name) == 0)
		{
			if(option & LMP3D_TAR_INFO) printf("%s %d %d %x\n",tarph.name,tar->size,tar->type,offset);
			if(option & LMP3D_TAR_BUFFER)
			{
				tar->buffer = malloc(tar->size*sizeof(unsigned char));
				LMP3D_fread(tar->buffer,1,tar->size,file);
			}

			if(option & LMP3D_TAR_OFFSET)
			{
				tar->offset = offset;

			}

			LMP3D_fclose(file);
			return;
		}

		tmp = tar->size;
		offset += tar->size;

		i = offset&0x1FF;
		if(i != 0)
		{
			offset += 0x200-i;
			tmp += 0x200-i;
		}

		if(offset >= endfile)
		{
			LMP3D_fclose(file);
			printf("error tar : %s\n",filename);
			return;
		}

		LMP3D_fseek(file,tmp,SEEK_CUR);


/*
		//while(1)
		{
			i = LMP3D_fgetc(file);
			if(i == -1)
			{
				LMP3D_fclose(file);
				//printf("end\n");
				return;
			}

			//if(i != 0) break;
		}


		LMP3D_fseek(file,-1,SEEK_CUR);*/
	}

	LMP3D_fclose(file);

	printf("not2\n");
}
