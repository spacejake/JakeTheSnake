#!/bin/bash

g++ snake.c -o snake -I/usr/include/plib/ -lplibpu -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lGLEW -lXi -lXmu

