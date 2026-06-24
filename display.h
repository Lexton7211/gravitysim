#ifndef DISPLAY_H
#define DISPLAY_H

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000
#define DT (1.0 / 180.0)

#include <SDL2/SDL.h>
#include "logic.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} Display;

typedef struct {
  int y;
  int x;
  float zoom;
} Camera;

typedef struct {
  SDL_Color color;
  float alpha;
  float duration;
  float elapsed;
  int active;
} FlashEffect;

int setupDisplay(Display* display);
void killDisplay(Display* display);
void drawCircle(Display* display, Camera* camera, Vec* balls);
void flash_start(FlashEffect *flash, SDL_Color color, float duration);
void flash_update(FlashEffect *flash);
void flash_render(Display* display, FlashEffect *flash);

#endif
