/* src/nctui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <vector>

#include "../common/secpass.h"
#include "../common/wrapper.h"

#include "window.h"
#include "draw.h"

// prototypes
void
print_help (void);

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
  int status = 0;
  int c;
  std::vector<std::string> paths;

  // parse command line parameters
  if (argc < 2) {
    print_help ();
    return status;
  }
  while ((c = getopt (argc, argv, "e:i:t:")) != -1)
  {
    switch (c) {
    case 'e':
      break;
    case 'i':
      break;
    case 't':
      break;
    case '?':
      if (optopt == 'e' || optopt == 'i' || optopt == 't') {
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      } else {
        fprintf (stderr, "Unknown option `-%c`.\n", optopt);
      }
      return 1;
    default:
      status = -1;
    }
  }
  for (int i = optind; i < argc; ++i)
  {
    paths.push_back (std::string (argv[i]));
  }
  if (paths.size () == 0) {
    fprintf (stderr, "Fatal error: none path given!\n");
    status = -1;
    return status;
  }
  initialize_enclave ();
  File *file = new File ("dupa.secdb");

  // start and handle UI
  uint32_t pos = 0;
  Draw::init ();
  while (true)
  {
    Draw::draw (file, pos);
    getch ();
  }


  destroy_enclave ();
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
    "Usage: secpass-nc /path/to/dbfile [OPTIONS] /path/to/file\n"     <<
    "Options:\n" <<
    "  -e\tExport secdb to kdbx file (requires path)\n"   <<
    "  -i\tImport secdb from kdbx file (requires path)\n" <<
    "  -t\tLock interface after given timeout in s\n";

  return;
}
