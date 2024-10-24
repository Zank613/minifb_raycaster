#include "../include/raycaster.h"

int main(void) {
    Raycaster raycaster = raycaster_init("Raycaster", WIDTH, HEIGHT);
    if (raycaster.error != RAYCASTER_SUCCESS) {
        printf("Failed to open window or initialize resources.\n");
        return -1;
    }

    Texture *textures[NUM_TEXTURES];
    generate_textures(textures);

    Player player = 
    {
        5.5,  // x position
        18.5, // y position
        -1,   // dirX
        0,    // dirY
        0,    // planeX
        0.66  // planeY
    };

    // Run the game loop
    run_raycaster(&raycaster, &player, worldMap, textures);

    // Cleanup textures
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        if (textures[i])
        {
            free(textures[i]->pixels);
            free(textures[i]);
        }
    }

    // Cleanup
    cleanup_raycaster(&raycaster);

    return 0;
}