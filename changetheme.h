#ifndef _CHANGETHEME_
#define _CHANGETHEME_


#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <strings.h>

int get_pic_theme(const char *filepath, char *theme)
{
	png_structp png_ptr;
	png_infop info_ptr;

	FILE *fp = fopen(filepath, "rb");
	if (!fp) {
		return 1;
	}
	char checkheader[8];
	if (fread(checkheader, 1, 8, fp) != 8) { //长度过长，直接退出
		fclose(fp);
		return 1;
	} else if (png_check_sig(checkheader, 8) == 0) {
		fclose(fp);
		return 1;
	}
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	rewind(fp);

	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	int channels, color_type;
	int width, height;
	unsigned char * rgba;

	channels = png_get_channels(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	width = png_get_image_width(png_ptr, info_ptr);

	int size;
	int pos = 0;
	png_bytepp row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	size = height * width;


	if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		rgba = (png_bytep)malloc(size*sizeof(unsigned char)*4);
		if (NULL == rgba) {
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			return 1;
		}
		
		long int r_sum, g_sum, b_sum;
		r_sum = g_sum = b_sum = 0;
		for (int i = 0; i < height*0.01; ++i) {
			for (int j = 0; j < width * 4; j += 4) {
				rgba[pos++] = row_pointers[i][j+3];
				rgba[pos++] = row_pointers[i][j+2];
				rgba[pos++] = row_pointers[i][j+1];
				rgba[pos++] = row_pointers[i][j+0];

				r_sum += row_pointers[i][j+0];
				g_sum += row_pointers[i][j+1];
				b_sum += row_pointers[i][j+2];
			}
		}
		free(rgba);
		int all = height*0.01 * width;
		printf("%ld %ld %ld\n", r_sum, g_sum, b_sum);
		printf("#%lx%lx%lx", r_sum/all, g_sum/all, b_sum/all);
		sprintf(theme, "#%lx%lx%lx", r_sum/all, g_sum/all, b_sum/all);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return 0;
	} else if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
		rgba = (png_bytep)malloc(size*sizeof(unsigned char)*3);

		if (NULL == rgba) {
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			return 1;
		}

		long int r_sum, g_sum, b_sum;
		r_sum = g_sum = b_sum = 0;

		for (int i = 0; i < height*0.01; ++i)
			for (int j = 0; j < width*3; j+=3) {
				rgba[pos++] = row_pointers[i][j+2];
				rgba[pos++] = row_pointers[i][j+1];
				rgba[pos++] = row_pointers[i][j+0];

				r_sum += row_pointers[i][j+0];
				g_sum += row_pointers[i][j+1];
				b_sum += row_pointers[i][j+2];
			}
		free(rgba);
		int all = height*0.01 * width;
		printf("%ld %ld %ld\n", r_sum, g_sum, b_sum);

		sprintf(theme, "#%lx%lx%lx", r_sum/all, g_sum/all, b_sum/all);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return 0;
	} else {
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return 1;
	}
}
#endif
