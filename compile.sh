#!/bin/sh
gcc -ggdb -Wall -o terrain *.c -lm -lGL -lX11 -lfreeimage -lassimp -lglfw
