#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

using namespace std;

class account
{
public:
    account(string uid, string mid);
    string user_id;
    string member_id;
    string member_name;
    string member_image;
    string account_code;
    string account_link;
    string group_id;
    string group_code;
    float balance;
    string currency_id;
    string currency_name;
    string currency_plural;
    string currency_symbol;
    int currency_decimals;
};

#endif // ACCOUNT_H
