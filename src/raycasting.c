#include "raycaster.h"

int main()
{
    Raycaster raycaster = raycaster_init("Raycaster", WIDTH, HEIGHT);
    if (raycaster.error != RAYCASTER_SUCCESS)
    {
        return -1;
    }

    Player player = {
        .x = 22.0,
        .y = 12.0,
        .dirX = -1.0,
        .dirY = 0.0,
        .planeX = 0.0,
        .planeY = 0.66
    };

    int texture_count = 0;
    TextureEntry *textures = load_textures("../config/textures.conf", &texture_count);
    if (!textures)
    {
        printf("Failed to load textures.\n");
        cleanup_raycaster(&raycaster);
        return -1;
    }

    run_raycaster(&raycaster, &player, worldMap, textures, texture_count);

    cleanup_textures(textures, texture_count);
    cleanup_raycaster(&raycaster);

    return 0;
}
