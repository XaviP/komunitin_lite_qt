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
    QNetworkReply* get_call(QString);
public:
    bool hasAccess;
    explicit netServices(QObject *parent = nullptr);
    void get_access(std::string, std::string);
    std::vector<account> get_accounts();
    void get_account_balance(account*);
};

#endif // NETSERVICES_H
