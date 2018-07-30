#!/usr/bin/env bash
#this script switches your monitor to 720p if for some reason that resolution is not available for you.
sudo xrandr --newmode "1280x720_30.00"   35.25  1280 1320 1440 1600  720 723 728 736 -hsync +vsync
sudo xrandr --addmode eDP-1-1 1280x720_30.00
xrandr --output eDP-1-1 --mode 1280x720_30.00 --rate 30.00

