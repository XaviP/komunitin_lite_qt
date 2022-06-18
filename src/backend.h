#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtQml/qqml.h>
#include <QCloseEvent>
#include "netservices.h"

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    Backend(QObject *parent = nullptr);
    ~Backend();
    netServices ns;
    komunitin_settings kSettings;

private:
    void prepare_machine();
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);

public slots:
    void show_accounts_data();
    void show_account_balance();
    void show_account_transfers();
    void ask_for_new_auth();
    void try_authorization(const QString, const QString);
    void authorization_error();
    void check_account();
    void confirm_transfer();
    void transfer_done();

private slots:
    void appIsReady();
    void changeAccount(int);
    void on_actionNew_transaction_triggered();
    void on_actionNew_User_triggered();
    void on_actionReload_transfer_triggered();

signals:
    void app_is_ready();
    void show_login_dialog();
    void send_authorization();
    void reload_transfers();
    void new_user();
    void new_transfer();

};
#endif // MAINWINDOW_H
