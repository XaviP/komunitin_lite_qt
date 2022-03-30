#include <QDebug>
#include <string>
#include "account.h"

account::account(std::string uid, std::string mid) : user_id(uid), member_id(mid)
{}

void account::print_account() {
    qDebug() << QString::fromStdString(this->account_id);
}
