# libpng使用 

#### 介绍

`read_png_rgb.c`用来判断png图像主题颜色，类似win8改变壁纸后，主题自动改变

原理：读取png图像前1%行像素，分别计算r,g,b的像素平均值，结果即为主题颜色

#### 编译

`gcc -l png read_png_rgb.c`

#### 使用

`./a.out png_filepath`

#### Todo

- 整理`read_png_rgb.c`
- 加入图片写入

#### 参考

[Wang93IT博客](https://blog.csdn.net/wang93IT/article/details/85003730)