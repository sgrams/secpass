/* src/common/file.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#ifndef _SECPASS_COMMON_FILE_H
#define _SECPASS_COMMON_FILE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "secpass.h"

// for json support
#include "json.hpp"

using nlohmann::json;

#define SECPASS_FILE_VERSION 0x0

#endif //_SECPASS_COMMON_FILE_H
