#include "../include/raycaster.h"

Texture *init_texture(int width, int height)
{
    Texture *tex = (Texture *)malloc(sizeof(Texture));
    if (!tex)
    {
        printf("Failed to allocate Texture struct\n");
        return NULL;
    }
    tex->width = width;
    tex->height = height;
    tex->pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    if (!tex->pixels)
    {
        printf("Failed to allocate Texture pixels\n");
        free(tex);
        return NULL;
    }
    return tex;
}


void generate_textures(Texture *textures[])
{
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        textures[i] = init_texture(texWidth, texHeight);
        if (!textures[i])
        {
            printf("Failed to initialize texture %d\n", i);
            // Handle error
        }
    }

    for (int x = 0; x < texWidth; x++)
    {
        for (int y = 0; y < texHeight; y++)
        {
            int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
            int ycolor = y * 256 / texHeight;
            int xycolor = y * 128 / texHeight + x * 128 / texWidth;

            uint32_t color;
            // Texture 0: Flat red texture with black cross
            color = 0xFF0000; // Red color
            if (x == y || x == texWidth - y)
            {
                color = 0x000000; // Black color
            }
            set_pixel(textures[0], x, y, color);

            // Texture 1: Sloped greyscale
            color = (xycolor << 16) | (xycolor << 8) | xycolor;
            set_pixel(textures[1], x, y, color);

            // Texture 2: Sloped yellow gradient
            color = (xycolor << 8) | (xycolor << 16);
            set_pixel(textures[2], x, y, color);

            // Texture 3: XOR greyscale
            color = (xorcolor << 16) | (xorcolor << 8) | xorcolor;
            set_pixel(textures[3], x, y, color);

            // Texture 4: XOR green
            color = xorcolor << 8;
            set_pixel(textures[4], x, y, color);

            // Texture 5: Red bricks
            color = 0xC00000; // Dark red
            if ((x % 16) && (y % 16))
            {
                color = 0xFF0000; // Bright red
            }
            set_pixel(textures[5], x, y, color);

            // Texture 6: Red gradient
            color = ycolor << 16;
            set_pixel(textures[6], x, y, color);

            // Texture 7: Flat grey texture
            color = (128 << 16) | (128 << 8) | 128;
            set_pixel(textures[7], x, y, color);
        }
    }
}

void set_pixel(Texture *tex, int x, int y, uint32_t color)
{
    int index = y * tex->width + x;
    tex->pixels[index] = color;
}

uint32_t choose_wall_color(int side, int worldMap[MAP_HEIGHT][MAP_WIDTH], int mapX, int mapY, Color *color_selection)
{
    uint32_t color;
    int map_value = worldMap[mapY][mapX];  // Get the map value at position
    color = color_selection[map_value];   // Select the color from the array

    if (side == 1)
    {
        // Reduce brightness for the color on the side (by halving RGB channels)
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >> 8) & 0xFF;
        uint32_t b = color & 0xFF;

        // Reduce the intensity of each channel by half
        r /= 2;
        g /= 2;
        b /= 2;

        // Pack the reduced color back into the 32-bit format
        color = (r << 16) | (g << 8) | b;
    }

    return color;
}

void make_red_gradient(Texture *tex)
{
    for (int y = 0; y < tex->height; ++y)
    {
        for (int x = 0; x < tex->width; ++x)
        {
            uint32_t red_value = (uint32_t)(x * 255 / tex->width);
            uint32_t color = MFB_RGB(red_value, 0, 0); // Red channel gradient
            set_pixel(tex, x, y, color);
        }
    }
}

void make_black_cross(Texture *tex)
{
    for (int y = 0; y < tex->height; ++y)
    {
        for (int x = 0; x < tex->width; ++x)
        {
            if (x == tex->width / 2 || y == tex->height / 2)
            {
                set_pixel(tex, x, y, MFB_RGB(0, 0, 0)); // Black cross
            }
            else
            {
                set_pixel(tex, x, y, MFB_RGB(255, 0, 0)); // Red background
            }
        }
    }
}