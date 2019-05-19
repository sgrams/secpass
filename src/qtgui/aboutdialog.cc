/* src/qtgui/aboutdialog.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

/*!
 * AboutDialog constructor
 * @param  QWidget *parent
 * @retval AboutDialog
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
AboutDialog::AboutDialog (QWidget *parent) :
  QDialog (parent),
  ui (new Ui::AboutDialog)
{
  ui->setupUi (this);
  connect (ui->buttonBox, SIGNAL (rejected ()), SLOT (close ()));
}


/*!
 * MainWindow destructor
 *
 * @author Stanisław Grams
 * @date   2019/05/18
 */
AboutDialog::~AboutDialog ()
{
  delete ui;
}
