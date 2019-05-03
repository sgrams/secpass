# secpass: password manager powered by Intel® SGX
secpass is a password manager being created for people who value their privacy.
By utilizing Intel® Software Guard Extensions, secpass is meant to protect your passwords
and notes in secret memory enclaves – hence "sec" in secpass.

## build status
[![Build Status](https://travis-ci.org/sgrams/secpass.svg?branch=master)](https://travis-ci.org/sgrams/secpass)

## prerequisites
- [Intel® SGX SDK](https://github.com/intel/linux-sgx)
- Qt5
- CMake
- GCC *or* Clang

## building secpass from source
```
# git clone https://github.com/sgrams/secpass.git
# cd secpass/
# mkdir build
# cd build && cmake ..
# make
```
### running secpass with Qt5 GUI
```
# ./secpass-qt
```
### running secpass with ncurses TUI
```
# ./secpass-nc
```

## MIT License

Copyright (c) 2019 Stanisław Grams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
