#/bin/bash
export LD_LIBRARY_PATH=./lib
g++ ChessSDL2.cpp -static-libgcc -static-libstdc++ -lSDL2 lib/libSDL2main.a -lSDL2_ttf -o ChessSDL2

