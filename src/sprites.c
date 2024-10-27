// sprites.c

#include "../include/raycaster.h"

Sprite sprites[NUM_SPRITES] = {
    {
        .x = 20.5,
        .y = 11.5,
        .texture_ids = {11, 12, 13, 14}, // Texture IDs for sprite 1 frames
        .num_frames = 4,
        .current_frame = 0,
        .distance = 0.0,
        .frame_duration = 0.2, // Each frame displayed for 0.2 seconds
        .frame_timer = NULL    // Will initialize later
    },
    {
        .x = 18.5,
        .y = 4.5,
        .texture_ids = {15, 16, 17, 18}, // Texture IDs for sprite 2 frames
        .num_frames = 4,
        .current_frame = 0,
        .distance = 0.0,
        .frame_duration = 0.2,
        .frame_timer = NULL
    },
    // Add more sprites as needed
};

// Function to initialize sprite timers
void init_sprites(void)
{
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        sprites[i].frame_timer = mfb_timer_create();
        if (!sprites[i].frame_timer)
        {
            printf("Failed to create timer for sprite %d\n", i);
        }
    }
}

// Function to clean up sprite timers
void cleanup_sprites(void)
{
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        if (sprites[i].frame_timer)
        {
            mfb_timer_destroy(sprites[i].frame_timer);
            sprites[i].frame_timer = NULL;
        }
    }
}
