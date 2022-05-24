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
    QString access_token;
    QString refresh_token;
    void prepare_request(QNetworkRequest&);

public slots:
    void get_access_reply(QNetworkReply*);
    void get_accounts_reply(QNetworkReply*);
    void get_account_balance_reply(QNetworkReply*);
    void get_account_transfers_reply(QNetworkReply*);
    void get_unknown_accounts_reply(QNetworkReply*);

public:
    explicit netServices(QObject *parent = nullptr);
    ~netServices();
    bool hasAccess;
    std::vector<account> accounts;
    int index_current_acc;
    string comma_list;
    void get_access(const std::string&, const std::string&);
    void get_accounts();
    void get_account_balance();
    void get_account_transfers();
    void get_unknown_accounts();

signals:
    void has_access();
    void has_accounts();
    void has_balance();
    void has_transfers();
    void has_all_data();
    void error_auth();
    void network_error();
};

#endif // NETSERVICES_H
