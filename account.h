#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include "transfer.h"

using namespace std;

class account
{
public:
    account(const string& uid, const string& aid);
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
    vector<transfer> transfers;

    string print_balance();
    string print_transfers();
    string print_account();
};

#endif // ACCOUNT_H
