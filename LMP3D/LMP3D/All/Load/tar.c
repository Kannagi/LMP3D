#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

void LMP3D_Tar(LMP3D_TAR *tar,char *pathtar,char *filename,int option)
{
    int i;
    char tmp[12],name[100];
    FILE *file;

    tar->buffer = NULL;
    tar->size = 0;
    tar->type = 0;
    tar->offset = 0;

    file = fopen(pathtar,"rb");

    if(file == NULL) return;

    while(1)
    {
        fread(name,1,100,file); //Name
        fseek(file,24,SEEK_CUR);
		fread(tmp,1,12,file); //Size
        tar->size = 0;

        for(i = 0;i < 11;i++)
            tar->size += (tmp[10-i] - '0') << (i*3);

        fseek(file,20,SEEK_CUR);

        tar->type = fgetc(file) - '0';
        fseek(file,100 + 255,SEEK_CUR);

        if(option & LMP3D_TAR_DEBUG) printf("%s %d %d %x\n",name,tar->size,tar->type,ftell(file));

		if(strcmp(filename,name) == 0)
        {
        	if(option & LMP3D_TAR_INFO) printf("%s %d %d %x\n",name,tar->size,tar->type,ftell(file));
        	if(option & LMP3D_TAR_BUFFER)
			{
				tar->buffer = malloc(tar->size*sizeof(unsigned char));
				fread(tar->buffer,1,tar->size,file);
			}

			if(option & LMP3D_TAR_OFFSET)
			{
				tar->offset = ftell(file);
			}

            fclose(file);
            return;
        }else
        fseek(file,tar->size,SEEK_CUR);


		i = ftell(file)%0x200;
        if(i != 0)
        {
        	fseek(file,0x200-i,SEEK_CUR);
        }


		while(1)
        {
            i = fgetc(file);
            if(i == -1)
            {
                fclose(file);
                return;
            }

            if(i != 0) break;
        }


        fseek(file,-1,SEEK_CUR);
    }

    fclose(file);
}
