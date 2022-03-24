#ifndef TRANSFER_H
#define TRANSFER_H

#include <string>

using namespace std;

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
    string currency_id;
    string currency_name;
    string currency_plural;
    string currency_symbol;
    string currency_decimals;
};

#endif // TRANSFER_H
