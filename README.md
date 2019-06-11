# secpass: password manager powered by Intel® SGX
<img style="text-center;" src="https://raw.githubusercontent.com/sgrams/secpass/master/assets/logo/logo.png" alt="logo" width="100"/>

*secpass* is a password manager being created for people who value their privacy. \
By utilizing Intel® Software Guard Extensions, secpass is meant to protect your passwords
and notes in secret memory enclaves – hence "sec" in secpass.

## Build status
[![CircleCI](https://circleci.com/gh/sgrams/secpass/tree/master.svg?style=svg)](https://circleci.com/gh/sgrams/secpass/tree/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/496080b6ae604018bbb243206e4f5433)](https://app.codacy.com/app/sgrams/secpass?utm_source=github.com&utm_medium=referral&utm_content=sgrams/secpass&utm_campaign=Badge_Grade_Dashboard)
![License Badge](https://img.shields.io/github/license/sgrams/secpass.svg?color=red)
## Prerequisites
### Common
- [Intel® SGX SDK](https://github.com/intel/linux-sgx)
- OpenSSL
- Argon2 library
- pkg-config
- CMake
- GCC *or* Clang
- xclip (for TUI clipboard support)
- ncurses

## Building secpass from source
```
$ git clone https://github.com/sgrams/secpass.git
$ cd secpass/build/
```

Build the project with following commands:
```
$ cd build/
$ ./build.sh
```

Run with:
```
$ cd bin/
$ ./secpass-nc
```

## Documentation
Documentation is available under [docs.secpass.org](https://docs.secpass.org/) and is dynamically
generated with Doxygen.

In order to generate docs please run
```
$ cd docs/ && doxygen secpass.doxy
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
