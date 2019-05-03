/* src/qtgui/mainwindow.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * MainWindow constructor
 * @param  QWidget *parent
 * @retval MainWindow
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
MainWindow::MainWindow (QWidget *parent) :
  QMainWindow (parent),
  ui (new Ui::MainWindow)
{
  ui->setupUi (this);
}

/*!
 * MainWindow destructor
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
MainWindow::~MainWindow ()
{
  delete ui;
}
