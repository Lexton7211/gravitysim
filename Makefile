SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LIBS := $(shell pkg-config --libs sdl2)

build:
	gcc $(SDL_CFLAGS) app.c display.c logic.c vec.c -o app $(SDL_LIBS) -lm

run:
	./app
