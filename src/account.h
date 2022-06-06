#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <vector>
#include "transfer.h"

using namespace std;

class account
{
public:
    account(const QString& uid, const QString& aid);
    QString user_id;
    QString member_id;
    QString member_name;
    QString member_image;
    QString account_id;
    QString account_code;
    QString account_link;
    QString group_id;
    QString group_code;
    int balance;
    Currency currency;
    vector<transfer> transfers;

    QString print_balance();
    QString print_transfers();
    QString print_account();
};

#endif // ACCOUNT_H
