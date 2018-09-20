#ifndef __LMP3D_File__
#define __LMP3D_File__

void *LMP3D_fopen(char *path,char *mode,void *buffer,int size);
int LMP3D_fclose(void *file);
int LMP3D_fseek(void *file,int offset,int whence);
int LMP3D_fread(void *ptr,int size,int nmemb,void *file);
int LMP3D_ftell(void *file);
int LMP3D_fgetc(void *file);
void LMP3D_File_Path(char *inpath,char *outpath);

#endif


