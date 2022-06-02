#ifndef NETSERVICES_H
#define NETSERVICES_H

#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "oauth2.h"
#include "account.h"

class netServices : public QObject {
Q_OBJECT

private:
    QNetworkAccessManager *netManager;

public:
    explicit netServices(QObject *parent = nullptr);
    ~netServices();
    Oauth2 oauth2;
    std::vector<account> accounts;
    int index_current_acc;
    string comma_list;

private slots:
    void get_accounts_reply(QNetworkReply*);
    void get_account_balance_reply(QNetworkReply*);
    void get_account_transfers_reply(QNetworkReply*);
    void get_unknown_accounts_reply(QNetworkReply*);

public slots:
    void get_accounts();
    void get_account_balance();
    void get_account_transfers();
    void get_unknown_accounts();

signals:
    void has_accounts();
    void has_balance();
    void has_transfers();
    void has_all_data();
    void network_error();
};

#endif // NETSERVICES_H
