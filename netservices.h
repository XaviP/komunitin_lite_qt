#ifndef NETSERVICES_H
#define NETSERVICES_H

#include <string>
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
    void get_call(QString, QNetworkReply*&);
public:
    bool hasAccess;
    explicit netServices(QObject *parent = nullptr);
    void get_access(std::string, std::string);
    void get_accounts(std::vector<account>&);
    void get_account_balance(account*);
    void get_account_transfers(account*);
};

#endif // NETSERVICES_H
