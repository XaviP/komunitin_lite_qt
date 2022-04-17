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
    QObject::connect(loginD, SIGNAL(send_authorization()),
            this, SLOT(try_authorization()), Qt::QueuedConnection);
    QObject::connect(ns, SIGNAL(access_reply(bool)),
            this, SLOT(authorization_reply(bool)));
    QObject::connect(this, SIGNAL(try_get_data()),
            this, SLOT(get_user_data()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ns;
}

void MainWindow::authenticate() {
    loginD->exec();
}

void MainWindow::try_authorization() {
    ns->get_access(loginD->get_email(), loginD->get_password());
}

void MainWindow::authorization_reply(bool error) {
    if (!error) {
        loginD->accept();
        qDebug() << "have access";
        //emit try_get_data();
    }
    else {
        loginD->ui->labelError->setText("Authentication error.");
        loginD->ui->pushButtonLogin->setEnabled(true);
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

