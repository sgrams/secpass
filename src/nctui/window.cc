/* src/nctui/window.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "window.h"
#include <string>

using namespace std;

bool Window::colors;

Window::Window (uint32_t x, uint32_t y, uint32_t width, uint32_t height, string name, bool border)
{
  this->x = x;
  this->y = y;
  this->width  = width;
  this->height = height;

  this->name   = name;
  this->border = newwin (height, width, y, x);

  if (border) {
    win = newwin (height - 2, width - 2, y + 1, x + 1);
  } else {
    win = newwin (height, width, y, x);
  }
}

Window::~Window ()
{
  delwin (border);
  delwin (win);
}

void
Window::set_colors (bool colors)
{
  Window::colors = colors;
}

void
Window::clear ()
{
  werase (border);
  werase (win);
}

void
Window::draw ()
{
  box (border, 0, 0);
  invert (true);
  mvwprintw (border, 0, 0, string ("[" + name + "]").c_str ());
  revert (true);
  wrefresh (border);
  wrefresh (win);
}

void
Window::move (uint32_t x, uint32_t y)
{
  wmove (win, y, x);
}

void
Window::print (string text)
{
  wprintw (win, text.c_str());
}

void
Window::print (string text, uint32_t x, uint32_t y)
{
  mvwprintw (win, y, x, text.c_str ());
}

void
Window::invert (bool border)
{
  if (border) {
    wattron (this->border, A_STANDOUT);
  }
  else {
    wattron (this->win, A_STANDOUT);
  }
}

void
Window::revert (bool border)
{
  if (border) {
    wattroff (this->border, A_STANDOUT);
  }
  else {
    wattroff (this->win, A_STANDOUT);
  }
}

void Window::color (int pair, bool border)
{
  if (border) {
    wattron (this->border, COLOR_PAIR(pair));
  }
  else {
    wattron (this->win, COLOR_PAIR(pair));
  }
}

void
Window::color_off (int pair, bool border)
{
  if (border) {
    wattroff (this->border, COLOR_PAIR(pair));
  }
  else {
    wattroff (this->win, COLOR_PAIR(pair));
  }
}

bool
Window::mouse (int &x, int &y)
{
 return wmouse_trafo (win, &y, &x, false);
}

WINDOW*
Window::get_window ()
{
  return win;
}
