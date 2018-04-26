#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

void LMP3D_Tar(LMP3D_TAR *tar,char *pathtar,char *filename,int option,void *buffer,int size)
{
    int i;
    char tmp[12],name[100];
    void *file;

    tar->buffer = NULL;
    tar->size = 0;
    tar->type = 0;
    tar->offset = 0;

    file = LMP3D_fopen(pathtar,"rb",buffer,size);

    if(file == NULL) return;

    while(1)
    {
        LMP3D_fread(name,1,100,file); //Name
        LMP3D_fseek(file,24,SEEK_CUR);
		LMP3D_fread(tmp,1,12,file); //Size
        tar->size = 0;

        for(i = 0;i < 11;i++)
            tar->size += (tmp[10-i] - '0') << (i*3);

        LMP3D_fseek(file,20,SEEK_CUR);

        tar->type = LMP3D_fgetc(file) - '0';
        LMP3D_fseek(file,100 + 255,SEEK_CUR);

        if(option & LMP3D_TAR_DEBUG) printf("%s %d %d %x\n",name,tar->size,tar->type,LMP3D_ftell(file));

		if(strcmp(filename,name) == 0)
        {
        	if(option & LMP3D_TAR_INFO) printf("%s %d %d %x\n",name,tar->size,tar->type,LMP3D_ftell(file));
        	if(option & LMP3D_TAR_BUFFER)
			{
				tar->buffer = malloc(tar->size*sizeof(unsigned char));
				LMP3D_fread(tar->buffer,1,tar->size,file);
			}

			if(option & LMP3D_TAR_OFFSET)
			{
				tar->offset = LMP3D_ftell(file);
			}

            LMP3D_fclose(file);
            return;
        }else
			LMP3D_fseek(file,tar->size,SEEK_CUR);


		i = LMP3D_ftell(file)&0x1FF;
        if(i != 0)
        {
        	LMP3D_fseek(file,0x200-i,SEEK_CUR);
        }


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


        LMP3D_fseek(file,-1,SEEK_CUR);
    }

    LMP3D_fclose(file);
}
