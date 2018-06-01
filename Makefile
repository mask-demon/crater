CC := g++
CPPFLAGS := -std=c++14 -Wall
LDLIBS := `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SDL_CONFIG := /mingw32/bin/sdl2-config

main: main.o stage.o mymath.o handleev.o

main.o: stage.h mymath.h handleev.h
stage.o: stage.h mymath.h
mymath.o: mymath.h
handleev.o: handleev.h
