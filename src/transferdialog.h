#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include "transfer.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class TransferDialog; }
QT_END_NAMESPACE

class TransferDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TransferDialog(QWidget *parent = nullptr);
    ~TransferDialog();
    Ui::TransferDialog *ui;

public slots:
    void confirm_transfer(transfer*);
    void on_cancelButton_clicked();

private slots:
    void on_continueButton_clicked();


signals:
    void check_account();
    void send_transfer();
    void exit_s3();

};

#endif // TRANSFERDIALOG_H
