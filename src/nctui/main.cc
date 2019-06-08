/* src/nctui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <iostream>

#include <unistd.h>
#include <ctype.h>
#include <string>
#include <vector>

#include "../common/secpass.h"
#include "../common/bridge.h"

#include "ui.h"

// prototypes
void
print_help (void);

void
print_version (void);

/*!
 * Main function
 * @param  int  argc
 * @param  char *argv[]
 * @retval int  status
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
int
main (int argc, char *argv[])
{
  int status = UI_OK;
  int idle_time = DEFAULT_IDLE_TIME;
  int c;
  std::vector<std::string> paths;
  string key_filepath;

  // parse command line parameters
  if (argc < 2) {
    print_help ();
    return status;
  }
  while ((c = getopt (argc, argv, "vht:")) != -1)
  {
    switch (c) {
    // print version
    case 'v':
      print_version ();
      return status;
    // print help
    case 'h':
      print_help ();
      return status;
    // set timeout time
    case 't':
      idle_time = stoi (optarg);
      break;
    case '?':
      return 1;
    default:
      status = -1;
    }
  }

  // handle filepaths
  for (int i = optind; i < argc; ++i)
  {
    paths.push_back (std::string (argv[i]));
  }
  if (paths.size () == 0) {
    std::cerr << "Error: Path to secpass file was not given. Exiting.\n";
    status = -1;
    return status;
  }

  // start main event loop
  status = br_enclave_init ();
  if (BRIDGE_OK != status) {
    std::cerr << "Fatal error: secpass was not able to initialize secure enclave. Exiting.\n";
    goto MAIN_SECPASS_EXIT;
  }

  if (paths.size () > 1) {
    key_filepath = paths.at(1);
  }

  status = Ui::main_loop (idle_time, paths.at(0), key_filepath);
  if (UI_OK != status) {
    if (UI_ERR_FILE == status) {
      std::cout << "Unable to open `" << paths.at(0) << "`. The database is broken or the master password is invalid.\n";
    }
    goto SAFE_SECPASS_EXIT;
  }

SAFE_SECPASS_EXIT:
  status = br_enclave_close ();
  if (BRIDGE_OK != status) {
    std::cerr << "Fatal error: secpass was not able to close secure enclave. Exiting.\n";
  }

MAIN_SECPASS_EXIT:
  return status;
}

/*!
 * Prints help to stdout
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
void
print_help ()
{
  std::cout <<
    "secpass-nc " << SECPASS_VERSION << " © " << SECPASS_AUTHORS << "\n" <<
    "Usage: secpass-nc /path/to/dbfile\n"     <<
    "Options:\n" <<
    "  -v\tprint version\n" <<
    "  -h\tprint this help\n" <<
    "  -t\texit application after given idle time in [s] (default: " << DEFAULT_IDLE_TIME << "s)\n";

  return;
}

/*!
 * Prints version to stdout
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/06/07
 */
void
print_version ()
{
  std::cout << "secpass-nc " << SECPASS_VERSION << "\n";
  return;
}
