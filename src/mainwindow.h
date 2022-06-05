#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netservices.h"
#include "logindialog.h"
#include "transferdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    netServices ns;
    LoginDialog loginD;
    TransferDialog transD;
    komunitin_settings kSettings;

private:
    Ui::MainWindow *ui;
    bool firstTimeShown;
    void prepare_machine();
    void showEvent(QShowEvent*);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);

public slots:
    void show_accounts_data();
    void show_account_balance();
    void show_account_transfers();
    void ask_for_new_auth();
    void try_authorization();
    void authorization_error();
    void check_account();
    void confirm_transfer();

private slots:
    void changeAccount(int);
    void on_actionQuit_triggered();
    void on_actionNew_transaction_triggered();
    void on_actionNew_User_triggered();

signals:
    void window_shown();
    void change_account();
    void new_user();
    void new_transfer();

};
#endif // MAINWINDOW_H
