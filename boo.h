#ifndef BOO_H
#define BOO_H

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

#define MAX_KEYS 512
#define MAX_AXIS 6

#ifdef __cplusplus 
extern "C" {
#endif

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double  real64;

typedef struct game_offscreen_buffer
{
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
} game_offscreen_buffer;

struct game_rect
{
    int x, y;
    int w, h;
};

#ifdef __cplusplus 
}
#endif

#endif
