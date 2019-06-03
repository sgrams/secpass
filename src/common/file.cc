/* src/common/file.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#include "file.h"

File::File (string filepath)
{
  this->path = filepath;
}
File::~File ()
{
}
