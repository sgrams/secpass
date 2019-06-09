/* src/nctui/ui.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "ui.h"
#include "draw.h"
#include "window.h"
#include "clipboard.h"

#include "../common/secpass.h"
#include "../common/bridge.h"

#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

int
Ui::main_loop (int idle_time, string filepath, string key_filepath)
{
  bool file_open = false;
  int status = 0;
  unsigned int pos = 0;
  uint8_t c;
  string auth_key;
  string master_key;

  vector<string> entries;

  Draw::init ();

  status = br_file_check (filepath);
  if (BRIDGE_ER_WR_EXT == status) {
    status = UI_ERR_FILE;
    goto MAIN_LOOP_EXIT;
  }

  Draw::draw_auth_popup (filepath, &auth_key);

  // perform KDF if file provided
  master_key = auth_key;

  if (BRIDGE_ER_WR_FILE == status) {
    br_file_create (filepath, master_key); // create file if does not exist
  }

  status = br_file_open (master_key, filepath);
  if (0 != status) {
    status = UI_ERR_FILE;
    goto MAIN_LOOP_EXIT;
  }
  file_open = true;

  // main loop
  while (1<2)
  {
    br_fetch_names (&entries); // get entries from enclave
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
    br_file_save (filepath);
  }
  Draw::stop ();
  return status;
}

int
Ui::handle_input (vector<string> entries, uint8_t c, unsigned int &pos, unsigned int elements)
{
  static char secret[MAX_SECRET_LEN] = {0};

  std::string new_secret_name;
  std::string new_secret;
  std::string name_to_find;
  noecho ();

  switch (c) {
    case EXIT_KEY:
      return 1;

    case NEW_KEY:
      Draw::draw_new_entry (&new_secret_name, &new_secret);
      br_secret_add (new_secret_name, new_secret);
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
        if (entries.at (i).find (name_to_find) == 0) {
          pos = i;
          break;
        }
      }
      break;

    case COPY_KEY:
      name_to_find = entries.at (pos);
      br_secret_fetch (name_to_find, secret);
      // copy to clipboard here
      printw ("%s", secret);
      memset (secret, 0, MAX_SECRET_LEN);
      break;

    case RESIZE_KEY:
      Draw::stop ();
      Draw::init ();
      break;
  }
  return 0;
}
