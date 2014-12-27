#include <stdint.h>

#define internal static
#define global_variable static
#define local_persist static

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


#include "boo.h"
void loadTextureFromFile(const char *filename, game_offscreen_buffer *buffer);
#include "boo.cpp"

#include <stdio.h>
#include <sys/mman.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

struct sdl_offscreen_buffer {
    SDL_Texture *texture;
    void *memory;
    int width;
    int height;
    int pitch;
};

global_variable int StartTime;
global_variable sdl_offscreen_buffer GlobalBackbuffer;
global_variable SDL_Window *Window;
global_variable SDL_Renderer *Renderer;

internal void SDLResizeTexture(sdl_offscreen_buffer *buffer, int width, int height) {
    int bytesPerPixel = 4;

    if (buffer->memory) {
        munmap(buffer->memory, buffer->width * buffer->height * bytesPerPixel);
    }

    if (buffer->texture) {
        SDL_DestroyTexture(buffer->texture);
    }

    buffer->texture = SDL_CreateTexture(Renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        width,
                                        height);
    buffer->width = width;
    buffer->height = height;
    buffer->pitch = width * bytesPerPixel;
    buffer->memory = mmap(0,
                          width * height * bytesPerPixel,
                          PROT_READ|PROT_WRITE,
                          MAP_PRIVATE|MAP_ANONYMOUS,
                          -1,
                          0);
}


internal void
SDLUpdateWindow(SDL_Window *window, SDL_Renderer *Renderer, sdl_offscreen_buffer *buffer) 
{
    SDL_UpdateTexture(buffer->texture, 0, buffer->memory, buffer->pitch);
    SDL_RenderCopy(Renderer, buffer->texture, 0, 0);
    SDL_RenderPresent(Renderer);
}

void loadTextureFromFile(const char *filename, game_offscreen_buffer *buffer) {

    SDL_Surface *tmp = IMG_Load(filename);

    if (tmp) {
		int key = SDL_MapRGB(tmp->format, 255, 0, 255);
		SDL_SetColorKey(tmp, SDL_TRUE, key);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, tmp);

		SDL_FreeSurface(tmp);
		SDL_DestroyTexture(texture);
    }
}

bool handleEvent(SDL_Event *event) {
    bool shouldQuit = false;

    switch (event->type) {
        case SDL_QUIT:
            shouldQuit = true;
            break;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    shouldQuit = true;
                    break;
            }


        case SDL_WINDOWEVENT:
        {
            switch(event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    /*
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);
                    */
                    printf("Window size changed (%d, %d)\n", 
                            event->window.data1,
                            event->window.data2);
                    SDLResizeTexture(&GlobalBackbuffer, event->window.data1, event->window.data2);
                } break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    printf("window focus gained\n");
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    // SDL_Renderer *renderer = SDL_GetRenderer(window);
                    SDLUpdateWindow(window, Renderer, &GlobalBackbuffer);
                } break;
            }
        } break;
    }
    return shouldQuit;
}

int main( int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);

    Window = SDL_CreateWindow("Boo The Game",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                        /*SDL_WINDOW_FULLSCREEN |*/
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (Window) {
        Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

        if (Renderer) {
            StartTime = SDL_GetTicks();
            bool running = true;
            int width, height;
            SDL_GetWindowSize(Window, &width, &height);
            SDLResizeTexture(&GlobalBackbuffer, width, height);
            int XOffset = 0;
            int YOffset = 0;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (handleEvent(&event)) {
                        running = false;
                    }
                }

                game_offscreen_buffer buffer = {};

                buffer.memory = GlobalBackbuffer.memory;
                buffer.width = GlobalBackbuffer.width;
                buffer.height = GlobalBackbuffer.height;
                buffer.pitch = GlobalBackbuffer.pitch;

                GameUpdateAndRender(&buffer, XOffset, YOffset);

                SDLUpdateWindow(Window, Renderer, &GlobalBackbuffer);
                ++XOffset;
                YOffset += 2;
            }
        } else {
            printf( "No Renderer\n");
        }
    } else {
        printf( "No window\n" );
    }

    SDL_Quit();
    return 0;
}
