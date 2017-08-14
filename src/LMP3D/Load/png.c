#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __MINGW32__
#include <libpng10/png.h>
#endif

#ifndef __MINGW32__
#include <png.h>
#endif

#include "LMP3D/LMP3D.h"


LMP3D_Texture *LMP3D_Load_png(const char *adresse)
{
	FILE *file = NULL;
    LMP3D_Texture *texture = NULL;
    int bit_depth, color_type,magic_ok,i;
    png_byte magic[8];
    png_bytep *row_pointers = NULL;

	file = fopen(adresse,"rb");
	if(file == NULL) return NULL;

	fread (magic, 1, sizeof (magic), file);

	#ifdef __MINGW32__
	magic_ok = png_check_sig (magic, sizeof (magic));
	#endif

	#ifndef __MINGW32__
	magic_ok = png_sig_cmp (magic,0, sizeof (magic));
	#endif

	if(magic_ok != 0)
	{
		fclose (file);
		return NULL;
	}

	texture = malloc(sizeof(LMP3D_Texture));
    texture->pixel = NULL;
    texture->palette = NULL;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct (png_ptr);

	png_init_io (png_ptr, file);

	png_set_sig_bytes (png_ptr, sizeof (magic));

	png_read_info (png_ptr, info_ptr);

	bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	color_type = png_get_color_type (png_ptr, info_ptr);


	if (bit_depth == 16)
		png_set_strip_16 (png_ptr);
	else if (bit_depth < 8)
		png_set_packing (png_ptr);

	png_read_update_info (png_ptr, info_ptr);

	png_get_IHDR (png_ptr, info_ptr,(png_uint_32*)(&texture->w),(png_uint_32*)(&texture->h),&bit_depth, &color_type,NULL, NULL, NULL);

	switch (color_type)
	{
		case PNG_COLOR_TYPE_GRAY:
		  texture->format = 6409;
		  texture->type = 1;
		break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
		  texture->format = 6410;
		  texture->type = 2;
		break;

		case PNG_COLOR_TYPE_RGB:
		  texture->format = 6407;
		  texture->type = 3;
		break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
		  texture->format = 6408;
		  texture->type = 4;
		break;

		default:
		break;
	}

	texture->bits = texture->type*8;

	texture->taille = texture->w*texture->h * texture->type;

	texture->pixel = malloc(texture->w*texture->h * 4);

	row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texture->h);

	for (i = 0; i < texture->h; ++i)
	{
		row_pointers[i] = (png_bytep)(texture->pixel +
		((texture->h - (i + 1)) * texture->w * texture->type));
	}



	/* read pixel data using row pointers */
	png_read_image (png_ptr, row_pointers);

	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	free(row_pointers);

	fclose (file);




    return texture;

}
