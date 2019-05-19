/* src/qtgui/settingsdialog.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_DIALOG_SETTINGS_H
#define _SECPASS_DIALOG_SETTINGS_H
#include <QDialog>

namespace Ui {
  class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget *parent = nullptr);
  ~SettingsDialog ();

private:
  Ui::SettingsDialog *ui;
};

#endif // _SECPASS_DIALOG_SETTINGS_H
