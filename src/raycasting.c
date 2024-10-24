#include "../include/raycaster.h"

int main(void) {
    Raycaster raycaster = raycaster_init("Raycaster", WIDTH, HEIGHT);
    if (raycaster.error != RAYCASTER_SUCCESS) {
        printf("Failed to open window or initialize resources.\n");
        return -1;
    }

    Color color_selection[] = 
    {
        COLOR_WHITE,  // 0 or invalid case
        COLOR_WHITE,  // 1
        COLOR_RED,    // 2
        COLOR_BLUE,   // 3
        COLOR_PURPLE, // 4
        COLOR_CYAN,   // 5
        COLOR_GREEN   // 6
    };

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
    run_raycaster(&raycaster, &player, worldMap, color_selection);

    // Cleanup
    cleanup_raycaster(&raycaster);

    return 0;
}