#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LMP3D/LMP3D.h"

LMP3D_TileMap *LMP3D_Load_TileMap(char *filename,int offset,void *buffer,int size)
{
	void *file;
	file = LMP3D_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	LMP3D_fseek(file, offset, SEEK_SET);

	LMP3D_TileMap *tilemap = malloc(sizeof(LMP3D_TileMap));

	unsigned char sizewh[2];

	LMP3D_fread(sizewh,1,2,file);
	tilemap->w = sizewh[0]<<4;
	tilemap->h = sizewh[1]<<4;
	tilemap->position.x = 0;
	tilemap->position.y = 0;

	//printf("size t :%d %d\n",tilemap.w,tilemap.h);

	tilemap->tiles = malloc(tilemap->w*tilemap->h*2);

	LMP3D_fread(tilemap->tiles,2,tilemap->w*tilemap->h,file);

	LMP3D_fclose(file);

	return tilemap;
}



