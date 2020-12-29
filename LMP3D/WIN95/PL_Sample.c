#include <stdio.h>
#include <stdlib.h>

#ifdef WINDOWS95

#include "LMP3D/LMP3D.h"

void LMP3D_Texture_Format_Convert(LMP3D_Texture *texture)
{
	switch (texture->format)
	{
		case LMP3D_FORMAT_LUM:
		break;

		case LMP3D_FORMAT_LUMA:
		break;

		case LMP3D_FORMAT_RGB888:
		break;

		case LMP3D_FORMAT_RGBA8888:
		break;

		case LMP3D_FORMAT_RGB555:

		break;

		case LMP3D_FORMAT_RGBA1555:

		break;

		case LMP3D_FORMAT_8BPP:
		break;

		case LMP3D_FORMAT_4BPP:

		break;

		case LMP3D_FORMAT_2BPP:

		break;

		default:

		break;
	}

}


void LMP3D_Camera_Perspective(LMP3D_Camera camera)
{

}

void LMP3D_Convert_Model(LMP3D_Model *obj)
{

}


void LMP3D_Camera_Ortho2D()
{

}



void LMP3D_VRAM_Set(unsigned int vram)
{

}
unsigned int LMP3D_VRAM_Get()
{

}
void LMP3D_Draw_Text(int x,int y,char *text)
{

}
void LMP3D_VRAM_Info(char *info)
{

}

void LMP3D_File_Path(char *inpath,char *outpath)
{
	strcpy(outpath,inpath);
}

unsigned short LMP3D_Convert_Pixel(unsigned short pixel)
{
	return pixel;
}

void LMP3D_Texture_Convert_Internal(LMP3D_Texture *texture)
{

}

void LMP3D_MatrixMultiply(float* dest,float* src1,float* src2)
{

}
void LMP3D_Draw_Sprite(LMP3D_Texture *texture,Vector2i position,Recti *rectin,int flag)
{

}

void LMP3D_Draw_Sprite_Array(LMP3D_Sprite *sprite,int n)
{

}

//Switch Buffer
void LMP3D_FlipBuffer(LMP3D_Buffer *buffer)
{

}

//Draw model 3D
void LMP3D_Model_Draw(LMP3D_Model *obj)
{

}

static void *LMP3D_Tex;
void LMP3D_SelectTexture_WIN95(LMP3D_Texture *texture)
{
	LMP3D_Tex = texture->pixel;
}

void blit_texture(int px,int py,char tile)
{
	unsigned int *pixels = LMP3D_FrameBuffer_WIN95();
	unsigned int *tpixels = LMP3D_Tex;

	int i = (px>>1) + (py << 7);
	int x,y;

	int tx = tile&0x0F;
	int ty = tile&0xF0;

	int l=(tx<<3)+(ty<<7);

	for(y =0;y < 16;y++)
	{
		for(x =0;x < 8;x++)
		{
			pixels[i] = tpixels[l];
			i++;
			l++;
		}

		i += (128-8);
		l += (128-8);
	}
}

//----
void bitmap_font2(char *texte,int tx,int ty)
{
	int i = 0;
	int x = tx;


	while(texte[i] != 0)
	{
		//printf("%d\n",tx);

		if(texte[i] != ' ' && texte[i] != '\n')
			blit_texture(tx,ty,texte[i]);

		tx += 16;

		if(texte[i] == '\n')
		{
			tx = x;
			ty += 16;
		}

		i++;
	}
}

//RAM to VRAM
void LMP3D_Texture_Upload_VRAM(LMP3D_Texture *texture)
{

}

//malloc VRAM + LMP3D_Texture_Upload_VRAM
void LMP3D_Texture_Upload(LMP3D_Texture *texture)
{

}

//Select Texture
void LMP3D_Texture_Setup(LMP3D_Texture *texture)
{



}

//Delete Texture
void LMP3D_Texture_Delete(LMP3D_Texture *texture)
{

}




#endif

