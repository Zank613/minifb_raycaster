// texture.c

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/raycaster.h"

Texture *load_texture(const char *filename)
{
    Texture *tex = (Texture *)malloc(sizeof(Texture));
    if (!tex)
    {
        printf("Failed to allocate Texture struct\n");
        return NULL;
    }

    int channels;
    unsigned char *data = stbi_load(filename, &tex->width, &tex->height, &channels, 4);
    if (!data)
    {
        printf("Failed to load texture: %s\n", filename);
        free(tex);
        return NULL;
    }

    tex->pixels = (uint32_t *)malloc(tex->width * tex->height * sizeof(uint32_t));
    if (!tex->pixels)
    {
        printf("Failed to allocate Texture pixels\n");
        stbi_image_free(data);
        free(tex);
        return NULL;
    }

    for (int i = 0; i < tex->width * tex->height; i++)
    {
        unsigned char r = data[i * 4];
        unsigned char g = data[i * 4 + 1];
        unsigned char b = data[i * 4 + 2];
        unsigned char a = data[i * 4 + 3];

        tex->pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }

    stbi_image_free(data);
    return tex;
}

TextureEntry *load_textures(const char *config_filename, int *texture_count)
{
    FILE *file = fopen(config_filename, "r");
    if (!file)
    {
        printf("Failed to open texture configuration file: %s\n", config_filename);
        return NULL;
    }

    // First, count the number of textures
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] != '\n' && line[0] != '#') // Ignore empty lines and comments
            count++;
    }

    rewind(file); // Reset file pointer to the beginning

    TextureEntry *textures = (TextureEntry *)malloc(count * sizeof(TextureEntry));
    if (!textures)
    {
        printf("Failed to allocate textures array.\n");
        fclose(file);
        return NULL;
    }

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < count)
    {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#')
            continue;

        // Remove newline character
        line[strcspn(line, "\r\n")] = 0;

        // Parse ID and filename
        int id;
        char filename[256];
        if (sscanf(line, "%d %255s", &id, filename) != 2)
        {
            printf("Invalid line in texture configuration: %s\n", line);
            continue;
        }

        textures[i].id = id;
        textures[i].texture = load_texture(filename);
        if (!textures[i].texture)
        {
            printf("Failed to load texture %d: %s\n", id, filename);
            // Handle error appropriately, possibly set a default texture
        }
        i++;
    }

    fclose(file);

    *texture_count = i;
    return textures;
}

void cleanup_textures(TextureEntry *textures, int texture_count)
{
    for (int i = 0; i < texture_count; i++)
    {
        if (textures[i].texture)
        {
            if (textures[i].texture->pixels)
            {
                free(textures[i].texture->pixels);
            }
            free(textures[i].texture);
        }
    }
    free(textures);
}

// set_pixel function remains the same
void set_pixel(Texture *tex, int x, int y, uint32_t color)
{
    int index = y * tex->width + x;
    tex->pixels[index] = color;
}
