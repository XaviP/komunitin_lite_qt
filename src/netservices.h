#ifndef NETSERVICES_H
#define NETSERVICES_H

#include <vector>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "oauth2.h"
#include "account.h"
#include "transfer.h"

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
    QString comma_list;
    transfer* newTrans;

private slots:
    void get_accounts_reply(QNetworkReply*);
    void get_account_balance_reply(QNetworkReply*);
    void get_account_transfers_reply(QNetworkReply*);
    void get_unknown_accounts_reply(QNetworkReply*);
    void get_check_account_reply(QNetworkReply*);
    void post_new_transfer_reply(QNetworkReply*);

public slots:
    void get_accounts();
    void get_account_balance();
    void get_account_transfers();
    void get_unknown_accounts();
    void get_check_account(const QString&, const QString&);
    void post_new_transfer();

signals:
    void has_accounts();
    void has_balance();
    void has_transfers();
    void has_all_data();
    void network_error();
    void confirm_transfer();
    void transfer_done();
};

#endif // NETSERVICES_H
