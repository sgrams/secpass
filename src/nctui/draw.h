/* src/nctui/window.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_NCTUI_DRAW_H
#define _SECPASS_NCTUI_DRAW_H
#include "curses.h"
#include "../common/file.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// color settings
const int32_t TITLE_FOREGROUND = COLOR_YELLOW;
const int32_t TITLE_BACKGROUND = -1;
const int32_t TITLE_COLOR_PAIR = 1;

const int32_t SELECT_FOREGROUND = COLOR_BLACK;
const int32_t SELECT_BACKGROUND = COLOR_GREEN;
const int32_t SELECT_COLOR_PAIR = 2;

const int32_t BORDER_FOREGROUND = COLOR_YELLOW;
const int32_t BORDER_BACKGROUND = -1;
const int32_t BORDER_COLOR_PAIR = 3;

// start coordinates
const int32_t list_start_x = 2;
const int32_t list_start_y = 6;
const float   list_height  = 0.75f;

const int32_t input_start_x = 2;
const int32_t input_start_y = 2;
const float   input_height  = 0.1f;

const int32_t title_height  = 1;

// keybinds
const uint8_t EXIT_KEY   = (uint8_t)'q';
const uint8_t DOWN_KEY   = (uint8_t)KEY_DOWN;
const uint8_t UP_KEY     = (uint8_t)KEY_UP;
const uint8_t NEW_KEY    = (uint8_t)'n';
const uint8_t REMOVE_KEY = (uint8_t)'d';
const uint8_t EDIT_KEY   = (uint8_t)'e';
const uint8_t FIND_KEY   = (uint8_t)'/';
const uint8_t RESIZE_KEY = (uint8_t)KEY_RESIZE;

namespace Draw {
  void init ();
  void draw (vector<string> entries, string filepath, uint32_t selected);
  void draw_auth_popup (string filepath, string *key);
  void draw_find_entry (string *str);
  void stop ();
}
#endif // NCTUI_DRAW_H
