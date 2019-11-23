#! /bin/bash

[ -f Challenge4Textures.o ] && { rm Challenge4Textures.o; }

g++ -o Challenge4Textures.o Challenge4Textures.cpp  -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w

./Challenge4Textures.o