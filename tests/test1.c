#include "../include/raycaster.h"

int main(void) 
{
    Raycaster raycaster = raycaster_init("Raycaster", WIDTH, HEIGHT);
    if (raycaster.error != RAYCASTER_SUCCESS) 
    {
        printf("Failed to open window or initialize resources.\n");
        return -1;
    }

    Player player = 
    {
        2.5,  // x position
        18.5, // y position
        -1,   // dirX
        0,    // dirY
        0,    // planeX
        0.66  // planeY
    };

    // Run the game loop
    run_raycaster(&raycaster, &player, worldMap);

    // Cleanup
    cleanup_raycaster(&raycaster);

    return 0;
}
