<img src="/Img/LogoMHAB.png" width=150 align="right" >

# LoRa Image Transmission Project

## Goal : Transmit an image with two LoRa modem

## Description

Transmit a ppm image or an image taken with a raspberry Pi Camera with two LoRa modem
With this communication protocol, you can use a reliable mode. It means that the receiver
communicate the missing packets to send again while all the packets are not received
If some packets can't be send received, you can build the image whatever and the
missing pixels will be fills with black color.
-----------------------------------------------------------------
## Installation of image magick
First you need to update & upgrade your Raspberry Pi
```
sudo apt-get update
sudo apt-get upgrade
```
Then we need to install imagemagick to convert jpg and ppm image
```
sudo apt install imagemagick
```
-----------------------------------------------------------------
## Building
```
sudo apt-get install git
git clone https://github.com/MorgesHAB/LoRaImageTransmission
```
Then if your modem is the transmitter, type
```
make Transmitter
```
Or if it is the receiver modem, type
```
make Receiver
```
-----------------------------------------------------------------
## Use Syntax
For the transmitter software
```
./Transmitter image.ppm
./Transmitter livePicture
./Transmitter image.ppm [packet number]
```
for the receiver software
```
./Receiver
```
-----------------------------------------------------------------
## Installation of a web server to read the image
```
sudo apt install apache2
```
Change the right of this directory
```
sudo chown -R pi:www-data /var/www/html/
sudo chmod -R 770 /var/www/html/
```
Then install php
```
sudo apt-get intsall php5 libapache2-mod-php5
```
Now we will program in php so delete the html file and create a php one
```
sudo rm /var/www/html/index.html
echo "<?php phpinfo(); ?>" > /var/www/html/index.php
```
Now you can check that the server is working by typing the IP address of the Pi on a web browser.
for example :  `192.32.12.45`


<img src="/Img/LoRa.png" width=800>
<p align="center"><em>Illustration of the two LoRa modem, one on a Raspberry Pi zero and one on a RPi 3B</em><br></p>
