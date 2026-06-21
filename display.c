#include "display.h"
#include "logic.h"

int setupDisplay(Display* display){
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  display->window = SDL_CreateWindow(
    "Gravity Simulation",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (!display->window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }


  display->renderer = SDL_CreateRenderer(
    display->window, 
    -1, 
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (!display->renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(display->window);
    SDL_Quit();
    return 1;
  }

  SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_BLEND);

  return 0;
}

void killDisplay(Display* display){
  if(display->renderer){
    SDL_DestroyRenderer(display->renderer);
  }
  if(display->window){
    SDL_DestroyWindow(display->window);
  }
  SDL_Quit();
}

void drawCircle(Display* display, Camera* camera, Vec* balls){
  for(size_t i = 0; i < vec_len(balls); i++) {
    Ball *ball = vec_get(balls, i);
    int cx = ball->x;
    int cy = ball->y;
    int r = ball->r;

    double screen_cx = (cx - camera->x) * camera->zoom + WINDOW_WIDTH  / 2;
    double screen_cy = (cy - camera->y) * camera->zoom + WINDOW_HEIGHT / 2;
    double scaled_r  = r * camera->zoom;

    if(screen_cx + scaled_r < 0){
      continue;
    }
    if(screen_cx - scaled_r > WINDOW_WIDTH){
      continue;
    }
    if(screen_cy + scaled_r < 0){
      continue;
    }
    if(screen_cy - scaled_r > WINDOW_HEIGHT){
      continue;
    }

    SDL_SetRenderDrawColor(display->renderer, ball->c.r, ball->c.g, ball->c.b, ball->c.a);

    int diameter = 2 * r;
    SDL_Point* points = malloc(diameter * diameter * sizeof(SDL_Point));
    int count = 0;

    for (int y = -r; y <= r; y++) {
      for (int x = -r; x <= r; x++) {
        if (x*x + y*y <= r*r) {
          points[count].x = ((cx + x) - camera->x) * camera->zoom + WINDOW_WIDTH / 2;
          points[count].y = ((cy + y) - camera->y) * camera->zoom + WINDOW_HEIGHT / 2;
          count++;
        }
      }
    }

    if (count > 0) {
      SDL_RenderDrawPoints(display->renderer, points, count);
    }
    free(points);
  }
}
