#include <string>
#include "account.h"

account::account(const std::string& uid, const std::string& mid)
    : user_id(uid), member_id(mid)
{}

string account::print_balance() {
    int factor = 1;
    for (int i=0; i<currency.decimals; i++) { factor *= 10; }
    return std::to_string(float(balance)/float(factor)) +
           " " + currency.plural;
}

string account::print_transfers() {
    string out = "";
    for (int i=0; i<(int)transfers.size(); i++) {
        out += transfers[i].print_transfer();
    }
    return out;
}

string account::print_account() {
    string out = "";

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
    out += "currency decimals: " + std::to_string(currency.decimals) + "\n";
    out += "transfers :\n";
    out += print_transfers();

    return out;
 }
