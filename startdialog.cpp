#include "startdialog.h"
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartDialog){
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &StartDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void StartDialog::onAccepted(){
    emit onProjectAccepted(ui->width->value(), ui->height->value(), ui->projectName->toPlainText());
}

StartDialog::~StartDialog(){
    delete ui;
}
