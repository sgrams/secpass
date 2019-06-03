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
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// prototypes
string
create_title (File *file);
void
draw_title (File *file);
void
draw_controls ();
void
draw_entries (File *file, uint32_t selected);
void
draw_divider (string div, uint32_t y);

// variables
Window *title_window;
Window *entries_window;
Window *control_window;
Window *input_window;

bool     colors = false;
uint32_t start_x;
uint32_t start_y;
uint32_t width;
uint32_t height;
const char divider = '-';

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
  mouseinterval (0);

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
  entries_window = new Window (start_x, start_y, width, height, "entries");
  control_window = new Window (input_start_x, input_start_y, COLS - input_start_x - 2, in_height, "controls");
  input_window = new Window (input_start_x, input_start_y, COLS - input_start_x - 2, in_height, "add entry");

  return;
}

void
Draw::draw (File *file, uint32_t selected)
{
  draw_title (file);
  draw_controls ();
  draw_entries (file, selected);

  return;
}

void
Draw::stop ()
{
  delete title_window;
  delete entries_window;
  delete control_window;
  delete input_window;

  endwin ();
  curs_set (1);
}

void
draw_title (File *file)
{
  title_window->clear ();
  title_window->invert ();
  title_window->color (TITLE_COLOR_PAIR);
  title_window->print (create_title (file), 0, 0);
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
  line << "[" << ((char) EXIT_KEY) << "] quit           ";
  line << "[space] add entry";
  control_window->print(line.str());
  line.str("");

  control_window->move(0, 1);
  line << "[" << ((char) REMOVE_KEY) << "] delete entry   ";
  line << "[enter] show entry   ";
  control_window->print(line.str());
  line.str("");

  control_window->color(BORDER_COLOR_PAIR, true);
  control_window->draw();

  return;
}

void
draw_entries (File *file, uint32_t selected)
{

  return;
}

string
create_title (File *file)
{
  string title = "[ secpass " + (const string)SECPASS_VERSION + " ]";

  uint32_t tmp = (COLS - file->path.size () - 2) / 2;

  for (size_t i = title.size (); i < tmp; ++i)
  {
    title += divider;
  }

  title += "[";
  title += file->path;
  title += "]";

  for (size_t i = title.size (); i < (size_t)COLS; ++i)
  {
    title += divider;
  }

  return title;
}
