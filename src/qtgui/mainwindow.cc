/* src/qtgui/mainwindow.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDateTime>
#include <QDialog>
#include <QUrl>
#include <QList>
#include <QString>
#include <QStringListModel>
#include <QVector>
#include <string>

#include "../common/secpass.h"
#include "../common/bridge.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "settingsdialog.h"

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
  // set up ui from *.ui files
  ui->setupUi (this);

  // connect main signals
  connect (ui->actionAbout, SIGNAL (triggered ()), SLOT (showAboutDialog ()));
  connect (ui->actionAboutQt, SIGNAL (triggered ()), SLOT (showAboutQtMessageBox ()));
  connect (ui->actionSettings, SIGNAL (triggered ()), SLOT (showSettingsDialog ()));

  // set title
  setWindowTitle (QString ("secpass %1").arg(SECPASS_VERSION));
}

/*!
 * Shows "About secpass" QDialog
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
void
MainWindow::showAboutDialog ()
{
  auto *aboutDialog = new AboutDialog (this);
  aboutDialog->open ();
}

/*!
 * Shows settings QDialog
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
void
MainWindow::showSettingsDialog ()
{
  auto *settingsDialog = new SettingsDialog (this);
  settingsDialog->open ();
}

/*!
 * Shows "About Qt" QMessageBox
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
void
MainWindow::showAboutQtMessageBox ()
{
  QMessageBox::aboutQt (this, tr ("About Qt"));
}


/*!
 * Links actionHomepage to secpass homepage
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
void
MainWindow::on_actionHomepage_triggered ()
{
  bool response = QDesktopServices::openUrl (QUrl (SECPASS_HOMEPAGE, QUrl::TolerantMode));
  if (!response) {
    QMessageBox::warning (this, tr ("Error"), tr ("Failed to open secpass homepage"), QMessageBox::Close);
  }
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
