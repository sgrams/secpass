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
const int32_t list_start_x = 2; // start col
const int32_t list_start_y = 6; // start row
const int32_t input_start_x = 2; // start col
const int32_t input_start_y = 2; // start row
const int32_t title_height  = 1; // height in rows

// heights
const float   list_height  = 0.75f;
const float   input_height  = 0.1f;

// key bindings
const uint8_t EXIT_KEY   = (uint8_t)'q';
const uint8_t SAVE_KEY   = (uint8_t)'s';
const uint8_t DOWN_KEY   = (uint8_t)KEY_DOWN;
const uint8_t UP_KEY     = (uint8_t)KEY_UP;
const uint8_t NEW_KEY    = (uint8_t)'n';
const uint8_t REMOVE_KEY = (uint8_t)'d';
const uint8_t FIND_KEY   = (uint8_t)'/';
const uint8_t EDIT_KEY   = (uint8_t)'e';
const uint8_t RESIZE_KEY = (uint8_t)KEY_RESIZE;
const uint8_t COPY_KEY   = (uint8_t)'\n';

// namespace with window-drawing functions
namespace Draw {
  void init ();
  void draw (vector<string> entries, const string filepath, uint32_t selected);
  void draw_auth_popup (const string filepath, string *key);
  void draw_find_entry (string *str);
  void draw_new_entry (string *str, string *sec);
  void draw_edit_entry (string *str, string *sec, bool *to_be_closed);
  void draw_remove_entry (bool *to_be_closed);
  void print_message (const char *msg);
  void stop ();
}
#endif // NCTUI_DRAW_H
