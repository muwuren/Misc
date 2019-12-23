#include "changetheme.h"

int main(void)
{
	char theme[8];
	int ret;
	
	ret = get_pic_theme("/home/foxer/Pictures/i3wallpapers/now.png", theme);
	if (ret == 1) {
		printf("error\n");
		return 1;
	}
	printf("%s\n", theme);
	return 0;
}

