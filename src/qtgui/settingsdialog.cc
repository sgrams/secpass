/* src/qtgui/settingsdialog.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/*!
 * SettingsDialog constructor
 * @param  QWidget *parent
 * @retval SettingsDialog
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
SettingsDialog::SettingsDialog (QWidget *parent) :
  QDialog (parent),
  ui (new Ui::SettingsDialog)
{
  ui->setupUi (this);
  connect (ui->buttonBox, SIGNAL (rejected ()), SLOT (close ()));
}

/*!
 * SettingsDialog destructor
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
SettingsDialog::~SettingsDialog ()
{
  delete ui;
}
