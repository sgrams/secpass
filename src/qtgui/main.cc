/* src/qtgui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <QApplication>
#include "mainwindow.h"

/*!
 * Main function
 * @param  int  argc
 * @param  char *argv[]
 * @retval int
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  MainWindow w;
  w.show ();

  return app.exec ();
}
