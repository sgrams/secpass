#!/bin/sh
## build/build-nc.sh
## Builds ncurses target
##
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
cmake ..
make secpass-nc
mv src/secpass-nc .
make clean
