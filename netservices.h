#ifndef NETSERVICES_H
#define NETSERVICES_H

#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "account.h"

class netServices : public QObject {
Q_OBJECT

private:
    QNetworkAccessManager *netManager;
    QByteArray authHeaderValue;
    account* current_account;
    void prepare_request(QNetworkRequest&);
    void get_call(const QString&, QNetworkReply*&);

public slots:
    void get_access_reply(QNetworkReply*);
    void get_accounts_reply(QNetworkReply*);
    void get_account_balance_reply(QNetworkReply*);
    void get_account_transfers_reply(QNetworkReply*);
    void get_account_data(account*);

public:
    explicit netServices(QObject *parent = nullptr);
    ~netServices();
    bool hasAccess;
    void get_access(const std::string&, const std::string&);
    void get_accounts();
    void get_account_balance(account*);
    void get_account_transfers(account*);
    void get_unknown_accounts(account*, const std::string&);

signals:
//    void account_balance_data(account*);
//    void transfers_data(account*);
//    void unknown_accounts_data(account*);

    void access_reply(bool error);
    void accounts_reply(bool error, std::vector<account>);
    void account_balance_reply(bool error);
    void account_transfers_reply(bool error, std::string comma_list);
    void account_unknown_accounts_reply(bool error);
};

#endif // NETSERVICES_H
