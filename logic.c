#include "logic.h"
#include "display.h"

void updateBalls(Vec* balls) {
  for (size_t i = 0; i < vec_len(balls); i++) {
    Ball *ball = vec_get(balls, i);
    ball->x += ball->vx * DT;
    ball->y += ball->vy * DT;
  }
}

void createBallObject(Vec* balls, int randomVelocitys, double cx, double cy, double vx, double vy, int r, double m, SDL_Color c){
  if(randomVelocitys != 0 && randomVelocitys != 1){
    vx = (rand() % randomVelocitys) + 1;
    vy = (rand() % randomVelocitys) + 1;
  }
  Ball *ball = malloc(sizeof(Ball));
  *ball = (Ball){.x = cx, .y = cy, .vx = vx, .vy = vy, .r = r, .m = m, .c = c};
  vec_push(balls, ball);
}

void ballCollisions(Vec* balls){
  for(size_t i = 0; i < vec_len(balls); i++){
    for(size_t j = i + 1; j < vec_len(balls); j++){
      Ball *ball1 = vec_get(balls, i);
      Ball *ball2 = vec_get(balls, j);
      double distX = ball1->x - ball2->x;
      double distY = ball1->y - ball2->y;
      double distance = sqrt((distX * distX) + (distY * distY));

      if (distance <= ball1->r + ball2->r && distance > 0) {
        double nx = distX / distance;
        double ny = distY / distance;

        double overlap = (ball1->r + ball2->r) - distance + 0.5;
        ball1->x += nx * (overlap / 2.0);
        ball1->y += ny * (overlap / 2.0);
        ball2->x -= nx * (overlap / 2.0);
        ball2->y -= ny * (overlap / 2.0);

        double dvx = ball1->vx - ball2->vx;
        double dvy = ball1->vy - ball2->vy;
        double dot = dvx * nx + dvy * ny;

        if (dot >= 0) continue;

        double impulse = (2.0 * dot) / (ball1->m + ball2->m);

        ball1->vx -= impulse * ball2->m * nx;
        ball1->vy -= impulse * ball2->m * ny;
        ball2->vx += impulse * ball1->m * nx;
        ball2->vy += impulse * ball1->m * ny;
      }
    }
  }
}

void calculateGravity(Vec* balls){
  for(size_t i = 0; i < vec_len(balls); i++){
    for(size_t j = i + 1; j < vec_len(balls); j++){
      Ball *ball1 = vec_get(balls, i);
      Ball *ball2 = vec_get(balls, j);
      double distX = ball2->x - ball1->x;
      double distY = ball2->y - ball1->y;
      double distance = sqrt((distX * distX) + (distY * distY));
      if (distance < 1e-10) continue;

      double force = GRAVITY_CONSTANT * (ball1->m * ball2->m) / (distance * distance * distance);
      double fX = force * distX;
      double fY = force * distY;
      ball1->vx += fX / ball1->m * DT;
      ball1->vy += fY / ball1->m * DT;
      ball2->vx -= fX / ball2->m * DT;
      ball2->vy -= fY / ball2->m * DT;
    }
  }
}
