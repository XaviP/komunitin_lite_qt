#ifndef TRANSFER_H
#define TRANSFER_H

#include <string>

using namespace std;

struct Currency {
    string id;
    string name;
    string plural;
    string symbol;
    int decimals;
};

class transfer
{
public:
    transfer(string trans_id);
    string id;
    float amount;
    string meta;
    string state;
    string created;
    string updated;
    string payer_account_id;
    string payer_account_code;
    string payee_account_id;
    string payee_account_code;
    Currency currency;
};

#endif // TRANSFER_H
