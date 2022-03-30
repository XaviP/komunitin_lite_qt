#include <string>
#include <iostream>
#include <QCoreApplication>
#include "netservices.h"
#include "account.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    netServices *ns = new netServices();
    std::string email = "";
    std::string password = "";
    while (!ns->hasAccess) {
        std::cout << "Email: ";
        std::cin >> email;
        std::cout << "Password: ";
        std::cin >> password;
        ns->get_access(email, password);
    }
    std::vector<account> accounts;
    ns->get_accounts(accounts);
    ns->get_account_balance(&accounts[0]);
    accounts[0].print_account();
    std::cout.precision(2);
    std::cout << accounts[0].account_code << ": " << accounts[0].balance << std::endl;
    ns->get_account_transfers(&accounts[0]);
//    for () {
//        std::cout << 12;
//    }
    return a.exec();
}
