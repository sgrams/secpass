/* src/nctui/ui.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_NCTUI_UI_H
#define _SECPASS_NCTUI_UI_H
#include "ui.h"
#include "draw.h"
#include "window.h"

#include "../common/secpass.h"
#include "../common/file.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

#define UI_OK        0x00
#define UI_ERR_FILE  0x01
#define UI_ERR_BR    0x02
#define UI_ERR_UNDEF 0xFF

namespace Ui {
  int main_loop (int idle_time, string filepath, string key_filepath);
  int handle_input (string filepath, uint8_t *salt, size_t salt_len, vector<string> entries, uint8_t c, unsigned int &pos, unsigned int elements);
};

#endif // _SECPASS_NCTUI_UI_H
