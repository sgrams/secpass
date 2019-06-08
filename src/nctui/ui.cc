/* src/nctui/ui.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "ui.h"
#include "draw.h"
#include "window.h"

#include "../common/secpass.h"
#include "../common/bridge.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

int
Ui::main_loop (int idle_time, string filepath)
{
  bool file_open = false;
  int status = 0;
  unsigned int pos = 0;
  uint8_t c;

  vector<string> entries;

  Draw::init ();

  string auth_key;
  timeout (10000000);
  Draw::draw_auth_popup (filepath, &auth_key);
  //status = br_file_open (auth_key, filepath);
  if (0 != status) {
    goto MAIN_LOOP_EXIT;
  }
  file_open = true;

  // get entries
  entries.push_back ("test1sdasdasadasdsadsadjlksajdlksajdhgdsnjgkdndkjsnkjfnesfnasssssssssssssssssssssssssssssssssaaaaaaaaaasdasdasd123123123123123123");
  entries.push_back ("test2");
  entries.push_back ("test3");

  // main loop
  while (1<2)
  {
    //br_fetch_names (&entries);
    Draw::draw (entries, filepath, pos);
    timeout (idle_time * 1000); // convert time to ms
    c = getch ();
    if ((uint8_t)ERR == c) {
      goto MAIN_LOOP_EXIT;
    }
    // break main loop if quit issued
    if (handle_input (entries, c, pos, entries.size ())) {
      break;
    }
  }

MAIN_LOOP_EXIT:
  if (file_open) {
    // save file
  }
  Draw::stop ();
  return status;
}

int
Ui::handle_input (vector<string> entries, uint8_t c, unsigned int &pos, unsigned int elements)
{
  std::string new_secret;
  std::string name_to_find;
  noecho ();

  switch (c) {
    case EXIT_KEY:
      return 1;
    case NEW_KEY:
      // fill a new secret and add to enclave
      // and save to file
      break;
    case EDIT_KEY:
      // open a secret, add to enclave, check if name and secret is ok
      // and save to file
      break;
    case UP_KEY:
      if (0 != pos) {
        pos--;
      }
      break;
    case DOWN_KEY:
      if (0 != elements) {
        pos++;
        if (pos >= elements) {
          pos = elements - 1;
        }
      }
      break;

    case FIND_KEY:
      // get a string to find
      Draw::draw_find_entry (&name_to_find);
      for (size_t i = 0; i < entries.size (); ++i)
      {
        if (entries.at (i) == name_to_find) {
          pos = i;
          break;
        }
      }
      break;

    case RESIZE_KEY:
      Draw::stop ();
      Draw::init ();
      break;

  }

  echo ();
  return 0;
}
