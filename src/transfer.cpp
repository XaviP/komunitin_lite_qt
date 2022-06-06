#include <sstream>
#include <iomanip>
#include <string>
#include "transfer.h"

transfer::transfer(const QString& trans_id)
    : id(trans_id)
{}

int transfer::get_factor() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }
    return factor;
}

QString transfer::print_amount() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(currency.decimals) << (float(amount)/float(get_factor()));
    return QString::fromStdString(stream.str() + " " + currency.plural.toStdString());
}

QString transfer::print_transfer() {
    return "from " + payer_account_code + " to " + payee_account_code +
           ": " + print_amount() + "\n";
}
