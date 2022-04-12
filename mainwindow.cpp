#include <vector>
#include "account.h"
#include "mainwindow.h"
#include "logindialog.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ns(new netServices)
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
        string email = loginD.get_email(); //"gauss@integralces.net";
        string password = loginD.get_password(); //"integralces";
        ns->get_access(email, password);
    }
}

