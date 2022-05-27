#include <sstream>
#include <iomanip>
#include <string>
#include "transfer.h"

transfer::transfer(const string& trans_id)
    : id(trans_id)
{}

string transfer::print_amount() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(currency.decimals) << (float(amount)/float(factor));
    return stream.str() + " " + currency.plural;
}

string transfer::print_transfer() {
    return "from " + payer_account_code + " to " + payee_account_code +
           ": " + print_amount() + "\n";
}
