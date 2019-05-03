#!/bin/sh
## build/build.sh
## Builds all available targets
##
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
cmake ..
make
mv src/secpass-nc .
mv src/secpass-qt .
make clean
