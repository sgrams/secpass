/* src/qtgui/mainwindow.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_MAINWINDOW_H
#define _SECPASS_MAINWINDOW_H
#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow (QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  // on_action*_triggered bindings
  void on_actionHomepage_triggered ();

private slots:
  void showAboutDialog ();
  void showSettingsDialog ();
  void showAboutQtMessageBox ();
};

#endif // _SECPASS_MAINWINDOW_H
