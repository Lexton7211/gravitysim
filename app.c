#include "logic.h"
#include "display.h"

int main(int argc, char *argv[]){
  srand(time(NULL));

  int defaultRadius = 26;
  int deleteRadius = 32;
  int restart = 1;
  double heldX = 0.0;
  double heldY = 0.0;
  while(restart){
    int placeChoice = 1;
    int lockedBall = -1;
    int alternate = 1;
    double lineOffsetX = 0.0;
    double lineOffsetY = 0.0;
    restart = 0;
    Display display = {0};
    Camera camera = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0f};
    
    int lineStartX = 0;
    int lineStartY = 0;

    Vec balls;
    vec_init(&balls);
    Vec outline;
    vec_init(&outline);

    if(setupDisplay(&display) != 0){
      return 1;
    }

    //main loop
    int running = 1;
    int pause = 0;
    SDL_Event event;
    while(running){
      Uint32 frameStart = SDL_GetTicks();
      while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
          running = 0;
        }
        if(event.type == SDL_KEYDOWN) {
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              running = 0;
              break;
            case SDLK_r:
              restart = 1;
              running = 0;
              break;
            case SDLK_SPACE:
              if(pause == 0){
                pause = 1;
              }
              else{
                pause = 0;
              }
              break;
            case SDLK_1:
              placeChoice = 1;
              break;
            case SDLK_2:
              placeChoice = 2;
              break;
            case SDLK_3:
              placeChoice = 3;
              break;
            case SDLK_4:
              placeChoice = 4;
              break;
          }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
          double worldX = (event.button.x - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
          double worldY = (event.button.y - WINDOW_HEIGHT / 2.0) / camera.zoom + camera.y;
          if(event.button.button == SDL_BUTTON_LEFT){
            if(placeChoice == 1){
              createBallObject(&balls, 0, worldX, worldY, 0, 0, 64, 1000000, (SDL_Color){255, 255, 0, 255});
            }
            else if(placeChoice == 2){
              lineStartX = worldX;
              lineStartY = worldY;
              if(lockedBall != -1 && lockedBall < vec_len(&balls)){
                Ball *ball = vec_get(&balls, lockedBall);
                lineOffsetX = lineStartX - ball->x;
                lineOffsetY = lineStartY - ball->y;
              }
            }
            else if(placeChoice == 4){
              for(size_t i = 0; i < vec_len(&outline); i++){
                vec_remove(&outline, i);
              }
              Ball *ball1 = malloc(sizeof(Ball));
              *ball1 = (Ball){.x = worldX, .y = worldY, .vx = 0, .vy = 0, .r = deleteRadius * 2, .m = 0, .c = (SDL_Color){0, 0, 0, 0}};
              for(size_t i = 0; i < vec_len(&balls); i++){
                Ball *ball2 = vec_get(&balls, i);
                double distX = ball1->x - ball2->x;
                double distY = ball1->y - ball2->y;
                double distance = sqrt((distX * distX) + (distY * distY));

                if(distance <= ball1->r + ball2->r){
                  if(lockedBall == i){
                    lockedBall = -1;
                  }
                  else{
                    lockedBall = i;
                  }
                }
              }
              free(ball1);
            }

          }
          if(event.button.button == SDL_BUTTON_RIGHT){
            heldX = worldX;
            heldY = worldY;
          }
        }
        if(event.type == SDL_MOUSEBUTTONUP){
          double worldX = (event.button.x - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
          double worldY = (event.button.y - WINDOW_HEIGHT / 2.0) / camera.zoom + camera.y;
          if(event.button.button == SDL_BUTTON_LEFT){
            if(placeChoice == 2){
              double vx = (worldX - lineStartX) * SCALE;
              double vy = (worldY - lineStartY) * SCALE;
              if(lockedBall != -1 && lockedBall < vec_len(&balls)){
                Ball *locked = vec_get(&balls, lockedBall);
                vx += locked->vx;
                vy += locked->vy;
              }
              createBallObject(&balls, 0, lineStartX, lineStartY, -vx, -vy, defaultRadius, 100, (SDL_Color){255, 255, 255, 255});
              lineStartX = 0;
              lineStartY = 0;
            }
          }
        }
        if(event.type == SDL_MOUSEWHEEL){
          int mx, my;
          Uint32 buttons = SDL_GetMouseState(&mx, &my);

          double worldX = (mx - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
          double worldY = (my - WINDOW_HEIGHT / 2.0) / camera.zoom + camera.y;

          if(event.wheel.y == 1){
            camera.zoom += 0.1;
          }
          else{
            camera.zoom += -0.1;
          }
          if(camera.zoom > 1.5){
            camera.zoom = 1.5;
          }
          else if(camera.zoom < 0.05){
            camera.zoom = 0.05;
          }

          double worldAfterX = (mx - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
          double worldAfterY = (my - WINDOW_HEIGHT  / 2.0) / camera.zoom + camera.y;

          camera.x += worldX - worldAfterX;
          camera.y += worldY - worldAfterY;
        }
      }

      for(size_t i = 0; i < vec_len(&outline); i++){
        vec_remove(&outline, i);
      }

      int mx, my;
      Uint32 buttons = SDL_GetMouseState(&mx, &my);

      double worldX = (mx - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
      double worldY = (my - WINDOW_HEIGHT / 2.0) / camera.zoom + camera.y;


    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
      if(placeChoice == 3){
        for(size_t i = 0; i < vec_len(&outline); i++){
          vec_remove(&outline, i);
        }
        createBallObject(&outline, 0, worldX, worldY, 0, 0, deleteRadius, 0, (SDL_Color){255, 0, 0, 85});
        Ball *ball1 = malloc(sizeof(Ball));
        *ball1 = (Ball){.x = worldX, .y = worldY, .vx = 0, .vy = 0, .r = deleteRadius, .m = 0, .c = (SDL_Color){0, 0, 0, 0}};
        for(size_t i = 0; i < vec_len(&balls); i++){
          Ball *ball2 = vec_get(&balls, i);
          double distX = ball1->x - ball2->x;
          double distY = ball1->y - ball2->y;
          double distance = sqrt((distX * distX) + (distY * distY));

          if(distance <= ball1->r + ball2->r){
            vec_remove(&balls, i);
            i--;
          }
        }
        free(ball1);
      }
    }

      if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        double shiftX = worldX - heldX;
        double shiftY = worldY - heldY;

        camera.x -= shiftX;
        camera.y -= shiftY;

        heldX = worldX;
        heldY = worldY;
      }

      if(pause == 0){
        calculateGravity(&balls);
        ballCollisions(&balls);
        updateBalls(&balls);
      }
      for(size_t i = 0; i < vec_len(&balls); i++){
        Ball *ball = vec_get(&balls, i);
        if(ball->x > 300000 || ball->x < -300000){
          if(lockedBall == i){
            lockedBall = -1;
          }
          else if(lockedBall > i){
            lockedBall--;
          }
          vec_remove(&balls, i);
          i--;
        }
        else if(ball->y > 300000 || ball->y < -300000){
          if(lockedBall == i){
            lockedBall = -1;
          }
          else if(lockedBall > i){
            lockedBall--;
          }
          vec_remove(&balls, i);
          i--;
        }
      }

      if(lockedBall != -1 && lockedBall < vec_len(&balls)){
        Ball *ball = vec_get(&balls, lockedBall);
        camera.x = ball->x;
        camera.y = ball->y;
      }

      const Uint8* state = SDL_GetKeyboardState(NULL);
      if(state[SDL_SCANCODE_F]){
        if(placeChoice == 1){
          if(alternate){
            createBallObject(&balls, 0, worldX, worldY, 0, 0, defaultRadius, 100, (SDL_Color){255, 255, 255, 255});
            alternate = 0;
          }
          else{
            alternate = 1;
          }
        }
        if(placeChoice == 2){
          if(pause == 0){
            if(alternate){
              double vx =  (worldX - lineStartX) * SCALE;
              double vy = (worldY - lineStartY) * SCALE;
              if(lockedBall != -1 && lockedBall < vec_len(&balls)){
                Ball *locked = vec_get(&balls, lockedBall);
                vx += locked->vx;
                vy += locked->vy;
              }
              createBallObject(&balls, 0, lineStartX, lineStartY, -vx, -vy, 10, 40, (SDL_Color){255, 255, 255, 255});
              alternate = 0;
            }
            else{
              alternate = 1;
            }
          }
        }
      }

      if(lockedBall != -1 && lockedBall < vec_len(&balls) && placeChoice == 2 && lineStartX != 0 && lineStartY != 0){
        Ball *ball = vec_get(&balls, lockedBall);
        lineStartX = ball->x + lineOffsetX;
        lineStartY = ball->y + lineOffsetY;
      }

      Uint32 button = SDL_GetMouseState(&mx, &my);

      worldX = (mx - WINDOW_WIDTH  / 2.0) / camera.zoom + camera.x;
      worldY = (my - WINDOW_HEIGHT / 2.0) / camera.zoom + camera.y;


      //rendering
      SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 255);
      SDL_RenderClear(display.renderer);


      if(lineStartX != 0 && lineStartY != 0){
        createBallObject(&outline, 0, lineStartX, lineStartY, 0, 0, defaultRadius, 0, (SDL_Color){255, 255, 255, 128});
        SDL_SetRenderDrawColor(display.renderer, 255, 0, 0, 255);
        int screenStartX = (lineStartX - camera.x) * camera.zoom + WINDOW_WIDTH  / 2.0;
        int screenStartY = (lineStartY - camera.y) * camera.zoom + WINDOW_HEIGHT / 2.0;
        int screenEndX   = (worldX     - camera.x) * camera.zoom + WINDOW_WIDTH  / 2.0;
        int screenEndY   = (worldY     - camera.y) * camera.zoom + WINDOW_HEIGHT / 2.0;

        SDL_RenderDrawLine(display.renderer, screenStartX, screenStartY, screenEndX, screenEndY);
      }

      drawCircle(&display, &camera, &outline);
      drawCircle(&display, &camera, &balls);

      SDL_RenderPresent(display.renderer);
  
      Uint32 frameEnd = SDL_GetTicks();
      Uint32 elapsed = frameEnd - frameStart;
      if(elapsed > 0){
        printf("FPS: %f\n", 1000.0 / elapsed);
      }

      SDL_Delay(DT);
    }
    killDisplay(&display);
  }
  return 0;
}
