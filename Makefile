#CC := i686-w64-mingw32-g++
CC := g++
CPPFLAGS := -std=c++14 -Wall
#LDLIBS := /mingw64/lib/libSDL2_image.a -lSDL2_ttf -lSDL2_mixer `sdl2-config --cflags --static-libs` -static-libgcc
LDLIBS := -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ljpeg -lpng -lz -ltiff -llzma -lwebp -lfreetype -lharfbuzz -lfreetype -lgraphite2 -lbz2 -l:libmpg123.dll.a -lFLAC -lvorbisfile -lvorbis -logg `sdl2-config --cflags --static-libs` -static
#LDLIBS:= `pkg-config --libs --static --cflags SDL2 SDL2_image SDL2_mixer SDL2_ttf` -static

#SDL_CONFIG := /mingw32/bin/sdl2-config

main: main.o stage.o mymath.o handleev.o

main.o: stage.h mymath.h handleev.h
stage.o: stage.h mymath.h
mymath.o: mymath.h
handleev.o: handleev.h
