#include "../include/raycaster.h"

Raycaster raycaster_init(char *name, int width, int height)
{
    Raycaster raycaster;
    raycaster.window = mfb_open(name, width, height);
    if(!raycaster.window)
    {
        printf("Failed to open window.\n");
        raycaster.error = RAYCASTER_ERROR_WINDOW;
        return raycaster;
    }

    raycaster.buffer = malloc(width * height * sizeof(uint32_t));
    if (!raycaster.buffer)
    {
        printf("Failed to allocate buffer.\n");
        raycaster.error = RAYCASTER_ERROR_BUFFER;
        return raycaster;
    }

    raycaster.error = RAYCASTER_SUCCESS;
    return raycaster;
}

void run_raycaster(Raycaster *raycaster, Player *player, int worldMap[MAP_HEIGHT][MAP_WIDTH], Color *color_selection) 
{
    while (mfb_wait_sync(raycaster->window)) 
    {
        // Clear buffer
        memset(raycaster->buffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

        // Perform raycasting
        perform_raycasting(player, raycaster->buffer, worldMap, WIDTH, HEIGHT, color_selection);

        // Update the window
        if (mfb_update(raycaster->window, raycaster->buffer) < 0) 
        {
            break; // Break if the window closed or error occurred
        }

        // Handle keyboard input
        const uint8_t *key_buffer = mfb_get_key_buffer(raycaster->window);
        get_move(key_buffer, worldMap, player);
    }
}

void cleanup_raycaster(Raycaster *raycaster) 
{
    if (raycaster->buffer) 
    {
        free(raycaster->buffer);
        raycaster->buffer = NULL;
    }
}