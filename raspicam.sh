#!/bin/bash

DATE=$("%Y-%m-%d_%H%M")

raspistill -vf -hf -o /home/pi/camera/$DATE.jpg

# sudo ?
raspiyuv -rgb -o $DATE.rgb
echo -e "P3\n3280 2464\n255" > $DATE.ppm
cat $DATE.rgb >> test.ppm
