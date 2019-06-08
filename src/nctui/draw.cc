/* src/nctui/draw.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "draw.h"
#include "window.h"
#include "../common/secpass.h"
#include "../common/file.h"
#include <curses.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// prototypes
string
create_title (string filepath);
void
draw_title (string filepath);
void
draw_controls ();
void
draw_entries (vector<string> entries, uint32_t selected);
void
draw_divider (string div, uint32_t y);
void
draw_auth_popup ();

// variables
Window *title_window;
Window *entries_window;
Window *details_window;
Window *control_window;
Window *input_window;
Window *find_window;
Window *auth_window;

bool     colors = false;
uint32_t start_x;
uint32_t start_y;
uint32_t width;
uint32_t height;
const char divider = '-';


uint32_t x_pos;
uint32_t y_pos;
uint32_t end_entry   = 0;
uint32_t start_entry = 0;
uint32_t list_offset = 0;

void
Draw::init ()
{
  cout << "\033]0;" << "secpass " << SECPASS_VERSION << "\7" << std::flush;
  initscr ();
  raw ();
  use_default_colors ();

  if (has_colors ()) {
    start_color ();
    init_pair (TITLE_COLOR_PAIR, TITLE_FOREGROUND, TITLE_BACKGROUND);
    init_pair (SELECT_COLOR_PAIR, SELECT_FOREGROUND, SELECT_BACKGROUND);
    init_pair (BORDER_COLOR_PAIR, BORDER_FOREGROUND, BORDER_BACKGROUND);

    colors = true;
  }
  Window::set_colors (colors);

  keypad (stdscr, true);
  noecho ();

  clear ();
  refresh ();

  curs_set (0);

  start_x = list_start_x;
  start_y = list_start_y;
  width   = COLS - start_x - 2;
  height  = LINES * list_height;

  int32_t in_height = input_height * LINES;
  in_height = max (in_height, 4);

  title_window   = new Window (0, 0, COLS, title_height, "title", false);
  entries_window = new Window (start_x, start_y, width, height, "secrets");
  details_window = new Window (start_x, start_y, width, height, "details");
  control_window = new Window (input_start_x, input_start_y, COLS - input_start_x - 2, in_height, "controls");
  input_window = new Window (input_start_x, input_start_y, COLS - input_start_x - 2, in_height, "add entry");
  find_window = new Window (input_start_x, input_start_y, COLS - input_start_x - 2, in_height, "find entry");

  return;
}

void
Draw::draw (vector<string> entries, string filepath, uint32_t selected)
{
  draw_title (filepath);
  draw_controls ();
  draw_entries (entries, selected);

  return;
}

void
Draw::stop ()
{
  delete title_window;
  delete entries_window;
  delete control_window;
  delete input_window;
  delete find_window;
  delete details_window;

  endwin ();
  curs_set (1);
}

void
Draw::draw_auth_popup (string filepath, string *key) {
  int w = 50;
  int h = 10;
  int x = (COLS  - w) / 2;
  int y = (LINES - h) / 2;

  curs_set (0);
  auth_window = new Window (x, y, 50, 10, "authorization", true);
  draw_title (filepath);
  auth_window->get_password ("Please enter master password:", key);

  curs_set (1);
  delete auth_window;
}

void
Draw::draw_find_entry (string *str) {
  find_window->clear ();
  find_window->draw ();
  find_window->get_input ("Type a name of the secret", str);
}

void
draw_title (string filepath)
{
  title_window->clear ();
  title_window->invert ();
  title_window->color (TITLE_COLOR_PAIR);
  title_window->print (create_title (filepath), 0, 0);
  title_window->color_off (TITLE_COLOR_PAIR);
  title_window->revert ();
  title_window->color (BORDER_COLOR_PAIR);
  title_window->draw ();

  return;
}

void
draw_controls ()
{
  control_window->clear ();
  stringstream line;

  control_window->move (0, 0);
  line << "[" << ((unsigned char) EXIT_KEY) << "] quit secpass";
  control_window->print(line.str());
  line.str("");

  control_window->move(0, 1);
  line << "[" << ((unsigned char) NEW_KEY) << "] new secret : ";
  line << "[" << ((char) REMOVE_KEY) << "] delete selected : ";
  line << "[" << ((unsigned char) FIND_KEY) << "] find secret : ";
  line << "[enter] show selected ";
  control_window->print(line.str());
  line.str("");

  control_window->color(BORDER_COLOR_PAIR, true);
  control_window->draw();

  return;
}

void
draw_entries (vector<string> entries, uint32_t selected)
{
  entries_window->clear ();

  if (entries.size ()) {
    x_pos = 1;
    y_pos = 1;

    size_t num_entries = height - 2;
    end_entry = entries.size ();

    if (end_entry > num_entries) {
      end_entry = num_entries;
    }

    if (num_entries < entries.size ()) {
      while (selected > end_entry + list_offset - 2 && selected != 0) {
        list_offset++;
      }
      while (selected < start_entry + list_offset && selected != entries.size () - 1)
      {
        list_offset--;
      }
    } else {
      list_offset = 0;
    }

    size_t count = start_entry + list_offset;
    for (size_t i = start_entry + list_offset; i < end_entry + list_offset && i < entries.size (); ++i)
    {
      string entry = entries.at (i);
      // show enumeration
      entries_window->invert ();
      entries_window->color (TITLE_COLOR_PAIR);

      std::string num = std::to_string (count + 1);
      if (num.size () < 2)
      {
        num = "0" + num;
      }

      entries_window->print (num + "#", x_pos, y_pos);
      entries_window->color_off (TITLE_COLOR_PAIR);
      entries_window->revert ();
      x_pos += 5;

      std::string line = entries.at (i);
      size_t padding = 8;
      if (line.size () >= width - padding) {
        line = line.substr (0, width - padding - 4) + "...";
      }

      if (i == selected) {
        for (size_t j = line.size () + x_pos; j < width - 3; j++)
        {
          line += " ";
        }
        entries_window->color (SELECT_COLOR_PAIR);
        entries_window->print (line, x_pos, y_pos);
        entries_window->color_off (SELECT_COLOR_PAIR);
        entries_window->revert ();
      } else {
        entries_window->print (line, x_pos, y_pos);
      }

      y_pos++;
      // all printed
      if (y_pos > num_entries || i >= entries.size () - 1) {
        break;
      }
      x_pos -= 5;
      count++;
    }
  }

  entries_window->color (BORDER_COLOR_PAIR, true);
  entries_window->draw ();
  return;
}

string
create_title (string filepath)
{
  string title = "[ secpass " + (const string)SECPASS_VERSION + " ]";

  uint32_t tmp = (COLS - filepath.size () - 2) / 2;

  for (size_t i = title.size (); i < tmp; ++i)
  {
    title += divider;
  }

  title += "[";
  title += filepath;
  title += "]";

  for (size_t i = title.size (); i < (size_t)COLS; ++i)
  {
    title += divider;
  }

  return title;
}
