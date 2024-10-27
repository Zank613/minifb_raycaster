// raycaster.h

#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "colors.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>                 // for memset
#include <MiniFB.h>

// Define constants
#define WIDTH 800                   // screen width
#define HEIGHT 600                  // screen height
#define MOVE_SPEED 0.05             // Adjusted move speed
#define ROT_SPEED 0.03              // Adjusted rotation speed
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define NUM_SPRITES 3       // Number of sprites in the game
#define MAX_FRAMES 4        // Maximum number of frames per sprite animation

// Enable or disable shading (set to 1 to enable, 0 to disable)
#define ENABLE_SHADING 1

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

// Texture structure assuming each pixel is 4 bytes (RGBA)
typedef struct
{
    int width;
    int height;
    uint32_t *pixels; // Array of 32-bit packed pixel data (RGBA)
} Texture;

// TextureEntry structure to map texture IDs to Texture pointers
typedef struct
{
    int id;          // Texture ID matching the IDs in worldMap
    Texture *texture;
} TextureEntry;

// Sprite structure with animation support
typedef struct
{
    double x;                         // X position in the world
    double y;                         // Y position in the world
    int texture_ids[MAX_FRAMES];      // Array of texture IDs for animation frames
    int num_frames;                   // Number of frames in the animation
    int current_frame;                // Index of the current frame
    double distance;                  // Distance from the player (used for sorting)
    double frame_duration;            // Duration each frame is displayed
    struct mfb_timer *frame_timer;    // Timer for frame duration
} Sprite;

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

extern Sprite sprites[];      // Declare the external sprites array

// initializes the window and framebuffer
Raycaster raycaster_init(const char *name, int width, int height);

// perform raycasting calculations
void perform_raycasting(Player *player, uint32_t *buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH],
                        int width, int height, TextureEntry *textures, int texture_count);

// run the engine loop
void run_raycaster(Raycaster *raycaster, Player *player, int worldMap[MAP_HEIGHT][MAP_WIDTH],
                  TextureEntry *textures, int texture_count);

// moves on the direction depending on the input
void get_move(const uint8_t *key_buffer, int worldMap[MAP_HEIGHT][MAP_WIDTH], Player *player);

// Load textures from configuration
TextureEntry *load_textures(const char *config_filename, int *texture_count);

// Cleanup textures
void cleanup_textures(TextureEntry *textures, int texture_count);

// cleanup everything
void cleanup_raycaster(Raycaster *raycaster);

// Function to initialize sprite timers
void init_sprites(void);

// Function to clean up sprite timers
void cleanup_sprites(void);

#endif // RAYCASTER_H
