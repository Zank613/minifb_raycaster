#ifndef COLORS_H
#define COLORS_H

#define BACKGROUND_COLOR 0xFF63747D  // 0xFF (alpha) + 63747D (hex color)

// Enumeration for colors using RGB values packed into a 32-bit integer
typedef enum 
{
    COLOR_BLACK   = 0x000000, // Black
    COLOR_WHITE   = 0xFFFFFF, // White
    COLOR_RED     = 0xFF0000, // Red
    COLOR_GREEN   = 0x00FF00, // Green
    COLOR_BLUE    = 0x0000FF, // Blue
    COLOR_YELLOW  = 0xFFFF00, // Yellow
    COLOR_CYAN    = 0x00FFFF, // Cyan
    COLOR_MAGENTA = 0xFF00FF, // Magenta
    COLOR_PURPLE  = 0x800080, // Purple
    COLOR_ORANGE  = 0xFFA500, // Orange
    COLOR_BROWN   = 0xA52A2A, // Brown
    COLOR_GRAY    = 0x808080, // Gray
    COLOR_PINK    = 0xFFC0CB, // Pink
    COLOR_LIME    = 0x00FF00, // Lime
    COLOR_MAROON  = 0x800000, // Maroon
    COLOR_OLIVE   = 0x808000, // Olive
    COLOR_NAVY    = 0x000080, // Navy
    COLOR_TEAL    = 0x008080, // Teal
    COLOR_SILVER  = 0xC0C0C0  // Silver
} Color;

#endif // COLORS_H