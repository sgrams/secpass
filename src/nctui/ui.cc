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
#include <fstream>
#include <chrono>

using namespace std;

int
Ui::main_loop (int idle_time, string filepath, string key_filepath)
{
  bool file_open = false;
  bool file_new  = false;

  int status = 0;
  unsigned int pos = 0;
  uint8_t c;
  vector<string> entries;
  uint8_t salt[KDF_SALT_SIZE] = {0};
  size_t  salt_len = KDF_SALT_SIZE;
  string password;

  uint8_t master_key[KDF_KEY_SIZE] = {0};
  size_t  master_key_len = KDF_KEY_SIZE;

  // initialize UI
  Draw::init ();

  // open db file
  status = br_file_valid (filepath);
  if (BRIDGE_ER_WR_EXT == status ||
      BRIDGE_ER_WR_FILE == status) {
    status = UI_ERR_FILE;
    goto MAIN_LOOP_EXIT;
  } else if (BRIDGE_ER_FILE_NONEX == status) {
    file_new = true;
    printw ("File may be invalid. Creating new one.");
  }

  br_file_get_init (filepath, file_new, salt, salt_len);
  Draw::draw_auth_popup (filepath, &password);

  status = br_derive_key_argon2 (
    password.c_str (),
    br_file_exists (key_filepath) ? NULL : key_filepath.c_str (),
    salt,
    salt_len,
    master_key,
    master_key_len
    );

  // clear user input
  password.clear ();
  if (!file_new) {
    status = br_file_open (master_key, master_key_len, filepath);
  }

  if (status == BRIDGE_ER_FILE_INIT) {
    printw ("Creating new file...");
  } else if (-1 == status) {
    status = UI_ERR_FILE;
    goto MAIN_LOOP_EXIT;
  } else {
  }

  // send master key to the enclave
  br_auth (master_key, master_key_len);
  // clear master key after file opening
  memset (master_key, 0, KDF_KEY_SIZE);
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
    if (handle_input (filepath, salt, salt_len, entries, c, pos, entries.size ())) {
      break;
    }
  }

MAIN_LOOP_EXIT:
  if (file_open) {
    br_file_save (filepath, salt, salt_len);
    if (br_file_exists (filepath + ".lockfile") == BRIDGE_OK) {
      string lockfile_path = filepath + ".lockfile";
      std::remove (lockfile_path.c_str ());
    }
  }
  Draw::stop ();
  return status;
}

int
Ui::handle_input (string filepath, uint8_t *salt, size_t salt_len, vector<string> entries, uint8_t c, unsigned int &pos, unsigned int elements)
{
  static char secret[MAX_SECRET_LEN] = {0};

  std::string new_secret_name;
  std::string new_secret;
  std::string name_to_find;
  bool to_be_deleted;
  noecho ();

  switch (c) {
    case EXIT_KEY:
      return 1;

    case NEW_KEY:
      Draw::draw_new_entry (&new_secret_name, &new_secret);
      if (new_secret_name.size () > MAX_NAME_LEN) {
        new_secret_name.resize (MAX_NAME_LEN);
      }
      if (new_secret.size () > MAX_SECRET_LEN) {
        new_secret.resize (MAX_SECRET_LEN);
      }

      br_secret_add (new_secret_name, new_secret);
      break;

    case SAVE_KEY:
      br_file_save (filepath, salt, salt_len);
      break;

    case REMOVE_KEY:
      to_be_deleted = false;
      Draw::draw_remove_entry (&to_be_deleted);
      if (to_be_deleted && entries.size () > 0) {
        br_secret_del (entries.at (pos));
      }
      pos = 0;
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
      if (entries.size () > 0) {
        name_to_find = entries.at (pos);
        br_secret_fetch (name_to_find, secret);
        Clipboard::copy (secret);
        printw ("Entry copied. Please press any key to wipe the clipboard and continue.");
        timeout (CLIPBOARD_TIME * 1000); // convert time to ms
        getch ();
        Clipboard::copy ("");
        memset (secret, 0, MAX_SECRET_LEN);
        Draw::stop ();
        Draw::init ();
      }
      break;

    case RESIZE_KEY:
      Draw::stop ();
      Draw::init ();
      break;
  }
  return 0;
}
