#include "../include/raycaster.h"

void get_move(const uint8_t *key_buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], Player *player)
{
    // Move forward if no wall in front
    if (key_buffer[KB_KEY_W]) 
    {
        double newX = player->x + player->dirX * MOVE_SPEED;
        double newY = player->y + player->dirY * MOVE_SPEED;
        if (worldMap[(int)newY][(int)newX] == 0) 
        {
            player->x = newX;
            player->y = newY;
        }
    }

    // Move backward if no wall is behind
    if (key_buffer[KB_KEY_S])
    {
        double newX = player->x - player->dirX * MOVE_SPEED;
        double newY = player->y - player->dirY * MOVE_SPEED;
        if (worldMap[(int)newY][(int)newX] == 0)
        {
            player->x = newX;
            player->y = newY;
        }
    }

    // Rotate to the right
    if (key_buffer[KB_KEY_D])
    {
        double oldDirX = player->dirX;
        player->dirX = player->dirX * cos(-ROT_SPEED) - player->dirY * sin(-ROT_SPEED);
        player->dirY = oldDirX * sin(-ROT_SPEED) + player->dirY * cos(-ROT_SPEED);
        double oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(-ROT_SPEED) - player->planeY * sin(-ROT_SPEED);
        player->planeY = oldPlaneX * sin(-ROT_SPEED) + player->planeY * cos(-ROT_SPEED);
    }

    // Rotate to the left
    if (key_buffer[KB_KEY_A])
    {
        double oldDirX = player->dirX;
        player->dirX = player->dirX * cos(ROT_SPEED) - player->dirY * sin(ROT_SPEED);
        player->dirY = oldDirX * sin(ROT_SPEED) + player->dirY * cos(ROT_SPEED);
        double oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(ROT_SPEED) - player->planeY * sin(ROT_SPEED);
        player->planeY = oldPlaneX * sin(ROT_SPEED) + player->planeY * cos(ROT_SPEED);
    }
}