#include "../include/raycaster.h"

Raycaster raycaster_init(const char *name, int width, int height)
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

void run_raycaster(Raycaster *raycaster, Player *player, int worldMap[MAP_HEIGHT][MAP_WIDTH],
                   TextureEntry *textures, int texture_count)
{
    while (1)
    {
        // Update events
        mfb_update_state state = mfb_update_events(raycaster->window);
        if (state != STATE_OK)
        {
            printf("Window closed or error occurred.\n");
            break;
        }

        // Get keyboard input
        const uint8_t *key_buffer = mfb_get_key_buffer(raycaster->window);
        if (key_buffer)
        {
            get_move(key_buffer, worldMap, player);
        }

        // Perform raycasting and render the scene
        perform_raycasting(player, raycaster->buffer, worldMap, WIDTH, HEIGHT, textures, texture_count);

        // Update the window with the buffer
        state = mfb_update(raycaster->window, raycaster->buffer);
        if (state != STATE_OK)
        {
            printf("Window closed or error occurred during update.\n");
            break;
        }
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