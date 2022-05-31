#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

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
signals:

};

#endif // TRANSFERDIALOG_H
