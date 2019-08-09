#!/bin/bash

NAME=ImgTx

raspistill -o $NAME.jpg
# install before :
# $   sudo apt install imagemagick
convert -compress none $NAME.jpg $NAME.ppm

# convert inverse :
# $   convert img.ppm img.jpg
