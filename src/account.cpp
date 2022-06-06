#include <sstream>
#include <iomanip>
#include <string>
#include "account.h"

account::account(const QString& uid, const QString& mid)
    : user_id(uid), member_id(mid)
{}

QString account::print_balance() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(currency.decimals) << (float(balance)/float(factor));
    return QString::fromStdString(stream.str() + " " + currency.plural.toStdString());
}

QString account::print_transfers() {
    QString out = "";
    for (int i=0; i<(int)transfers.size(); i++) {
        out += transfers[i].print_transfer();
    }
    return out;
}

QString account::print_account() {
    QString out = "";

    out += "user id: " + user_id + "\n";
    out += "member id: " + member_id + "\n";
    out += "member name: " + member_name + "\n";
    out += "member image: " + member_image + "\n";
    out += "account id: " + account_id + "\n";
    out += "account code: " + account_code + "\n";
    out += "account link: " + account_link + "\n";
    out += "group id: " + group_id + "\n";
    out += "group code: " + group_code + "\n";
    out += "balance: " + print_balance() + "\n";
    out += "currency id: " + currency.id + "\n";
    out += "currency name: " + currency.name + "\n";
    out += "currency plural: " + currency.plural + "\n";
    out += "currency symbol: " + currency.symbol + "\n";
    out += "currency decimals: " + QString::number(currency.decimals) + "\n";
    out += "transfers :\n";
    out += print_transfers();

    return out;
 }
