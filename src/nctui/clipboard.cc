/* src/nctui/clipboard.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "clipboard.h"

int Clipboard::copy (const char *secret)
{
  FILE *fp;

  // copy to clipboard with xclip
  fp = popen ("xclip -i", "w");
  if (fwrite (secret, 1, strlen (secret), fp) != strlen (secret)) {
    return -2;
  }
  pclose (fp);
  return 0;
}
