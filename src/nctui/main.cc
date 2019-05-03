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

#include "../common/secpass.h"

// prototypes
void print_help ();

/*!
 * Main function
 * @param int  argc
 * @param char *argv[]
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
int
main (int argc, char *argv[])
{
  int status = 0;

  if (argc < 2) {
    print_help ();
    return status;
  }

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
    "Usage: secpass-nc /path/to/dbfile [OPTIONS]\n"     <<
    "Options:\n" <<
    "  -e\tExport secdb to kdbx file (requires path)\n"   <<
    "  -i\tImport secdb from kdbx file (requires path)\n" <<
    "  -t\tLock interface after given timeout in s\n";

  return;
}
