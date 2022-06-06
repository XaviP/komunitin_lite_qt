#ifndef TRANSFER_H
#define TRANSFER_H

#include <QString>

using namespace std;

struct Currency {
    QString id;
    QString name;
    QString plural;
    QString symbol;
    int decimals;
};

class transfer
{
public:
    transfer(const QString& trans_id);
    QString id;
    int amount;
    QString meta;
    QString state;
    QString created;
    QString updated;
    QString payer_account_id;
    QString payer_account_code;
    QString payee_account_id;
    QString payee_account_code;
    Currency currency;

    int get_factor();
    QString print_amount();
    QString print_transfer();
};

#endif // TRANSFER_H
