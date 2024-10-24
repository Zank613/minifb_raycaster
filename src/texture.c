#include "../include/raycaster.h"

Texture *init_texture(int width, int height)
{
    Texture *tex = (Texture *)malloc(sizeof(Texture));
    tex->width = width;
    tex->height = height;
    tex->pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t)); // 32 bits (4 bytes) per pixel
    return tex;
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