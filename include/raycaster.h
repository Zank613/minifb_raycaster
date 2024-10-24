#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "colors.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <MiniFB.h>

// Define constants
#define WIDTH 800
#define HEIGHT 600
#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

extern int worldMap[MAP_HEIGHT][MAP_WIDTH];

typedef enum
{
    RAYCASTER_SUCCESS = 0,     // No error
    RAYCASTER_ERROR_WINDOW,    // Window creation failed
    RAYCASTER_ERROR_BUFFER,    // Framebuffer allocation failed
    RAYCASTER_ERROR_UNKNOWN    // Unknown error (catch-all)
} RAYCASTER_ERROR;

typedef struct
{
    struct mfb_window *window;
    uint32_t *buffer;
    RAYCASTER_ERROR error;
} Raycaster;

// Player structure
typedef struct
{
    double x;
    double y;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} Player;

// initializes the window and framebuffer
Raycaster raycaster_init(char *name, int width, int height);

// perform raycasting calculations
void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], int width, int height);

// run the raycasting engine loop
void run_raycaster(Raycaster *raycaster, Player *player, int worldMap[MAP_HEIGHT][MAP_WIDTH]);

// moves on the direction depending on the input
void get_move(const uint8_t *key_buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], Player *player);

// cleanup everything
void cleanup_raycaster(Raycaster *raycaster);

#endif // RAYCASTER_H