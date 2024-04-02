#include "helpdialog.h"
#include "ui_helpdialog.h"

/*!
 * \brief HelpDialog::HelpDialog
 * \param parent
 *
 * Implements the methods defined in helpdialog.h
 *
 * Reviewed by Kevin Soto
 */
HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::reject);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
