#!/bin/sh
## build/build-qt.sh
## Builds qturses target
##
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
cmake ..
make secpass-qt
mv src/secpass-qt .
make clean
