#include <png.h>

png_structp create_png_struct()
{
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        return NULL;
    printf("RAM Address for file: %p\n", png);
    return png;
}

png_infop read_png_info(png_structp png, FILE *fp)
{
    png_infop info = png_create_info_struct(png);
    if (!info)
        return NULL;
    if (setjmp(png_jmpbuf(png)))
        return NULL;
    png_init_io(png, fp);
    png_read_info(png, info);
    return info;
}