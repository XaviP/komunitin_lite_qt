#include <QSettings>
#include <QTimer>
#include <QCloseEvent>
#include "backend.h"
#include "transfer.h"

Backend::Backend(QObject *parent)
    : QObject(parent),
      ns(),
      kSettings()
{
    loadSettings();
    ns.oauth2.kSettingsP = &kSettings;

    // wait 100ms (non-blocking) for QApplication::exec()
    // to get the state machine really started.
    QTimer::singleShot(100, this, SLOT(appIsReady()));
}

Backend::~Backend()
{}

void Backend::test_qml() {
    qDebug() << "connected to QML";
}

void Backend::appIsReady() {
    // transite to 2nd state in state machine
    emit app_is_ready();
}

void Backend::ask_for_new_auth() {
    // open new_user dialog
    // loginD.open();
}

void Backend::try_authorization() {
//    ui->statusbar->showMessage(tr("Trying authorization..."));
//    kSettings.user_email = loginD.get_email();
//    ns.oauth2.get_access(loginD.get_email(), loginD.get_password());
}

void Backend::authorization_error() {
//    if (loginD.isVisible()) {
//        loginD.ui->labelError->setText(tr("Authentication error."));
//        loginD.ui->pushButtonLogin->setEnabled(true);
//        loginD.ui->lineEditEmail->setEnabled(true);
//        loginD.ui->lineEditPassword->setEnabled(true);
//        ui->statusbar->showMessage(tr("Enter credentials to try again."));
//    } else {
//        loginD.open();
//    }
}

void Backend::show_accounts_data() {
//    if (loginD.isVisible()) {
//        loginD.accept();
//    }
//    if (ui->accountComboBox->count() == 0) {
//        for (int i=0; i  < (int)ns.accounts.size(); i++) {
//            ui->accountComboBox->addItem(ns.accounts[i].account_code);
//        }
//    }
//    ui->accountComboBox->setCurrentIndex(ns.index_current_acc);
//    ui->nameInsertLabel->setText(tr("Name") + ": " + ns.accounts[ns.index_current_acc].member_name);
//    ui->statusbar->showMessage(tr("Loading balance data..."));
}

void Backend::show_account_balance() {
//    ui->balanceInsertLabel->setText(tr("Balance") + ": " + ns.accounts[ns.index_current_acc].print_balance());
//    ui->statusbar->showMessage(tr("Loading transfers data..."));
}

void Backend::show_account_transfers() {

//    ui->statusbar->showMessage(tr("All data is loaded."));
//    int rows = ns.accounts[ns.index_current_acc].transfers.size();
//    ui->tableWidget->setRowCount(rows);
//    for (int row = 0; row < rows; row++) {
//        transfer trans = ns.accounts[ns.index_current_acc].transfers[row];
//        QString data[6] = {
//            trans.created.left(10),
//            trans.meta,
//            trans.payer_account_code,
//            trans.payee_account_code,
//            trans.print_amount(),
//            trans.state
//        };
//        for (int column = 0; column < 6; column++) {
//            QTableWidgetItem * cellItem = new QTableWidgetItem();
//            cellItem->setData(Qt::DisplayRole, data[column]);
//            ui->tableWidget->setItem( row, column, cellItem );
//        }
//    }
}

void Backend::changeAccount(int index) {
//    if (index != ns.index_current_acc) {
//        ns.index_current_acc = index;
//        ui->tableWidget->clearContents();
//        ui->tableWidget->setRowCount(1);
//        emit change_account();
//    }
}

void Backend::on_actionReload_transfer_triggered() {
//    ui->tableWidget->clearContents();
//    ui->tableWidget->setRowCount(1);
//    ui->statusbar->showMessage(tr("Reloading transfers..."));
//    emit change_account();
}

void Backend::on_actionNew_User_triggered() {
//    kSettings = {"","","",0,0};
//    ns.accounts.clear();

//    loginD.ui->labelError->setText("");
//    loginD.ui->pushButtonLogin->setEnabled(true);
//    loginD.ui->lineEditEmail->setText("");
//    loginD.ui->lineEditEmail->setEnabled(true);
//    loginD.ui->lineEditPassword->setText("");
//    loginD.ui->lineEditPassword->setEnabled(true);

//    ui->nameInsertLabel->setText("");
//    ui->accountComboBox->blockSignals(true);
//    ui->accountComboBox->clear();
//    ui->accountComboBox->blockSignals(false);
//    ui->balanceInsertLabel->setText("");
//    ui->tableWidget->clearContents();
//    ui->tableWidget->setRowCount(1);

//    emit new_user();
}

void Backend::on_actionNew_transaction_triggered() {
//    transD.current_group = ns.accounts[ns.index_current_acc].group_code;
//    transD.setWindowTitle(tr("New transfer in group") + " " + transD.current_group);
//    transD.ui->toAccountLabel->setText(ns.accounts[ns.index_current_acc].account_code);
//    transD.ui->currencyLabel->setText(ns.accounts[ns.index_current_acc].currency.plural);
//    transD.open();
//    ui->statusbar->showMessage(tr("New transfer"));
//    emit new_transfer();
}

void Backend::check_account() {
//    QString fromAccount = transD.ui->fromAccountLineEdit->text();
//    QString groupCode = ns.accounts[ns.index_current_acc].group_code;
//    ns.get_check_account(groupCode, fromAccount);
}

void Backend::confirm_transfer() {
//    ns.newTrans->amount =  int(transD.ui->amountLineEdit->text().toFloat() * ns.newTrans->get_factor());
//    ns.newTrans->meta = transD.ui->conceptPlainTextEdit->toPlainText();
//    transD.confirm_transfer(ns.newTrans);
}

void Backend::transfer_done() {
//    ui->statusbar->showMessage(tr("New transfer is done"));
//    transD.ui->fromAccountLineEdit->setText("");
//    transD.ui->amountLineEdit->setText("");
//    transD.ui->conceptPlainTextEdit->setPlainText("");
//    transD.ui->fromAccountLineEdit->setEnabled(true);
//    transD.ui->amountLineEdit->setEnabled(true);
//    transD.ui->conceptPlainTextEdit->setEnabled(true);
//    transD.ui->continueButton->setEnabled(true);
//    transD.close();
}

void Backend::on_actionQuit_triggered() {
//    QApplication::quit();
}

void Backend::loadSettings()
{
  QSettings settings;
  kSettings.user_email = settings.value("user_email", "").toString();
  kSettings.access_token = settings.value("access_token", "").toString();
  kSettings.refresh_token = settings.value("refresh_token", "").toString();
  kSettings.created = settings.value("created", 0).toInt();
  kSettings.expires_in = settings.value("expires_in", 3600).toInt();
}

void Backend::saveSettings()
{
    QSettings settings;
    settings.setValue("user_email", kSettings.user_email);
    settings.setValue("access_token", kSettings.access_token);
    settings.setValue("refresh_token", kSettings.refresh_token);
    settings.setValue("created", kSettings.created);
    settings.setValue("expires_in", kSettings.expires_in);
}

void Backend::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}


