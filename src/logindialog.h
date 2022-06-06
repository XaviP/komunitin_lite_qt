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
    QString get_email();
    QString get_password();
    Ui::LoginDialog *ui;

private:


private slots:
    void pushButtonLogin_clicked();

signals:
    void send_authorization();
};

#endif // LOGINDIALOG_H
