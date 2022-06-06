#include <QRegularExpression>

#include "logindialog.h"
#include "./ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    connect(ui->pushButtonLogin, SIGNAL(clicked()),
            this, SLOT(pushButtonLogin_clicked()), Qt::UniqueConnection);
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

void LoginDialog::pushButtonLogin_clicked() {

    static const QRegularExpression reEmail("^[A-Za-z-\\.]+@([A-Za-z-\\.])+[A-Za-z-]{2,4}$");

    if (!reEmail.match(ui->lineEditEmail->text()).hasMatch()) {
        ui->labelError->setText(tr("Invalid email"));
        return;
    }
    if (get_password().empty()) {
        ui->labelError->setText(tr("Empty password"));
        return;
    }

    ui->pushButtonLogin->setEnabled(false);
    ui->lineEditEmail->setEnabled(false);
    ui->lineEditPassword->setEnabled(false);
    ui->labelError->setText(tr("Checking autorization..."));
    emit send_authorization();

}



