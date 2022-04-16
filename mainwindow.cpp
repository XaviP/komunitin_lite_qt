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
      accounts()
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ns, SIGNAL(access_reply(bool)),
            this, SLOT(authenticate_reply(bool))); //, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ns;
}

void MainWindow::authenticate() {
    LoginDialog loginD(this);
    loginD.exec();
    ns->get_access(loginD.get_email(), loginD.get_password());
}

void MainWindow::authenticate_reply(bool error) {
    if (!error) {
        qDebug() << "I have access.";
        get_user_data();
    }
    else {
        LoginDialog loginD(this);
        loginD.ui->labelError->setText("Authentication error.");
        loginD.exec();
        ns->get_access(loginD.get_email(), loginD.get_password());
    }
}

void MainWindow::get_user_data() {
    ns->get_accounts(accounts);
    for (int i=0; i  < (int)accounts.size(); i++) {
        ui->accountComboBox->addItem(QString::fromStdString(accounts[i].account_code));
    }
    ui->nameInsertLabel->setText(QString::fromStdString(accounts[0].member_name));
    ui->balanceInsertLabel->setText(QString::fromStdString(accounts[0].print_balance()));
    qDebug() << QString::fromStdString(accounts[0].account_code);
    ns->get_account_transfers(&accounts[0]);
    qDebug() << QString::fromStdString(accounts[0].print_account());
}

