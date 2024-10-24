// perform_raycasting.c

#include "../include/raycaster.h"

void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], int width, int height, Texture *textures[])
{
    for (int x = 0; x < width; x++) 
    {
        // Ray position and direction
        double cameraX = 2 * x / (double)width - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;

        // Map position
        int mapX = (int)player->x;
        int mapY = (int)player->y;

        // Length of ray from one x or y-side to next x or y-side
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double sideDistX, sideDistY;

        // Step and initial sideDist
        int stepX, stepY;

        if (rayDirX < 0) 
        {
            stepX = -1;
            sideDistX = (player->x - mapX) * deltaDistX;
        } 
        else 
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
        }

        if (rayDirY < 0) 
        {
            stepY = -1;
            sideDistY = (player->y - mapY) * deltaDistY;
        } 
        else 
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->y) * deltaDistY;
        }

        // Perform DDA
        int hit = 0;
        int side;
        while (hit == 0) 
        {
            // Jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) 
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } 
            else 
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // Check if ray has hit a wall
            if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && worldMap[mapY][mapX] > 0) 
            {
                hit = 1;
            }
        }

        // Calculate distance projected on camera direction
        double perpWallDist;
        if (side == 0) 
        {
            perpWallDist = (mapX - player->x + (1 - stepX) / 2) / rayDirX;
        } 
        else 
        {
            perpWallDist = (mapY - player->y + (1 - stepY) / 2) / rayDirY;
        }

        // Calculate height of line to draw on screen
        int lineHeight = (int)(height / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height) drawEnd = height - 1;

        // Choose wall texture based on map value
        int texNum = worldMap[mapY][mapX] - 1; // Adjust for zero-based indexing
        if (texNum < 0 || texNum >= NUM_TEXTURES)
        {
            texNum = 0; // Default to texture 0 if out of bounds
        }

        // Calculate value of wallX
        double wallX; // Exact position where wall was hit
        if (side == 0)
        {
            wallX = player->y + perpWallDist * rayDirY;
        }
        else
        {
            wallX = player->x + perpWallDist * rayDirX;
        }
        wallX -= floor(wallX);

        // x coordinate on the texture
        int texX = (int)(wallX * (double)texWidth);
        if (texX < 0) texX = 0;
        if (texX >= texWidth) texX = texWidth - 1;

        // Adjust texture coordinate for certain sides
        if (side == 0 && rayDirX > 0)
        {
            texX = texWidth - texX - 1;
        }
        if (side == 1 && rayDirY < 0)
        {
            texX = texWidth - texX - 1;
        }

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;

        // Starting texture coordinate
        double texPos = (drawStart - height / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;

            uint32_t color = textures[texNum]->pixels[texHeight * texY + texX];

            // Make color darker for y-sides
            if (side == 1)
            {
                // Divide color by 2 using bitwise operations
                color = (color >> 1) & 0x7F7F7F;
            }

            buffer[y * width + x] = color;
        }

        // Floor casting can be added here (optional)
    }
}
