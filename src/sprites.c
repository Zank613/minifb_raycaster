// sprites.c

#include "../include/raycaster.h"

Sprite sprites[NUM_SPRITES] = {
    {
        .x = 20.5,
        .y = 11.5,
        .texture_id = 11,    // Texture ID for sprite 1
        .distance = 0.0
    },
    {
        .x = 18.5,
        .y = 4.5,
        .texture_id = 15,    // Texture ID for sprite 2
        .distance = 0.0
    }
    // Add more sprites as needed
};

// Function to initialize sprites (if needed)
void init_sprites(void)
{
    // No initialization needed for static sprites
}

// Function to clean up sprites (if needed)
void cleanup_sprites(void)
{
    // No cleanup needed for static sprites
}
