#!/bin/sh
convert -delay 6 record/*.bmp -layers Optimize screencast.gif
rm record/*.bmp
