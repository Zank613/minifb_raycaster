# MiniFB Raycaster

![The Raycaster](/assets/raycaster_6.png "Raycaster")

## Overview

MiniFB Raycaster is a simple raycasting engine built using the MiniFB library. It demonstrates basic 3D rendering techniques using 2D raycasting. The engine now supports loading custom textures from external image files specified in a configuration file, allowing for greater flexibility and customization.

## Features

- Basic 3D rendering using raycasting
- **Support for custom textures loaded from image files**
- Simple and lightweight
- Cross-platform support

## Requirements

- C compiler (GCC, Clang, etc.)
- CMake
- [MiniFB](https://github.com/emoon/minifb) library
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) (included in the project)
- Image files for textures (e.g., PNG files)

## Installation

1. **Clone the repository:**

    ```sh
    git clone --recursive https://github.com/Zank613/minifb_raycaster.git
    ```

2. **Navigate to the project directory:**

    ```sh
    cd minifb_raycaster
    ```

3. **Place your texture images in the `assets` directory:**

    - Create an `assets` directory if it doesn't exist:

      ```sh
      mkdir assets
      ```

    - Add your texture image files (e.g., `wall1.png`, `wall2.png`, etc.) to the `assets` directory.

4. **Create or modify the texture configuration file:**

    - Create a file named `textures.conf` in the project's root directory.

    - Specify the texture IDs and corresponding file paths in `textures.conf`. For example:

      ```
      1 assets/wall1.png
      2 assets/wall2.png
      3 assets/wall3.png
      4 assets/wall4.png
      5 assets/wall5.png
      6 assets/wall6.png
      7 assets/wall7.png
      8 assets/wall8.png
      ```

    - Make sure the texture IDs match those used in the `worldMap` in `world_map.c`.

5. **Create a build directory and navigate into it:**

    ```sh
    mkdir build && cd build
    ```

6. **Generate the build files using CMake:**

    ```sh
    cmake ..
    ```

7. **Build the project:**

    ```sh
    cmake --build .
    ```

## Usage

### Run the executable:

```sh
./minifb_raycaster
```

The default player starts at position (22.0, 12.0) in the map, facing in the negative X direction. The game window will open, and you can use the WASD keys to move the player around.

## Controls
- W: Move forward

- S: Move backward

- A: Rotate left

- D: Rotate right

- Escape: Exit the application

## Customization
### 1. Add Texture Images:
 - Place your custom image files (e.g, PNG files) in the assets directory.
### 2. Update configuration file:
 - Edit "textures.conf" to include your new textures.

 - Each line should contain a texture ID and the path to the image file.

 - Example:
    ```sh
    1 assets/brick_wall.png
    2 assets/stone_wall.png
    3 assets/wood_wall.png
    ```
 - Ensure that the texture IDs correspond to the values used in the worldMap.
### Modify the World Map:
 - Edit world_map.c to use the new texture IDs in the worldMap array.

 - Example:
    ```C
    int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    // ... rest of the map ...
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    ```
 - Replace the numbers in the map with the texture IDs corresponding to your textures.

### Modifying the Player's starting position
 - Modify the player's starting position and direction in the Player struct in raycasting.c to change the initial state.
 ```C
 Player player = {
    .x = 22.0,    // Player's initial x position
    .y = 12.0,    // Player's initial y position
    .dirX = -1.0, // Player's initial direction on X-axis
    .dirY = 0.0,  // Player's initial direction on Y-axis
    .planeX = 0.0,
    .planeY = 0.66
 };
 ```

### Creating Your Own Maps
 - You can modify the worldMap to create your own levels or maps by changing the values of the 2D array in the worldMap[MAP_HEIGHT][MAP_WIDTH] grid.

 - Use 0 for empty spaces and texture IDs (e.g., 1, 2, 3, etc.) to represent walls with different textures.

## Compilers Tested
 - **GCC (MinGW on Windows)**: Succesfully run without issues.

 - **MSVC (Microsoft Visual Studio)**: Segmentation faults encountered during build.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/Zank613/minifb_raycaster/blob/master/LICENSE) file for details.

## Acknowledgements
 - [MiniFB](https://github.com/emoon/minifb) for the framebuffer library.
 - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) for image loading.
 - [Lode Vandevenne](https://lodev.org/cgtutor/raycasting.html) for raycasting tutorial.

## Additional Screenshots

![The Raycaster](/assets/raycaster_1.png "Raycaster")
![The Raycaster](/assets/raycaster_3.png "Raycaster")
![The Raycaster](/assets/raycaster_4.png "Raycaster")
![The Raycaster](/assets/raycaster_5.png "Raycaster")