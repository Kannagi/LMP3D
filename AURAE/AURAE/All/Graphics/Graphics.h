#ifndef __AURAE_Graphics__
#define __AURAE_Graphics__

void AURAE_Clear(void);
void AURAE_Init(void);

void AURAE_VRAM_Set(unsigned int vram);
unsigned int AURAE_VRAM_Get();

void AURAE_VRAM_Info(char *info);


#endif
