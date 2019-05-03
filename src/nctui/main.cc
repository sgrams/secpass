/* src/nctui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <iostream>
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
  return EXIT_SUCCESS;
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
    "secpass-nc " + SECPASS_VERSION + "\n" +
    "Usage: secpass-nc /path/to/dbfile [OPTIONS]\n" +
    "Options:\n" +
    "  -e\tExport secdb to kdbx file (requires path)" +
    "  -i\tImport secdb from kdbx file (requires path)" +
    "  -t\tLock interface after given timeout in s" +
    "\n";

  return;
}
