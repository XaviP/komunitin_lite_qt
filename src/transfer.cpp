#include <sstream>
#include <iomanip>
#include <string>
#include "transfer.h"

transfer::transfer(const string& trans_id)
    : id(trans_id)
{}

int transfer::get_factor() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }
    return factor;
}

string transfer::print_amount() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(currency.decimals) << (float(amount)/float(get_factor()));
    return stream.str() + " " + currency.plural;
}

string transfer::print_transfer() {
    return "from " + payer_account_code + " to " + payee_account_code +
           ": " + print_amount() + "\n";
}
