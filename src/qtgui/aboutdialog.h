/* src/qtgui/aboutdialog.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_DIALOG_ABOUT_H
#define _SECPASS_DIALOG_ABOUT_H
#include <QDialog>

namespace Ui {
  class AboutDialog;
}

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AboutDialog (QWidget *parent = nullptr);
  ~AboutDialog ();

private:
  Ui::AboutDialog *ui;
};

#endif // _SECPASS_DIALOG_ABOUT_H
