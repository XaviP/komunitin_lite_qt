#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStateMachine>
#include <QHistoryState>
#include <QMainWindow>
#include "logindialog.h"
#include "netservices.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    netServices ns;
    komunitin_settings kSettings;
    LoginDialog loginD;
    QStateMachine machine;
    bool firstTimeShown;
    QState *s1;
    QHistoryState *s1H;
    QState *s10NoAccess;
    QState *s11CheckTokens;
    QState *s12HasAccess;
    QState *s13HasAccounts;
    QState *s14HasBalance;
    QState *s15HasTransfers;
    QState *s16HasAllData;
    QState *s2;
    QState *s20ShowDialog;
    QState *s21TryAuth;
    QState *s22HasAccess;
    void create_state_machine();
    void showEvent(QShowEvent*);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);


private slots:
    void ask_for_new_auth();
    void try_authorization();
    void authorization_error();
    void show_accounts_data();
    void show_account_balance();
    void show_account_transfers();
    void changeAccount(int);

    void on_actionQuit_triggered();
    void on_actionNew_transaction_triggered();
    void on_actionNew_User_triggered();

signals:
    void window_shown();
    void change_account();
    void new_user();

};
#endif // MAINWINDOW_H
