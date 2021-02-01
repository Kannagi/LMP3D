#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AURAE/AURAE.h"

AURAE_TileMap *AURAE_Load_TileMap(char *filename,int offset,void *buffer,int size)
{
	void *file;
	file = AURAE_fopen(filename,"rb",buffer,offset+size);

	if(file == NULL) return NULL;

	AURAE_fseek(file, offset, SEEK_SET);

	AURAE_TileMap *tilemap = malloc(sizeof(AURAE_TileMap));

	unsigned char sizewh[2];

	AURAE_fread(sizewh,1,2,file);
	tilemap->w = sizewh[0]<<4;
	tilemap->h = sizewh[1]<<4;
	tilemap->position.x = 0;
	tilemap->position.y = 0;

	//printf("size t :%d %d\n",tilemap.w,tilemap.h);

	tilemap->tiles = malloc(tilemap->w*tilemap->h*2);

	AURAE_fread(tilemap->tiles,2,tilemap->w*tilemap->h,file);

	AURAE_fclose(file);

	return tilemap;
}



