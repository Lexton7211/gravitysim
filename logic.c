#include "logic.h"
#include "display.h"

void updateBalls(Vec* balls) {
  for(size_t i = 0; i < vec_len(balls); i++){
    Ball *ball = vec_get(balls, i);
    ball->x += ball->vx * DT + 0.5 * ball->ax * DT * DT;
    ball->y += ball->vy * DT + 0.5 * ball->ay * DT * DT;
  }
  for(size_t i = 0; i < vec_len(balls); i++) {
    for(size_t j = i + 1; j < vec_len(balls); j++){
      Ball *ball1 = vec_get(balls, i);
      Ball *ball2 = vec_get(balls, j);

      double ax1, ay1, ax2, ay2;
      calculateGravity(balls, i, j, &ax1, &ay1, &ax2, &ay2);

      ball1->vx += 0.5 * (ball1->ax + ax1) * DT;
      ball1->vy += 0.5 * (ball1->ay + ay1) * DT;
      ball2->vx += 0.5 * (ball2->ax + ax2) * DT;
      ball2->vy += 0.5 * (ball2->ay + ay2) * DT;

      ball1->ax = ax1;
      ball1->ay = ay1;
      ball2->ax = ax2;
      ball2->ay = ay2;
    }
  }
}

void createBallObject(Vec* balls, int randomVelocitys, double cx, double cy, double vx, double vy, int r, double m, SDL_Color c){
  if(randomVelocitys != 0 && randomVelocitys != 1){
    vx = (rand() % randomVelocitys) + 1;
    vy = (rand() % randomVelocitys) + 1;
  }
  Ball *ball = malloc(sizeof(Ball));
  *ball = (Ball){.x = cx, .y = cy, .vx = vx, .vy = vy, .ax = 0, .ay = 0, .r = r, .m = m, .c = c};
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
        //velocity after merge
        double vfx = (ball1->m * ball1->vx + ball2->m * ball2->vx) / (ball1->m + ball2->m);
        double vfy = (ball1->m * ball1->vy + ball2->m * ball2->vy) / (ball1->m + ball2->m);
        
        double theta = atan(vfy / vfx);

        //gravitational binding
        double e = GRAVITY_CONSTANT * ball1->m * ball2->m / sqrt(pow(ball2->x - ball1->x, 2) + pow(ball2->y - ball1->y, 2));

        //merged body's position
        double xf = (ball1->m * ball1->x + ball2->m * ball2->x) / (ball1->m + ball2->m);
        double yf = (ball1->m * ball1->y + ball2->m * ball2->y) / (ball1->m + ball2->m);

        //calculate new radius
        int rf = sqrt(pow(ball1->r, 2) + pow(ball2->r, 2));
        if(ball1->m >= ball2->m){
          createBallObject(balls, 0, xf, yf, vfx, vfy, rf, ball1->m + ball2->m, ball1->c);
        }
        else{
          createBallObject(balls, 0, xf, yf, vfx, vfy, rf, ball1->m + ball2->m, ball2->c);
        }
        vec_remove(balls, i);
        vec_remove(balls, j - 1);
        i--;
        break;

        //may use later
        /*double nx = distX / distance;
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
        ball2->vy += impulse * ball1->m * ny;*/
      }
    }
  }
}

void calculateGravity(Vec* balls, size_t i, size_t j, double *ax1, double *ay1, double *ax2, double *ay2){
  Ball *ball1 = vec_get(balls, i);
  Ball *ball2 = vec_get(balls, j);
  double distX = ball2->x - ball1->x;
  double distY = ball2->y - ball1->y;
  double distance = sqrt((distX * distX) + (distY * distY));

  if(distance < 1e-10){
    printf("Idk problem calculateGravity() function\n");
  }

  double force = GRAVITY_CONSTANT * (ball1->m * ball2->m) / (distance * distance * distance);
  double fX = force * distX;
  double fY = force * distY;

  *ax1 = fX / ball1->m * DT;
  *ay1 = fY / ball1->m * DT;
  *ax2 = -fX / ball2->m * DT;
  *ay2 = -fY / ball2->m * DT;
}
