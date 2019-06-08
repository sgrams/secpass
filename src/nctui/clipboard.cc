/* src/nctui/clipboard.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "clipboard.h"

static pid_t proc = 0;
int saved_stdout  = -1;
static bool closer = false;

void
Clipboard::copy (string to_cpy)
{
  int pipefd[2];

  if (proc > 0) {
    return;
  }

  pipe (pipefd);

  saved_stdout = dup (STDOUT_FILENO);
  proc = fork ();

  if (proc < 0) {
    return;
  } else if (proc == 0) {

    close (pipefd[1]);
    dup2 (pipefd[0], STDIN_FILENO);
    close (pipefd[0]);

    execlp ("xsel", "xsel", "--clipboard", "--input", NULL);
    execlp ("xclip", "xclip", "-selection", "clipboard", "-in", NULL);

    fprintf (stdout, "%s", to_cpy.c_str ());
  } else {
    wait (NULL);
  }

  close (pipefd[0]);
  dup2 (pipefd[1], STDOUT_FILENO);
  close (pipefd[1]);

  if (!closer) {
    atexit (clipclose);
  }

  return;
}

void
Clipboard::clipclose ()
{
  if (!proc || saved_stdout < 0) {
    fflush (stdout);
    dup2 (saved_stdout, STDOUT_FILENO);
    close (saved_stdout);

    waitpid (proc, NULL, 0);
    proc = 0;
    saved_stdout = -1;
  }
}
