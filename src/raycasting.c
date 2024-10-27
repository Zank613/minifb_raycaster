#include "raycaster.h"

int main(void)
{
    // Initialize the raycaster
    Raycaster raycaster = raycaster_init("Raycaster", WIDTH, HEIGHT);
    if (raycaster.error != RAYCASTER_SUCCESS)
    {
        printf("Error: Raycaster initialization failed.\n");
        return -1;
    }
    printf("Raycaster initialized successfully.\n");

    // Initialize the player
    Player player = {
        .x = 22.0,
        .y = 12.0,
        .dirX = -1.0,
        .dirY = 0.0,
        .planeX = 0.0,
        .planeY = 0.66
    };
    printf("Player initialized at position (%.2f, %.2f).\n", player.x, player.y);

    // Load textures
    int texture_count = 0;
    TextureEntry *textures = load_textures("../config/textures.conf", &texture_count);
    if (!textures)
    {
        printf("Error: Failed to load textures.\n");
        cleanup_raycaster(&raycaster);
        return -1;
    }
    printf("Loaded %d textures:\n", texture_count);
    for (int i = 0; i < texture_count; i++)
    {
        printf("Texture ID %d\n", textures[i].id);
    }

    // Clear buffer to black for initial screen
    memset(raycaster.buffer, 0x00, WIDTH * HEIGHT * sizeof(uint32_t));

    // Initialize sprites
    init_sprites();
    printf("Sprites initialized.\n");

    // Verify worldMap data
    printf("World map data:\n");
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            printf("%d ", worldMap[y][x]);
        }
        printf("\n");
    }

    // Run the raycaster engine loop
    printf("Starting raycaster engine loop...\n");
    run_raycaster(&raycaster, &player, worldMap, textures, texture_count);

    // Cleanup
    printf("Cleaning up...\n");
    cleanup_sprites();
    cleanup_textures(textures, texture_count);
    cleanup_raycaster(&raycaster);

    printf("Raycaster exited successfully.\n");
    return 0;
}
