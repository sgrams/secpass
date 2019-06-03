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

const int32_t SELECT_FOREGROUND = COLOR_RED;
const int32_t SELECT_BACKGROUND = -1;
const int32_t SELECT_COLOR_PAIR = 3;

const int32_t BORDER_FOREGROUND = COLOR_YELLOW;
const int32_t BORDER_BACKGROUND = -1;
const int32_t BORDER_COLOR_PAIR = 4;

// start coordinates
const int32_t list_start_x = 2;
const int32_t list_start_y = 6;
const float   list_height  = 0.75f;

const int32_t input_start_x = 2;
const int32_t input_start_y = 2;
const float   input_height  = 0.1f;

const int32_t title_height  = 1;

// keybinds
const int32_t EXIT_KEY = 'q';
const int32_t DOWN_KEY = KEY_DOWN;
const int32_t UP_KEY = KEY_UP;
const int32_t NEW_KEY = ' ';
const int32_t REMOVE_KEY = 'd';
const int32_t EDIT_KEY = 'e';
const int32_t FIND_KEY = '/';

namespace Draw {
  void init ();
  void draw (File *file, uint32_t selected);
  void stop ();

  void
  get_input (string str = "");
}
#endif // _SECPASS_NCTUI_DRAW_H
