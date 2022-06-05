#include <QMessageBox>

#include "transfer.h"
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

void TransferDialog::on_continueButton_clicked() {
    ui->fromAccountLineEdit->setEnabled(false);
    ui->amountLineEdit->setEnabled(false);
    ui->conceptPlainTextEdit->setEnabled(false);
    ui->continueButton->setEnabled(false);
    emit check_account();
}

void TransferDialog::confirm_transfer(transfer* newTrans) {
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("Confirm new transfer"));
    QString message =
        tr("From") + " " + QString::fromStdString(newTrans->payer_account_code) + " " +
        tr("to") + " " + QString::fromStdString(newTrans->payee_account_code) + "\n" +
        tr("Amount") + " :" + QString::fromStdString(newTrans->print_amount()) + "\n" +
        tr("Concept") + ": " + QString::fromStdString(newTrans->meta) +
        "\n (This is under development, it won't send anything)";
    msgBox->setText(message);
    msgBox->setInformativeText("Do you confirm to send this transfer?");
    msgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Save);
    int ret = msgBox->exec();
}

