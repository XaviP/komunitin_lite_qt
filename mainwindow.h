#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStateMachine>
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
    LoginDialog loginD;
    QStateMachine machine;
    QState *state0NoAccess;
    QState *state1TryAccess;
    QState *state2HasAccess;
    QState *state3HasAccounts;
    QState *state4HasBalance;
    QState *state5HasTransfers;
    QState *state6HasAllData;
    void create_state_machine();
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);
    komunitin_settings kSettings;

private slots:
    void try_authorization();
    void authorization_error();
    void show_accounts_data();
    void show_account_balance();
    void show_account_transfers();
    void changeAccount(int);

signals:
    void change_account();

};
#endif // MAINWINDOW_H
