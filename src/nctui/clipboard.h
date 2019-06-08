/* src/nctui/clipboard.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_NCTUI_CLIPBOARD_H
#define _SECPASS_NCTUI_CLIPBOARD_H
#include <string>
using namespace std;

namespace Clipboard {
  void copy (string to_cpy);
  void clipclose ();
};
#endif // _SECPASS_NCTUI_CLIPBOARD_H
