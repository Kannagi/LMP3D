#ifndef __AURAE_File__
#define __AURAE_File__

void *AURAE_fopen(char *path,char *mode,void *buffer,int size);
int AURAE_fclose(void *file);
int AURAE_fseek(void *file,int offset,int whence);
int AURAE_fread(void *ptr,int size,int nmemb,void *file);
int AURAE_ftell(void *file);
int AURAE_fgetc(void *file);
void AURAE_File_Path(char *inpath,char *outpath);

#endif


