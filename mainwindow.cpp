#include <vector>
#include "account.h"
#include "mainwindow.h"
#include "logindialog.h"
#include "./ui_mainwindow.h"
#include "ui_logindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ns(new netServices),
      accounts(),
      loginD(new LoginDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(loginD, SIGNAL(finished(int)),
            this, SLOT(get_user_data(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ns;
}

void MainWindow::authenticate() {
    QObject::connect(loginD, SIGNAL(send_authorization()),
            this, SLOT(try_authorization()), Qt::SingleShotConnection);
    loginD->open();
}

void MainWindow::try_authorization() {
    QObject::connect(ns, SIGNAL(access_reply(bool)),
            this, SLOT(authorization_reply(bool)), Qt::SingleShotConnection);
    ns->get_access(loginD->get_email(), loginD->get_password());
}

void MainWindow::authorization_reply(bool error) {
    if (!error) {
        loginD->accept();
    }
    else {
        loginD->ui->labelError->setText("Authentication error.");
        loginD->ui->pushButtonLogin->setEnabled(true);
        loginD->ui->lineEditEmail->setEnabled(true);
        loginD->ui->lineEditPassword->setEnabled(true);
        QObject::connect(loginD, SIGNAL(send_authorization()),
                this, SLOT(try_authorization()), Qt::SingleShotConnection);
    }
}

void MainWindow::get_user_data(int) {
    ns->get_accounts(accounts);
    for (int i=0; i  < (int)accounts.size(); i++) {
        ui->accountComboBox->addItem(QString::fromStdString(accounts[i].account_code));
    }
    ui->nameInsertLabel->setText(QString::fromStdString(accounts[0].member_name));
//    ns->get_account_balance(&accounts[0]);
//    ui->balanceInsertLabel->setText(QString::fromStdString(accounts[0].print_balance()));
//    ns->get_account_transfers(&accounts[0]);
//    qDebug() << QString::fromStdString(accounts[0].print_transfers());
}

