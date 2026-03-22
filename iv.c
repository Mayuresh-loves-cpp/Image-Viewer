// importing standard libraries
#include <stdio.h>
#include <stdlib.h>

// SDL imports
#include <SDL2/SDL.h>

// image library imports
#include <png.h>
#include <jpeglib.h>

// custom utility imports
#include "png_util.h"
#include "other_util.h"

/********************* Global Variables *********************/
png_bytep *row_pointers = NULL; // global row pointers variable
int width, height;              // global width and height variables

void fillSurfaceWithImageRowPointers(SDL_Surface *surface, png_bytep *row_pointers, int current_width, int current_height)
{
    printf("Filling surface with image data...\n");

    SDL_LockSurface(surface);

    // Calculate current surface pitch (number of pixels per row) and get the pixel data pointer
    int pitch = surface->pitch / 4;
    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (int y = 0; y < current_height; y++)
    {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < current_width; x++)
        {
            png_bytep px = &(row[x * 4]);
            int r = px[0];
            int g = px[1];
            int b = px[2];
            int a = px[3];

            Uint32 color = SDL_MapRGBA(surface->format, r, g, b, a);
            // pixel.x = x;
            // pixel.y = y;
            // SDL_FillRect(surface, &pixel, color); // Fill red

            pixels[y * pitch + x] = color;
        }
    }

    SDL_UnlockSurface(surface);
}

// window resizing event watcher function
static int resizingEventWatcher(void *data, SDL_Event *event)
{
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
        if (window == (SDL_Window *)data)
        {
            // int new_width, new_height;
            // SDL_GetWindowSize(window, &new_width, &new_height);
            int new_width = event->window.data1;
            int new_height = event->window.data2;
            if (new_width > width || new_height > height)
            {
                printf("New dimensions exceed original image dimensions. Ignoring resize event.\n");
                return 0;
            }
            else
            {
                fillSurfaceWithImageRowPointers(SDL_GetWindowSurface(window), row_pointers, new_width, new_height);
                SDL_UpdateWindowSurface(window);
            }
            printf("width: %d, height: %d\n", new_width, new_height);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // printf("Hello, World!\n");

    if (argc != 2)
    {
        printf("Missing input image file\n");
        printf("Usage: %s <image file path>\n", argv[0]);
        return 1;
    }

    const char *extension = get_file_extension(argv[1]);
    if (!is_valid_extention(extension))
    {
        printf("Invalid file extension: %s\n", extension);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp)
    {
        printf("Failed to open file\n");
        return 1;
    }

    png_structp png = NULL;
    png_infop info = NULL;

    // png_bytep *row_pointers = NULL;
    // int width = 0, height = 0;

    if (strcmp(extension, "png") == 0)
    {
        png = create_png_struct();
        if (png == NULL)
        {
            printf("Failed to create image structure\n");
            return 1;
        }
        info = read_png_info(png, fp);
        if (info == NULL)
        {
            png_destroy_read_struct(&png, &info, NULL);
            return 1;
        }

        width = png_get_image_width(png, info);
        height = png_get_image_height(png, info);

        png_byte color_type = png_get_color_type(png, info);
        png_byte bit_depth = png_get_bit_depth(png, info);

        // Normalizing format
        if (bit_depth == 16)
        {
            png_set_strip_16(png);
        }

        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png);
        }

        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            png_set_expand_gray_1_2_4_to_8(png);
        }

        if (png_get_valid(png, info, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png);
        }

        if (
            color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

        if (
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png);
        }

        png_read_update_info(png, info);

        // populating row pointers with image data
        row_pointers = malloc(sizeof(png_bytep) * height);

        for (int y = 0; y < height; y++)
            row_pointers[y] = malloc(png_get_rowbytes(png, info));

        png_read_image(png, row_pointers);
    }
    else if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0)
    {
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, fp);
        jpeg_read_header(&cinfo, TRUE);

        cinfo.out_color_space = JCS_RGB;

        jpeg_start_decompress(&cinfo);
        // printf("RAM Address for file: %p\n", &cinfo);

        width = cinfo.output_width;
        height = cinfo.output_height;

        int channels = cinfo.output_components;
        int row_stride = width * channels;

        // populating row pointers with image data
        row_pointers = malloc(sizeof(png_bytep) * height);

        for (int y = 0; y < height; y++)
            row_pointers[y] = malloc(width * 4); // RGBA

        JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)(
            (j_common_ptr)&cinfo,
            JPOOL_IMAGE,
            row_stride,
            1);

        int y = 0;

        while (cinfo.output_scanline < (JDIMENSION)height)
        {
            jpeg_read_scanlines(&cinfo, buffer, 1);
            for (int x = 0; x < width; x++)
            {
                unsigned char r = buffer[0][x * 3];
                unsigned char g = buffer[0][x * 3 + 1];
                unsigned char b = buffer[0][x * 3 + 2];

                row_pointers[y][x * 4] = r;
                row_pointers[y][x * 4 + 1] = g;
                row_pointers[y][x * 4 + 2] = b;
                row_pointers[y][x * 4 + 3] = 255;
            }
            y++;
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *pwindow = SDL_CreateWindow(
        "Image Viewer",                                 // window title
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // initial position
        width, height,                                  // width and height
        SDL_WINDOW_RESIZABLE                            // flags

    );
    // SDL_SetWindowResizable(pwindow, SDL_TRUE);

    // setting maximum and minimum window size
    SDL_SetWindowMaximumSize(pwindow, width, height);
    SDL_SetWindowMinimumSize(pwindow, 300, 300);

    // event watcher for window resizing
    SDL_AddEventWatch(resizingEventWatcher, pwindow);

    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);
    psurface = SDL_GetWindowSurface(pwindow);

    // filling surface with initial image data
    fillSurfaceWithImageRowPointers(psurface, row_pointers, width, height);

    SDL_UpdateWindowSurface(pwindow);
    // SDL_Delay(10000);
    while (1)
    {
        SDL_Event pevent;
        if (SDL_WaitEvent(&pevent)) // blocks until event arrives
        {
            if (pevent.type == SDL_QUIT)
            {
                break;
            }
        }
    }
    // free(row_pointers);
    for (int y = 0; y < height; y++)
    {
        free(row_pointers[y]);
    }

    free(row_pointers);
    SDL_DestroyWindow(pwindow);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return 0;
}
