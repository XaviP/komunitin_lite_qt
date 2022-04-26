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
    void get_account_balance(account*);
    void get_account_transfers(account*);
    void get_unknown_accounts(account*, const std::string&);
    void get_call(const QString&, QNetworkReply*&);

public slots:
    void get_access_reply(QNetworkReply*);

public:
    explicit netServices(QObject *parent = nullptr);
    ~netServices();
    bool hasAccess;
    void get_access(const std::string&, const std::string&);
    void get_accounts(std::vector<account>&);
    void get_account_data(account*);

signals:
    void access_reply(bool error);
    void accounts_reply(bool error);
    void account_data_reply(bool error);

};

#endif // NETSERVICES_H
