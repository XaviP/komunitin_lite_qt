#include "logindialog.h"
#include "./ui_logindialog.h"

LoginDialog::LoginDialog(netServices *ns, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonLogin, SIGNAL(clicked()), this,
            SLOT(pushButtonLogin_clicked()), Qt::DirectConnection);
    connect(ns, SIGNAL(access_reply(error_reply)),
            this, SLOT(authentication_reply(error_reply)), Qt::DirectConnection);
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

void LoginDialog::pushButtonLogin_clicked()
{
    if (get_email().empty() || get_password().empty()) {
        qDebug() << "Empty user or password";
    } else {
        ui->lineEditEmail->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->pushButtonLogin->setEnabled(false);
        qDebug() << "calling get_access...";
        ns->get_access(get_email(), get_password());
    }
}

void LoginDialog::authentication_reply(error_reply e) {
    qDebug() << "entering authentication_reply...";
    if (e.error) {
        ui->labelError->setText(QString::fromStdString(e.error_text));
        ui->lineEditEmail->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->pushButtonLogin->setEnabled(true);
    } else {
        this->accept();
    }
}

