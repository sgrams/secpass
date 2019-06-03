/* src/nctui/window.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_NCTUI_WINDOW_H
#define _SECPASS_NCTUI_WINDOW_H
#include "curses.h"
#include <string>

using namespace std;

class Window {
  private:
    static bool colors;

    WINDOW *win;
    WINDOW *border;

    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;

		string name;

  public:
    Window (uint32_t x, uint32_t y, uint32_t width, uint32_t height, string name, bool border = 1);
    ~Window ();

    static void set_colors (bool colors);

    void clear ();
    void draw ();

    void move (uint32_t x, uint32_t y);

    void print (string text);
    void print (string text, uint32_t x, uint32_t y);

    void invert (bool border = 0);
    void revert (bool border = 0);

    void color (int pair, bool border = 0);
    void color_off (int pair, bool border = 0);

    bool mouse (int &x, int &y);

    WINDOW *get_window ();
};
#endif // _SECPASS_NCTUI_WINDOW_H
