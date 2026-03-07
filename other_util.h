#include <string.h>

const char *get_file_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

int is_valid_extention(const char *extension)
{
    const char *valid_extensions[] = {
        "png",
        "jpg",
        "jpeg",
        // "bmp", "gif"
    };
    int total_extentions = sizeof(valid_extensions) / sizeof(valid_extensions[0]);
    for (int i = 0; i < total_extentions; i++)
    {
        if (strcmp(extension, valid_extensions[i]) == 0)
            return 1;
    }
    return 0;
}