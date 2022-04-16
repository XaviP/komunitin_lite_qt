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
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    std::string get_email();
    std::string get_password();
    Ui::LoginDialog *ui;

private:


public slots:
    void pushButtonLogin_clicked();
//    void authentication_reply(bool);
};

#endif // LOGINDIALOG_H
