#include "../include/raycaster.h"
#include <stdlib.h>  // For qsort
#include <MiniFB.h>  // For MiniFB timing functions

extern Sprite sprites[];     // Declare the external sprites array

// Comparison function for qsort (sort sprites by distance, farthest to nearest)
int compare_sprites(const void *a, const void *b)
{
    const Sprite *spriteA = (const Sprite *)a;
    const Sprite *spriteB = (const Sprite *)b;

    // Compare the distances of two sprites to determine their rendering order.
    // Return 1 if spriteA is farther than spriteB, -1 if closer, and 0 if equal.
    if (spriteA->distance < spriteB->distance)
        return 1;
    else if (spriteA->distance > spriteB->distance)
        return -1;
    else
        return 0;
}

void draw_minimap(uint32_t *buffer, int width, int height, int map[MAP_HEIGHT][MAP_WIDTH], Player *player)
{
    int mapScale = 4; // Scale factor for the mini-map (1 map cell = 4 pixels on screen)
    int mapWidth = MAP_WIDTH * mapScale;
    int mapHeight = MAP_HEIGHT * mapScale;

    int offsetX = 10; // Mini-map offset from the left
    int offsetY = 10; // Mini-map offset from the top

    // Draw the mini-map
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            uint32_t color;
            if (map[y][x] > 0)
                color = 0x333333; // Wall color on mini-map
            else
                color = 0xCCCCCC; // Floor color on mini-map

            // Draw each cell scaled by mapScale
            for (int i = 0; i < mapScale; i++)
            {
                for (int j = 0; j < mapScale; j++)
                {
                    int bufferX = offsetX + x * mapScale + i;
                    int bufferY = offsetY + y * mapScale + j;
                    // Ensure we are within the bounds of the buffer before drawing.
                    if (bufferX >= 0 && bufferX < width && bufferY >= 0 && bufferY < height)
                        buffer[bufferY * width + bufferX] = color;
                }
            }
        }
    }

    // Draw the player's position on the mini-map
    int playerX = offsetX + (int)(player->x * mapScale);
    int playerY = offsetY + (int)(player->y * mapScale);
    // Draw the player as a small red square (3x3 pixels)
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int bufferX = playerX + i;
            int bufferY = playerY + j;
            if (bufferX >= 0 && bufferX < width && bufferY >= 0 && bufferY < height)
                buffer[bufferY * width + bufferX] = 0xFF0000; // Player color (red)
        }
    }

    // Optionally, draw enemy positions on the mini-map
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        int enemyX = offsetX + (int)(sprites[i].x * mapScale);
        int enemyY = offsetY + (int)(sprites[i].y * mapScale);
        // Draw each enemy as a green pixel
        if (enemyX >= 0 && enemyX < width && enemyY >= 0 && enemyY < height)
            buffer[enemyY * width + enemyX] = 0x00FF00; // Enemy color (green)
    }
}

void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH],
                        int width, int height, TextureEntry *textures, int texture_count)
{
    // Verify player position to make sure it's within the bounds of the map
    int playerMapX = (int)player->x;
    int playerMapY = (int)player->y;

    if (playerMapX < 0 || playerMapX >= MAP_WIDTH || playerMapY < 0 || playerMapY >= MAP_HEIGHT)
        return;
    
    if (worldMap[playerMapY][playerMapX] > 0) // Ensure player is not inside a wall
        return;

    // Variables for floor and ceiling textures
    int floorTexId = 9;     // Texture ID for floor
    int ceilingTexId = 10;  // Texture ID for ceiling
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

    if (!floorTexture || !ceilingTexture) // Ensure textures are loaded properly
        return;

    // Allocate memory for the z-buffer to store distances to walls for each screen column
    double *zBuffer = (double *)malloc(width * sizeof(double));
    if (!zBuffer)
        return;

    for (int x = 0; x < width; x++)
    {
        // Calculate the ray position and direction for this column
        double cameraX = 2 * x / (double)width - 1; // Maps x coordinate to camera plane (-1 to 1)
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;

        if (rayDirX == 0) rayDirX = 1e-30; // Avoid division by zero for rayDirX
        if (rayDirY == 0) rayDirY = 1e-30; // Avoid division by zero for rayDirY

        // Determine the initial map position of the ray
        int mapX = (int)player->x;
        int mapY = (int)player->y;

        // Calculate the length of the ray from one x-side to the next x-side
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double sideDistX, sideDistY;

        // Determine the step direction and initial side distance
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

        // Perform the Digital Differential Analysis (DDA) to detect wall collisions
        int hit = 0; // Indicates if a wall was hit
        int side = 0; // 0 for x-side, 1 for y-side
        int dda_iterations = 0;
        int max_dda_iterations = 1000; // Prevent infinite loops by setting a maximum limit
        while (hit == 0)
        {
            dda_iterations++;
            if (dda_iterations > max_dda_iterations)
                break; // Exit if we exceed the iteration limit to avoid infinite loops

            // Move to the next map square in x or y direction
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

            // Check if the ray has hit a wall or gone out of bounds
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
            {
                hit = 1; // If out of bounds, consider as a hit to stop further calculations
                break;
            }

            if (worldMap[mapY][mapX] > 0) // Wall detected
                hit = 1;
        }

        // Calculate distance to the point of collision
        double perpWallDist;
        if (side == 0) // Horizontal wall hit
            perpWallDist = (mapX - player->x + (1 - stepX) / 2) / rayDirX;
        else // Vertical wall hit
            perpWallDist = (mapY - player->y + (1 - stepY) / 2) / rayDirY;

        // Store distance in zBuffer for later sprite rendering
        zBuffer[x] = perpWallDist;

        // Calculate height of the line to draw on the screen based on distance
        int lineHeight = (int)(height / perpWallDist);
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height) drawEnd = height - 1;

        // Retrieve the texture for the current wall hit
        int texture_id = worldMap[mapY][mapX];
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
            // If no texture found, default to drawing a magenta color to indicate an error
            for (int y = drawStart; y < drawEnd; y++)
                buffer[y * width + x] = 0xFF00FF;
            continue;
        }

        int texWidth = current_texture->width;
        int texHeight = current_texture->height;

        // Calculate the x-coordinate on the texture
        double wallX;
        if (side == 0)
            wallX = player->y + perpWallDist * rayDirY;
        else
            wallX = player->x + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)texWidth);
        if (texX < 0) texX = 0;
        if (texX >= texWidth) texX = texWidth - 1;

        // Reverse the x texture coordinate for certain directions to create proper perspective
        if (side == 0 && rayDirX > 0)
            texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = texWidth - texX - 1;

        // Calculate texture step and initial texture position for vertical drawing
        double step = 1.0 * texHeight / lineHeight;
        double texPos = (drawStart - height / 2 + lineHeight / 2) * step;

        // Draw the pixels for the current column from drawStart to drawEnd
        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;

            // Retrieve the color from the texture
            uint32_t color = current_texture->pixels[texHeight * texY + texX];

            // Darken the color if the wall hit is vertical to simulate shadow
            if (side == 1)
            {
                uint32_t a = color & 0xFF000000;
                color = ((color >> 1) & 0x7F7F7F) | a;
            }

            // Write the pixel to the buffer
            buffer[y * width + x] = color;
        }

        // Calculate floor and ceiling wall coordinates for rendering
        double floorXWall, floorYWall;
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

        // Render floor and ceiling from the drawEnd point to the bottom of the screen
        double distWall = perpWallDist;

        for (int y = drawEnd + 1; y < height; y++)
        {
            double currentDist = height / (2.0 * y - height);
            double weight = currentDist / distWall;

            double currentFloorX = weight * floorXWall + (1.0 - weight) * player->x;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * player->y;

            // Get texture coordinates for floor and ceiling
            int floorTexX = (int)(currentFloorX * floorTexture->width) % floorTexture->width;
            int floorTexY = (int)(currentFloorY * floorTexture->height) % floorTexture->height;

            int ceilingTexX = (int)(currentFloorX * ceilingTexture->width) % ceilingTexture->width;
            int ceilingTexY = (int)(currentFloorY * ceilingTexture->height) % ceilingTexture->height;

            // Retrieve the floor and ceiling colors
            uint32_t floorColor = floorTexture->pixels[floorTexture->width * floorTexY + floorTexX];
            uint32_t ceilingColor = ceilingTexture->pixels[ceilingTexture->width * ceilingTexY + ceilingTexX];

            // Draw floor and ceiling
            buffer[y * width + x] = floorColor;
            buffer[(height - y) * width + x] = ceilingColor;
        }
    }

    // Calculate distances of each sprite from the player for sorting
    for (int i = 0; i < NUM_SPRITES; i++)
        sprites[i].distance = ((player->x - sprites[i].x) * (player->x - sprites[i].x) +
                               (player->y - sprites[i].y) * (player->y - sprites[i].y));

    // Sort sprites by distance from the player
    qsort(sprites, NUM_SPRITES, sizeof(Sprite), compare_sprites);

    // Render each sprite
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        // Calculate position relative to player
        double spriteX = sprites[i].x - player->x;
        double spriteY = sprites[i].y - player->y;

        // Inverse camera matrix to translate sprite position relative to the camera
        double invDet = 1.0 / (player->planeX * player->dirY - player->dirX * player->planeY);

        double transformX = invDet * (player->dirY * spriteX - player->dirX * spriteY);
        double transformY = invDet * (-player->planeY * spriteX + player->planeX * spriteY);

        if (transformY <= 0) // Skip sprites behind the player
            continue;

        // Calculate screen position of the sprite
        int spriteScreenX = (int)((width / 2) * (1 + transformX / transformY));
        int spriteHeight = abs((int)(height / transformY)); // Calculate height of the sprite
        int drawStartY = -spriteHeight / 2 + height / 2;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + height / 2;
        if (drawEndY >= height) drawEndY = height - 1;

        // Calculate width of the sprite
        int spriteWidth = abs((int)(height / transformY));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= width) drawEndX = width - 1;

        // Occlusion culling: skip the sprite if it's farther than the closest wall
        int visible = 0;
        for (int x = drawStartX; x < drawEndX; x++)
        {
            if (x >= 0 && x < width && transformY < zBuffer[x])
            {
                visible = 1;
                break;
            }
        }
        if (!visible) continue;

        // Retrieve sprite texture
        int texture_id = sprites[i].texture_id;
        Texture *spriteTexture = NULL;
        for (int t = 0; t < texture_count; t++)
        {
            if (textures[t].id == texture_id)
            {
                spriteTexture = textures[t].texture;
                remove_background(spriteTexture, BACKGROUND_COLOR); // Remove any background color if necessary
                break;
            }
        }

        if (!spriteTexture) // Skip if no texture found
            continue;

        int texWidth = spriteTexture->width;
        int texHeight = spriteTexture->height;

        // Render the sprite column by column
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;

            if (stripe >= 0 && stripe < width && transformY < zBuffer[stripe])
            {
                for (int y = drawStartY; y < drawEndY; y++)
                {
                    int d = (y) * 256 - height * 128 + spriteHeight * 128;
                    int texY = ((d * texHeight) / spriteHeight) / 256;

                    uint32_t color = spriteTexture->pixels[texHeight * texY + texX];
                    uint8_t alpha = (color >> 24) & 0xFF; // Extract alpha channel for transparency

                    if (alpha > 0) // Only render if not fully transparent
                    {
                        if (alpha == 255) // Fully opaque
                        {
                            buffer[y * width + stripe] = color;
                        }
                        else // Blend pixel with background
                        {
                            // Get background color from the buffer
                            uint32_t bgColor = buffer[y * width + stripe];

                            // Separate RGBA components
                            uint8_t bgR = (bgColor >> 16) & 0xFF;
                            uint8_t bgG = (bgColor >> 8) & 0xFF;
                            uint8_t bgB = bgColor & 0xFF;

                            uint8_t srcR = (color >> 16) & 0xFF;
                            uint8_t srcG = (color >> 8) & 0xFF;
                            uint8_t srcB = color & 0xFF;

                            // Perform alpha blending
                            uint8_t outR = (alpha * srcR + (255 - alpha) * bgR) / 255;
                            uint8_t outG = (alpha * srcG + (255 - alpha) * bgG) / 255;
                            uint8_t outB = (alpha * srcB + (255 - alpha) * bgB) / 255;

                            // Recombine components back into ARGB
                            buffer[y * width + stripe] = (255 << 24) | (outR << 16) | (outG << 8) | outB;
                        }
                    }
                }
            }
        }
    }

    // Free the z-buffer memory after use
    free(zBuffer);

    // Draw the mini-map after all the raycasting is done for visualization purposes
    draw_minimap(buffer, width, height, worldMap, player);
}
