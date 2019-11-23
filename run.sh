#! /bin/bash

[ -f textures.o ] && { rm textures.o; }

g++ -o textures.o textures.cpp  -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w

./textures.o