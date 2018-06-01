CC := g++
CPPFLAGS := -std=c++14 -Wall
LDLIBS := `wx-config --cppflags --libs` -static

SDL_CONFIG := /mingw32/bin/sdl2-config

fstar:

main: main.o stage.o mymath.o handleev.o

main.o: stage.h mymath.h handleev.h
stage.o: stage.h mymath.h
mymath.o: mymath.h
handleev.o: handleev.h
