#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include "logindialog.h"
#include "netservices.h"
#include "account.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void authenticate();

private:
    Ui::MainWindow *ui;
    netServices *ns;
    std::vector<account> accounts;
    LoginDialog* loginD;

private slots:
    void try_authorization();
    void authorization_reply(bool error);
    void get_user_data(int);
    void get_user_data_reply(bool error);
    void get_account_data_reply(bool error);

signals:
    void try_get_data();
};
#endif // MAINWINDOW_H
