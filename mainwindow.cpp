#include <vector>
#include "account.h"
#include "mainwindow.h"
#include "logindialog.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ns(new netServices),
      accounts()
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ns;
}

void MainWindow::authenticate() {
    while (!ns->hasAccess) {
        LoginDialog loginD(this);
        loginD.exec();
        ns->get_access(loginD.get_email(), loginD.get_password());
    }
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

