#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    netServices *ns;
    std::vector<account> accounts;
};
#endif // MAINWINDOW_H
