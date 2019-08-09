#!/bin/bash

NAME=ImgTx

raspistill -o $NAME.jpg
convert -compress none $NAME.jpg $NAME.ppm
