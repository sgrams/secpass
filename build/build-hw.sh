#!/bin/sh
## build/build.sh
## Builds all available targets
##
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
cmake .. -DSGX_MODE=HW -DSGX_BUILD=DEBUG
make
