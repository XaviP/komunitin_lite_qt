#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include "logindialog.h"
#include "netservices.h"
#include "account.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void authenticate();
    void get_user_data();

private:
    Ui::MainWindow *ui;
    netServices *ns;
    std::vector<account> accounts;

public slots:
    void login_dialog_close();
};
#endif // MAINWINDOW_H
