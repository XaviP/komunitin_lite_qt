#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include "transfer.h"

using namespace std;

class account
{
public:
    account(string uid, string aid);
    string user_id;
    string member_id;
    string member_name;
    string member_image;
    string account_id;
    string account_code;
    string account_link;
    string group_id;
    string group_code;
    int balance;
    Currency currency;
    std::vector<transfer> transfers;

    string print_balance() {
        int factor = 1;
        for (int i=0; i<currency.decimals; i++) { factor *= 10; }
        return std::to_string(float(balance)/float(factor)) +
               " " + currency.plural;
    }

    string print_account() {
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

       for (int i=0; i<transfers.size(); i++) {
           out += transfers[i].print_transfer();
       }

       return out;
    }

};

#endif // ACCOUNT_H
