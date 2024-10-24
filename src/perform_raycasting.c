#include "../include/raycaster.h"

void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], int width, int height)
{
    for (int x = 0; x < WIDTH; x++) 
    {
        // Ray position and direction
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;

        // Map position
        int mapX = (int)player->x;
        int mapY = (int)player->y;

        // Length of ray from one x or y-side to next x or y-side
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double sideDistX, sideDistY;

        // Step and initial sideDist
        int stepX, stepY;

        if (rayDirX < 0) 
        {
            stepX = -1;
            sideDistX = (player->x - mapX) * deltaDistX;
        } else 
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
        }

        if (rayDirY < 0) 
        {
            stepY = -1;
            sideDistY = (player->y - mapY) * deltaDistY;
        } else 
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->y) * deltaDistY;
        }

        // Perform DDA
        int hit = 0;
        int side;
        while (hit == 0) 
        {
            if (sideDistX < sideDistY) 
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else 
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (worldMap[mapY][mapX] > 0) hit = 1;
        }

        // Calculate distance projected on camera direction
        double perpWallDist;
        if (side == 0) 
        {
            perpWallDist = (mapX - player->x + (1 - stepX) / 2) / rayDirX;
        } else 
        {
            perpWallDist = (mapY - player->y + (1 - stepY) / 2) / rayDirY;
        }

        // Calculate height of line to draw on screen
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // Choose wall color
        uint32_t color;
        switch(worldMap[mapY][mapX]) 
        {
            case 1: color = COLOR_WHITE; break;
            case 2: color = COLOR_RED; break;
            case 3: color = COLOR_BLUE; break;
            case 4: color = COLOR_PURPLE; break;
            case 5: color = COLOR_CYAN; break;
            case 6: color = COLOR_GREEN; break;
            default: color = COLOR_WHITE; break;
        }
        if (side == 1) color = color / 2;

        // Draw the pixels of the stripe as a vertical line
        for (int y = drawStart; y < drawEnd; y++) 
        {
            buffer[y * WIDTH + x] = color;
        }
    }
}