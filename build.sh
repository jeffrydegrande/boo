#!/bin/bash

gcc -o boo sdl_boo.cpp `sdl2-config --cflags --libs` -lsdl2_image
echo 'ok'
