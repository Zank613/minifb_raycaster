# MiniFB Raycaster
![The Raycaster](/assets/raycaster_2.png "Raycaster")

## Overview
MiniFB Raycaster is a simple raycasting engine built using the MiniFB library. It demonstrates basic 3D rendering techniques using 2D raycasting.

## Features
- Basic 3D rendering using raycasting
- Simple and lightweight
- Cross-platform support

## Requirements
- C compiler (GCC, Clang, etc.)
- CMake
- MiniFB library

## Installation
1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/Zank613/minifb_raycaster.git
    ```
2. Navigate to the project directory:
    ```sh
    cd minifb_raycaster
    ```
3. Create a build directory and navigate into it:
    ```sh
    mkdir build && cd build
    ```
4. Generate the build files using CMake:
    ```sh
    cmake ..
    ```
5. Build the project:
    ```sh
    cmake --build .
    ```

## Usage
### Run the executable:
```sh
./minifb_raycaster
```

The default player starts at position (2.5, 18.5) in the map, facing in the negative X direction. The game window will open, and you can use the WASD keys to move the player around.

### Example Code Snippet
To create and run the raycaster in your own code, you can initialize the **Raycaster** and **Player** structs, as shown below:
```C
#include "raycaster.h"

int main(void)
{
    // Initialize the raycaster
    Raycaster raycaster = raycaster_init("Raycaster Example", WIDTH, HEIGHT);
    
    if (raycaster.error != RAYCASTER_SUCCESS)
    {
        printf("Failed to open window or initialize resources.\n");
        return -1;
    }

    // Initialize the player
    Player player = {
        .x = 2.5,      // Player's initial x position
        .y = 18.5,     // Player's initial y position
        .dirX = -1,    // Player's initial direction on X-axis
        .dirY = 0,     // Player's initial direction on Y-axis
        .planeX = 0,   // Camera plane X
        .planeY = 0.66 // Camera plane Y (FOV factor)
    };

    // Start the raycasting loop
    run_raycaster(&raycaster, &player, worldMap);

    // Cleanup
    cleanup_raycaster(&raycaster);

    return 0;
}

```
This initializes a simple game loop where you can move the player within a predefined map using raycasting to render the 3D scene.

### Controls
- Use the WASD keys to move the player:

### Customization
- Modify the player’s starting position and direction in the Player struct to change the initial state.

- You can modify the worldMap to create your own levels or maps by changing the values of the 2D array in the **worldMap[MAP_HEIGHT][MAP_WIDTH]** grid, where 1 represents a wall and 0 represents an empty space.

## Compilers Tested

- GCC (MinGW on Windows): Successfully built and ran without issues.

- MSVC (Microsoft Visual Studio): Segmentation faults encountered during build.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements
- [Minifb](https://github.com/emoon/minifb) for the framebuffer library
- [Lode Vandevenne](https://lodev.org/cgtutor/raycasting.html) for the raycasting tutorial

## Additional Screenshots

![The Raycaster](/assets/raycaster_1.png "Raycaster")
![The Raycaster](/assets/raycaster_3.png "Raycaster")
![The Raycaster](/assets/raycaster_4.png "Raycaster")
![The Raycaster](/assets/raycaster_5.png "Raycaster")