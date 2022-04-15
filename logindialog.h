#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "netservices.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(netServices* ns, QWidget *parent = nullptr);
    ~LoginDialog();
    std::string get_email();
    std::string get_password();

private:
    Ui::LoginDialog *ui;
    netServices* ns;

public slots:
    void pushButtonLogin_clicked();
    void authentication_reply(error_reply);
};

#endif // LOGINDIALOG_H
