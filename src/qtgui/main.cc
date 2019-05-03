/* src/qtgui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <QApplication>
#include "mainwindow.h"

int
main (int argc, char *argv[])
{
  QApplication a (argc, argv);
  MainWindow w;
  w.show ();

  return a.exec ();
}
