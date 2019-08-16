# LoRa Image Transmission Project

### Description : Transmit a PPM Image by a LoRa modem

-----------------------------------------------------------------
## Installation of image magick
First you need to update & upgrade your Raspberry Pi
```
sudo apt-get update
sudo apt-get upgrade
```
Then we need to install imagemagick to convert jpg and ppm image

`sudo apt install imagemagick`

-----------------------------------------------------------------
## Installation of a web server to read the image
`sudo apt install apache2`
Change the right of this directory
```
sudo chown -R pi:www-data /var/www/html/
sudo chmod -R 770 /var/www/html/
```
Then install php
`sudo apt-get intsall php5 libapache2-mod-php5`
Now we will program in php so delete the html file and create a php one
```
sudo rm /var/www/html/index.html
echo "<?php phpinfo(); ?>" > /var/www/html/index.php
```
Now you can check that the server is working by typing the IP adresse of the Pi on a web browser.
for example :  `192.32.12.45`
