#include <string>
#include "transfer.h"

transfer::transfer(const string& trans_id)
    : id(trans_id)
{}

string transfer::print_amount() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }

    return std::to_string(float(amount)/float(factor)) +
           " " + currency.plural;
}

string transfer::print_transfer() {
    return "from " + payer_account_code + " to " + payee_account_code +
           ": " + print_amount() + "\n";
}
