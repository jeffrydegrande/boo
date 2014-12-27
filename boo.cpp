#define SPRITE_SIZE    32

#include <stdio.h>
#include <string.h>

internal void RenderWeirdGradient(game_offscreen_buffer *buffer, int blueOffset, int greenOffset) {
    uint8 *row = (uint8 *)buffer->memory;

    for (int y =0;  y<buffer->height; ++y) {
        uint32 *pixel = (uint32 *)row;
        for (int x=0; x<buffer->width; ++x) {
            uint8 blue = (x+blueOffset);
            uint8 green = (y+greenOffset);
            *pixel++ = ((green << 8) | blue);
        }
        row += buffer->pitch;
    }
}

void GameUpdateAndRender(game_offscreen_buffer *buffer, int blueOffset, int greenOffset) {
    RenderWeirdGradient(buffer, blueOffset, greenOffset);
}
