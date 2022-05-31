#include "transferdialog.h"
#include "./ui_transferdialog.h"

TransferDialog::TransferDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TransferDialog)
{
    ui->setupUi(this);
}

TransferDialog::~TransferDialog() {
    delete ui;
}
