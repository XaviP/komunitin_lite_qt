#include "logindialog.h"
#include "./ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

std::string LoginDialog::get_email() {
    return ui->lineEditEmail->text().toStdString();
}

std::string LoginDialog::get_password() {
    return ui->lineEditPassword->text().toStdString();
}
