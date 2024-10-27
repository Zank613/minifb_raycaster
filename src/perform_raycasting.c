// perform_raycasting.c

#include "../include/raycaster.h"

void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH],
                        int width, int height, TextureEntry *textures, int texture_count)
{
    // Variables for floor and ceiling textures
    int floorTexId = 9;    // Texture ID for floor
    int ceilingTexId = 10; // Texture ID for ceiling
    Texture *floorTexture = NULL;
    Texture *ceilingTexture = NULL;

    // Find the floor and ceiling textures
    for (int i = 0; i < texture_count; i++)
    {
        if (textures[i].id == floorTexId)
            floorTexture = textures[i].texture;
        else if (textures[i].id == ceilingTexId)
            ceilingTexture = textures[i].texture;
    }

    if (!floorTexture || !ceilingTexture)
    {
        printf("Floor or ceiling texture not found.\n");
        // Handle error appropriately, possibly exit the function
        return;
    }

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

        // Get texture ID from worldMap
        int texture_id = worldMap[mapY][mapX];

        // Find the texture with matching ID
        Texture *current_texture = NULL;
        for (int i = 0; i < texture_count; i++)
        {
            if (textures[i].id == texture_id)
            {
                current_texture = textures[i].texture;
                break;
            }
        }

        if (!current_texture)
        {
            // Texture not found, use a default color or skip rendering
            for (int y = drawStart; y < drawEnd; y++)
            {
                buffer[y * width + x] = 0xFF00FF; // Magenta for missing texture
            }
            continue;
        }

        int texWidth = current_texture->width;
        int texHeight = current_texture->height;

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

        // Draw the wall
        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;

            uint32_t color = current_texture->pixels[texHeight * texY + texX];

#if ENABLE_SHADING
            // Apply distance-based shading
            double currentDist = (double)height / (2.0 * y - height);
            double shadeFactor = 1.0 - (currentDist / 10.0); // Adjust 10.0 to control shading distance
            if (shadeFactor < 0.0) shadeFactor = 0.0;

            // Extract color components
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            // Apply shading
            r = (uint8_t)(r * shadeFactor);
            g = (uint8_t)(g * shadeFactor);
            b = (uint8_t)(b * shadeFactor);

            // Repack color
            color = (r << 16) | (g << 8) | b | (color & 0xFF000000);
#endif

            // Make color darker for y-sides (additional shading)
            if (side == 1)
            {
                uint32_t a = color & 0xFF000000;
                color = ((color >> 1) & 0x7F7F7F) | a;
            }

            buffer[y * width + x] = color;
        }

        // Floor casting
        double floorXWall, floorYWall;

        // Determine the position of the floor and ceiling wall hits
        if (side == 0 && rayDirX > 0)
        {
            floorXWall = mapX;
            floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0)
        {
            floorXWall = mapX + 1.0;
            floorYWall = mapY + wallX;
        }
        else if (side == 1 && rayDirY > 0)
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY;
        }
        else
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY + 1.0;
        }

        double distWall, distPlayer;

        distWall = perpWallDist;
        distPlayer = 0.0;

        int yStart = drawEnd + 1;
        int yEnd = height - 1;

        // Loop from the bottom of the wall to the bottom of the screen
        for (int y = yStart; y < height; y++)
        {
            // Current distance from the player to the floor/ceiling
            double currentDist = height / (2.0 * y - height);

            double weight = currentDist / distWall;

            double currentFloorX = weight * floorXWall + (1.0 - weight) * player->x;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * player->y;

            // Floor texture coordinates
            int floorTexX = (int)(currentFloorX * floorTexture->width) % floorTexture->width;
            int floorTexY = (int)(currentFloorY * floorTexture->height) % floorTexture->height;

            // Ceiling texture coordinates
            int ceilingTexX = (int)(currentFloorX * ceilingTexture->width) % ceilingTexture->width;
            int ceilingTexY = (int)(currentFloorY * ceilingTexture->height) % ceilingTexture->height;

            // Fetch the floor and ceiling colors from the textures
            uint32_t floorColor = floorTexture->pixels[floorTexture->width * floorTexY + floorTexX];
            uint32_t ceilingColor = ceilingTexture->pixels[ceilingTexture->width * ceilingTexY + ceilingTexX];

#if ENABLE_SHADING
            // Apply shading based on distance
            double shadeFactor = 1.0 - (currentDist / 10.0); // Adjust 10.0 to control shading distance
            if (shadeFactor < 0.0) shadeFactor = 0.0;

            // Floor shading
            uint8_t fr = (floorColor >> 16) & 0xFF;
            uint8_t fg = (floorColor >> 8) & 0xFF;
            uint8_t fb = floorColor & 0xFF;

            fr = (uint8_t)(fr * shadeFactor);
            fg = (uint8_t)(fg * shadeFactor);
            fb = (uint8_t)(fb * shadeFactor);

            floorColor = (fr << 16) | (fg << 8) | fb | (floorColor & 0xFF000000);

            // Ceiling shading
            uint8_t cr = (ceilingColor >> 16) & 0xFF;
            uint8_t cg = (ceilingColor >> 8) & 0xFF;
            uint8_t cb = ceilingColor & 0xFF;

            cr = (uint8_t)(cr * shadeFactor);
            cg = (uint8_t)(cg * shadeFactor);
            cb = (uint8_t)(cb * shadeFactor);

            ceilingColor = (cr << 16) | (cg << 8) | cb | (ceilingColor & 0xFF000000);
#endif

            // Set the pixel colors in the buffer
            buffer[y * width + x] = floorColor;                         // Floor pixel
            buffer[(height - y) * width + x] = ceilingColor;            // Ceiling pixel
        }
    }
}
