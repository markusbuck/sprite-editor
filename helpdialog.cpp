#include "helpdialog.h"
#include "ui_helpdialog.h"

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
