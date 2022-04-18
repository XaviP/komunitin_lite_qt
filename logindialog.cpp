#include "logindialog.h"
#include "./ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
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

void LoginDialog::pushButtonLogin_clicked()
{
    if (get_email().empty() || get_password().empty()) {
        qDebug() << "Empty user or password";
    } else {
        ui->pushButtonLogin->setEnabled(false);
        ui->lineEditEmail->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->labelError->setText("Checking autorization...");
        emit send_authorization();
    }
}



