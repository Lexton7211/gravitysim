#ifndef LOGIC_H
#define LOGIC_H
#define GRAVITY_CONSTANT 500000.0f
#define SCALE 7.5f
#define RESTITUTION 1.0f
#define SLEEP_THRESHOLD 0.5f


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "vec.h"

typedef struct {
  double x, y;
  double vx, vy;
  int r;
  double m;
  SDL_Color c;
} Ball;

void updateBalls(Vec* balls);
void createBallObject(Vec* balls, int randomVelocitys, double cx, double cy, double vx, double vy, int r, double m, SDL_Color c);
void ballCollisions(Vec* balls);
void calculateGravity(Vec* balls);

#endif
