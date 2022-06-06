#include <QMessageBox>
#include <QRegularExpression>

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

void TransferDialog::on_cancelButton_clicked() {
    ui->fromAccountLineEdit->setText("");
    ui->amountLineEdit->setText("");
    ui->conceptPlainTextEdit->setPlainText("");
    ui->fromAccountLineEdit->setEnabled(true);
    ui->amountLineEdit->setEnabled(true);
    ui->conceptPlainTextEdit->setEnabled(true);
    ui->continueButton->setEnabled(true);
    close();
    emit exit_s3();
}

void TransferDialog::on_continueButton_clicked() {
    ui->fromAccountLineEdit->setText(ui->fromAccountLineEdit->text().trimmed());
    ui->amountLineEdit->setText(ui->amountLineEdit->text().trimmed().replace(",","."));

    static const QRegularExpression re1("^[A-Za-z0-9]{4}[0-9]{4}$");
    static const QRegularExpression re2("^[0-9]+([.][0-9]+)?$");

    if (!re1.match(ui->fromAccountLineEdit->text()).hasMatch()) {
        ui->errorLabel->setText(tr("Invalid account code."));
        return;
    } else if (!re2.match(ui->amountLineEdit->text()).hasMatch() || ui->amountLineEdit->text().toInt() == 0 ) {
        ui->errorLabel->setText(tr("Invalid amount."));
        return;
    } else if (ui->conceptPlainTextEdit->toPlainText().isEmpty()) {
        ui->errorLabel->setText(tr("Concept cannot be empty."));
        return;
    }
    ui->errorLabel->setText(tr("Checking account..."));
    ui->fromAccountLineEdit->setEnabled(false);
    ui->amountLineEdit->setEnabled(false);
    ui->conceptPlainTextEdit->setEnabled(false);
    ui->continueButton->setEnabled(false);
    emit check_account();
}

void TransferDialog::confirm_transfer(transfer* newTrans) {
    ui->errorLabel->setText(tr(""));
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("Confirm new transfer"));
    QString message =
        tr("From") + " " + QString::fromStdString(newTrans->payer_account_code) + " " +
        tr("to") + " " + QString::fromStdString(newTrans->payee_account_code) + "\n" +
        tr("Amount") + " :" + QString::fromStdString(newTrans->print_amount()) + "\n" +
        tr("Concept") + ": " + QString::fromStdString(newTrans->meta);
    msgBox->setText(message);
    msgBox->setInformativeText("Do you confirm to send this transfer?");
    QPushButton *cancelButton = msgBox->addButton(QMessageBox::Abort);
    QPushButton *confirmButton = msgBox->addButton(tr("Confirm"), QMessageBox::ActionRole);

    msgBox->exec();

    if (msgBox->clickedButton() == confirmButton) {
        delete msgBox;
        emit send_transfer();
    } else if (msgBox->clickedButton() == cancelButton) {
        delete msgBox;
        on_cancelButton_clicked();
    }
}




