#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

using namespace std;

struct Currency {
    string id;
    string name;
    string plural;
    string symbol;
    int decimals;
};

class account
{
public:
    account(string uid, string aid);
    void print_account();
    string user_id;
    string member_id;
    string member_name;
    string member_image;
    string account_id;
    string account_code;
    string account_link;
    string group_id;
    string group_code;
    float balance;
    Currency currency;
};

#endif // ACCOUNT_H
