#include <stdio.h>
#include <libpng16/png.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	png_structp png_ptr;
	png_infop info_ptr;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	FILE *fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("open file error\n");
		exit(1);
	}

	rewind(fp);
	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);

	struct {
		int width, height;
		int bit_depth;
		int alpha_flag;
		unsigned char *rgba;
	} out;
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	int channels, color_type;
	channels = png_get_channels(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	out.width = png_get_image_width(png_ptr, info_ptr);
	out.height = png_get_image_height(png_ptr, info_ptr);
	out.bit_depth = png_get_bit_depth(png_ptr, info_ptr);


	printf("channels = %d color_type = %d bit_depth = %d width = %d height = %d ...\n",
			channels, color_type, out.bit_depth, out.width, out.height);

	int i, j, k;
	int size, pos = 0;
	int temp;

	png_bytepp row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	size = out.width * out.height;
	if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		out.alpha_flag = 1;
		size *= (sizeof(unsigned char) * 4);
		out.rgba = (png_bytep)malloc(size);

		if (NULL == out.rgba) {
			printf("malloc rgba failed ..\n");
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			exit(1);
		}

		temp = channels;
		for (i = 0; i < out.height; ++i) {
			for (j = 0; j < out.width * 4; j += 4) {
					for (k = channels-1; k >= 0; --k) {
						out.rgba[pos++] = row_pointers[i][j+k];
					}
			}
		}

	} else if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
		out.alpha_flag = 0;
		size *= (sizeof(unsigned char) * 3);
		out.rgba = (png_bytep)malloc(size);
		if (NULL == out.rgba) {
			printf("malloc rbga failed\n");
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			exit(1);
		}

		temp = (3 * out.width);
		for (i = 0; i < out.height; ++i) {
			for (j = 0; j < temp; j+=3) {
				out.rgba[pos++] = row_pointers[i][j+2];
				out.rgba[pos++] = row_pointers[i][j+1];
				out.rgba[pos++] = row_pointers[i][j+0];
			}
			
		}
	} else {
		exit(1);
	}

	//if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD)) {
	//	printf("没有背景色\n");
	//} else {
	//	png_color_16p p_bg;
	//	png_get_bKGD(png_ptr, info_ptr, &p_bg);
	//}






	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	return 0;
}
