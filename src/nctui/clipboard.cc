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

static pid_t clipboard_process = 0;
static int saved_stdout = -1;
static bool registered_closer = false;


void Clipboard::cb_open ()
{
  int pipefd[2];

  if (clipboard_process > 0)
    return;

  if (pipe(pipefd) < 0) {
    exit (EXIT_FAILURE);
  }
  saved_stdout = dup(STDOUT_FILENO);
  if (saved_stdout < 0) {
    exit (EXIT_FAILURE);
  }

  clipboard_process = fork();

  if (clipboard_process == -1) {
    exit (EXIT_FAILURE);
  }

  if (!clipboard_process) {
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    execlp("xclip", "xclip", "-selection", "clipboard", "-in", NULL);
    execlp("xsel", "xsel", "--clipboard", "--input", NULL);
    execlp("pbcopy", "pbcopy", NULL);
    execlp("putclip", "putclip", "--dos", NULL);
  }
  close(pipefd[0]);
  dup2(pipefd[1], STDOUT_FILENO);
  close(pipefd[1]);

  if (!registered_closer) {
    atexit (cb_close);
    registered_closer = true;
  }
}

void Clipboard::cb_close ()
{
  if (!clipboard_process || saved_stdout < 0) {
    return;
  }

  fflush(stdout);
  dup2(saved_stdout, STDOUT_FILENO);
  close(saved_stdout);
  waitpid(clipboard_process, NULL, 0);
  clipboard_process = 0;
  saved_stdout = -1;
}
