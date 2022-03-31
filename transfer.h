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
    int amount;
    string meta;
    string state;
    string created;
    string updated;
    string payer_account_id;
    string payer_account_code;
    string payee_account_id;
    string payee_account_code;
    Currency currency;

    string print_amount() {
        int factor = 1;
        for (int i=0; i<currency.decimals; i++) { factor *= 10; }
        return std::to_string(float(amount)/float(factor)) + " " + currency.plural;
    }

    string print_transfer() {
        return "from " + payer_account_code + " to " + payee_account_code + ": " + print_amount() + "\n";
    }

};

#endif // TRANSFER_H
