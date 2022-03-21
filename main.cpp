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
    std::vector<account> accounts = ns->get_accounts();
    ns->get_account_balance(&accounts[0]);
    std::cout.precision(2);
    std::cout << accounts[0].account_code << ": " << accounts[0].balance << std::endl;
    return a.exec();
}
