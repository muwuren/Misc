#include <stdio.h>
#include <stdlib.h>
#include <png.h>

struct {
	int width, height;
	int bit_depth;
	int alpha_flag;
	unsigned char *rgba;
} out;

int main(int argc, char *argv[])
{
	png_structp png_ptr;
	png_infop info_ptr;

	FILE *fp;
	fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("%s can't been opened\n", argv[1]);
		return 1;
	}
	
	printf("launcher[%s] ...\n", PNG_LIBPNG_VER_STRING); //打印当前libpng版本号
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	rewind(fp);

	png_init_io(png_ptr, fp);
	
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	int channels, color_type;
	channels = png_get_channels(png_ptr, info_ptr);
	color_type 	= png_get_color_type(png_ptr, info_ptr);//颜色类型
	out.bit_depth = png_get_bit_depth(png_ptr, info_ptr);//位深度	
	out.width 	 = png_get_image_width(png_ptr, info_ptr);//宽
	out.height  = png_get_image_height(png_ptr, info_ptr);//高

	printf("channels = %d color_type = %d bit_depth = %d width = %d height = %d ...\n",
			channels, color_type, out.bit_depth, out.width, out.height);

	int size;
	int pos = 0;
	png_bytepp row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	size = out.width * out.height;
	if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		out.alpha_flag = 1;
		size *= (sizeof(unsigned char) * 4);
		out.rgba = (png_bytep)malloc(size);
		if (NULL == out.rgba) {
			printf("malloc rgba faile ...\n");
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			return 1;
		}

		long int r_sum, g_sum, b_sum;
		r_sum = g_sum = b_sum = 0;
		for (int i = 0; i < out.height*0.01; ++i) {
			for (int j = 0; j < out.width * 4; j += 4) {
				out.rgba[pos++] = row_pointers[i][j+3];
				out.rgba[pos++] = row_pointers[i][j+2];
				out.rgba[pos++] = row_pointers[i][j+1];
				out.rgba[pos++] = row_pointers[i][j+0];

				r_sum += row_pointers[i][j+0];
				g_sum += row_pointers[i][j+1];
				b_sum += row_pointers[i][j+2];
			}
		}
		int all = out.height*0.01 * out.width;
		printf("r_sum = %ld, g_sum = %ld, b_sum = %ld\n", r_sum, g_sum, b_sum);
		printf("r = %lx, g = %lx, b = %lx\n", r_sum/all, g_sum/all, b_sum/all);
	} else if (channels ==3 || color_type == PNG_COLOR_TYPE_RGB) {
		out.alpha_flag = 0;
		size *= (sizeof(unsigned char) * 3);
		out.rgba = (png_bytep)malloc(size);

		if (NULL == out.rgba) {
			printf("malloc rgba faile ...\n");
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
			return -1;
		}

		for (int i = 0; i < out.height; ++i)
			for (int j = 0; j < out.width*3; j+=3) {
				out.rgba[pos++] = row_pointers[i][j+2];
				out.rgba[pos++] = row_pointers[i][j+1];
				out.rgba[pos++] = row_pointers[i][j+0];
			}

	} else {
		return 1;
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(fp);
	return 0;
}
