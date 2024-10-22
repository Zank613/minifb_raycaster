// Simple Raycaster - Zank613

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <MiniFB.h>

// Define constants
#define WIDTH 800
#define HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 19
#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01

// Define color constants
#define COLOR_RED 0xFF0000
#define COLOR_WHITE 0xFFFFFF

// Define the map (ensure this is properly defined)
int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Player structure
typedef struct {
    double x;
    double y;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} Player;

int main(void) {
    struct mfb_window *window = mfb_open("Raycaster", WIDTH, HEIGHT);
    if (!window) {
        printf("Failed to open window.\n");
        return -1;
    }

    uint32_t buffer[WIDTH * HEIGHT];

    Player player = {
        2.5, // x position
        1.5, // y position
        -1,  // dirX
        0,   // dirY
        0,   // planeX
        0.66 // planeY
    };

    while (mfb_wait_sync(window)) {
        // Clear buffer
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            buffer[i] = 0x000000; // Black background
        }

        // Raycasting loop
        for (int x = 0; x < WIDTH; x++) {
            // Ray position and direction
            double cameraX = 2 * x / (double)WIDTH - 1;
            double rayDirX = player.dirX + player.planeX * cameraX;
            double rayDirY = player.dirY + player.planeY * cameraX;

            // Map position
            int mapX = (int)player.x;
            int mapY = (int)player.y;

            // Length of ray from one x or y-side to next x or y-side
            double deltaDistX = fabs(1 / rayDirX);
            double deltaDistY = fabs(1 / rayDirY);
            double sideDistX, sideDistY;

            // Step and initial sideDist
            int stepX, stepY;

            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (player.x - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - player.x) * deltaDistX;
            }

            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (player.y - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - player.y) * deltaDistY;
            }

            // Perform DDA
            int hit = 0;
            int side;
            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (worldMap[mapY][mapX] > 0) hit = 1;
            }

            // Calculate distance projected on camera direction
            double perpWallDist;
            if (side == 0) {
                perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayDirX;
            } else {
                perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayDirY;
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
            switch(worldMap[mapY][mapX]) {
                case 1: color = COLOR_RED; break;
                default: color = COLOR_WHITE; break;
            }
            if (side == 1) color = color / 2;

            // Draw the pixels of the stripe as a vertical line
            for (int y = drawStart; y < drawEnd; y++) {
                buffer[y * WIDTH + x] = color;
            }
        }

        // Update the window
        if (mfb_update(window, buffer) < 0) {
            break;
        }

        // Handle keyboard input
        const uint8_t *key_buffer = mfb_get_key_buffer(window);

        // Move forward if no wall in front
        if (key_buffer[KB_KEY_W]) {
            double newX = player.x + player.dirX * MOVE_SPEED;
            double newY = player.y + player.dirY * MOVE_SPEED;
            if (worldMap[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        }

        // Move backward if no wall behind
        if (key_buffer[KB_KEY_S]) {
            double newX = player.x - player.dirX * MOVE_SPEED;
            double newY = player.y - player.dirY * MOVE_SPEED;
            if (worldMap[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        }

        // Rotate to the right
        if (key_buffer[KB_KEY_D]) {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(-ROT_SPEED) - player.dirY * sin(-ROT_SPEED);
            player.dirY = oldDirX * sin(-ROT_SPEED) + player.dirY * cos(-ROT_SPEED);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(-ROT_SPEED) - player.planeY * sin(-ROT_SPEED);
            player.planeY = oldPlaneX * sin(-ROT_SPEED) + player.planeY * cos(-ROT_SPEED);
        }

        // Rotate to the left
        if (key_buffer[KB_KEY_A]) {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(ROT_SPEED) - player.dirY * sin(ROT_SPEED);
            player.dirY = oldDirX * sin(ROT_SPEED) + player.dirY * cos(ROT_SPEED);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(ROT_SPEED) - player.planeY * sin(ROT_SPEED);
            player.planeY = oldPlaneX * sin(ROT_SPEED) + player.planeY * cos(ROT_SPEED);
        }
    }

    return 0;
}
