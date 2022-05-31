#include <QSettings>
#include <QTimer>
#include <QCloseEvent>
#include "mainwindow.h"
#include "transfer.h"
#include "./ui_mainwindow.h"
#include "./ui_logindialog.h"
#include "transferdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ns(),
      kSettings(),
      loginD(this),
      machine(),
      firstTimeShown(true)
{
    loadSettings();
    ns.oauth2.kSettingsP = &kSettings;
    create_state_machine();

    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->accountComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(changeAccount(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete s1;
    delete s1H;
    delete s10NoAccess;
    delete s11CheckTokens;
    delete s12HasAccess;
    delete s13HasAccounts;
    delete s14HasBalance;
    delete s15HasTransfers;
    delete s16HasAllData;
    delete s2;
    delete s20ShowDialog;
    delete s21TryAuth;
    delete s22HasAccess;
}

void MainWindow::showEvent(QShowEvent *e) {
   QMainWindow::showEvent(e);
   if (firstTimeShown == true) {
      firstTimeShown = false;

      // wait 100ms (non-blocking) for QApplication::exec()
      // to get the state machine really started.
      QEventLoop loop;
      QTimer::singleShot(100, &loop, SLOT(quit()));
      loop.exec();

      emit window_shown();
   }
}

void MainWindow::ask_for_new_auth() {
    loginD.open();
}

void MainWindow::try_authorization() {
    ui->statusbar->showMessage("Trying authorization...");
    kSettings.user_email = QString::fromStdString(loginD.get_email());
    ns.oauth2.get_access(loginD.get_email(), loginD.get_password());
}

void MainWindow::authorization_error() {
    if (loginD.isVisible()) {
        loginD.ui->labelError->setText("Authentication error.");
        loginD.ui->pushButtonLogin->setEnabled(true);
        loginD.ui->lineEditEmail->setEnabled(true);
        loginD.ui->lineEditPassword->setEnabled(true);
        ui->statusbar->showMessage("Enter credentials to try again.");
    } else {
        loginD.open();
    }
}

void MainWindow::show_accounts_data() {
    if (loginD.isVisible()) {
        loginD.accept();
    }
    if (ui->accountComboBox->count() == 0) {
        for (int i=0; i  < (int)ns.accounts.size(); i++) {
            ui->accountComboBox->addItem(QString::fromStdString(ns.accounts[i].account_code));
        }
    }
    ui->accountComboBox->setCurrentIndex(ns.index_current_acc);
    ui->nameInsertLabel->setText(QString::fromStdString(ns.accounts[ns.index_current_acc].member_name));
    ui->statusbar->showMessage("Loading balance data...");
}

void MainWindow::show_account_balance() {
    ui->balanceInsertLabel->setText(QString::fromStdString(ns.accounts[ns.index_current_acc].print_balance()));
    ui->statusbar->showMessage("Loading transfers data...");
}

void MainWindow::show_account_transfers() {
    QStringList labelHeaders;
    labelHeaders << "created" << "about" << "from" << "to" << "amount" << "state";
    ui->tableWidget->setHorizontalHeaderLabels( labelHeaders );

    ui->statusbar->showMessage("All data is loaded.");
    int rows = ns.accounts[ns.index_current_acc].transfers.size();
    ui->tableWidget->setRowCount(rows);
    for (int row = 0; row < rows; row++) {
        transfer trans = ns.accounts[ns.index_current_acc].transfers[row];
        string data[6] = {
            trans.created.substr(0, 10),
            trans.meta,
            trans.payer_account_code,
            trans.payee_account_code,
            trans.print_amount(),
            trans.state
        };
        for (int column = 0; column < 6; column++) {
            QTableWidgetItem * cellItem = new QTableWidgetItem();
            cellItem->setData(Qt::DisplayRole, QString::fromStdString(data[column]));
            ui->tableWidget->setItem( row, column, cellItem );
        }
    }
}

void MainWindow::changeAccount(int index) {
    if (index != ns.index_current_acc) {
        ns.index_current_acc = index;
        emit change_account();
    }
}

void MainWindow::create_state_machine() {
    // group s1: flow with valid tokens
    QState *s1 = new QState();
    QState *s10NoAccess = new QState(s1);
    QState *s11CheckTokens = new QState(s1);
    QState *s12HasAccess = new QState(s1);
    QState *s13HasAccounts = new QState(s1);
    QState *s14HasBalance = new QState(s1);
    QState *s15HasTransfers = new QState(s1);
    QState *s16HasAllData = new QState(s1);

    s1->setInitialState(s10NoAccess);
    QHistoryState *s1H = new QHistoryState(s1);

    // group s2: email/passwd authentication
    QState *s2 = new QState();
    QState *s20ShowDialog = new QState(s2);
    QState *s21TryAuth = new QState(s2);
    s2->setInitialState(s20ShowDialog);

    s10NoAccess->addTransition(this, SIGNAL(window_shown()), s11CheckTokens);
    s11CheckTokens->addTransition(&ns.oauth2, SIGNAL(has_access()), s12HasAccess);
    s12HasAccess->addTransition(&ns, SIGNAL(has_accounts()), s13HasAccounts);
    s13HasAccounts->addTransition(&ns, SIGNAL(has_balance()), s14HasBalance);
    s14HasBalance->addTransition(&ns, SIGNAL(has_transfers()), s15HasTransfers);
    s15HasTransfers->addTransition(&ns, SIGNAL(has_all_data()), s16HasAllData);
    s16HasAllData->addTransition(this, SIGNAL(change_account()), s13HasAccounts);
    s16HasAllData->addTransition(this, SIGNAL(new_user()), s11CheckTokens);

    s20ShowDialog->addTransition(&loginD, SIGNAL(send_authorization()), s21TryAuth);
    s21TryAuth->addTransition(&ns.oauth2, SIGNAL(error_auth()), s20ShowDialog);

    s1->addTransition(&ns.oauth2, SIGNAL(new_auth()), s2);
    s2->addTransition(&ns.oauth2, SIGNAL(has_access()), s1H);

    machine.addState(s1);
    machine.addState(s2);
    machine.setInitialState(s1);
    machine.start();

    QObject::connect(s11CheckTokens, &QState::entered, &ns.oauth2, &Oauth2::check_tokens);
    QObject::connect(s12HasAccess, &QState::entered, &ns, &netServices::get_accounts);
    QObject::connect(s13HasAccounts, &QState::entered, this, &MainWindow::show_accounts_data);
    QObject::connect(s13HasAccounts, &QState::entered, &ns, &netServices::get_account_balance);
    QObject::connect(s14HasBalance, &QState::entered, this, &MainWindow::show_account_balance);
    QObject::connect(s14HasBalance, &QState::entered, &ns, &netServices::get_account_transfers);
    QObject::connect(s15HasTransfers, &QState::entered, &ns, &netServices::get_unknown_accounts);
    QObject::connect(s16HasAllData, &QState::entered, this, &MainWindow::show_account_transfers);


    QObject::connect(s20ShowDialog, &QState::entered, this, &MainWindow::ask_for_new_auth);
    QObject::connect(&loginD, &LoginDialog::send_authorization, this, &MainWindow::try_authorization);
    QObject::connect(&ns.oauth2, &Oauth2::error_auth, this, &MainWindow::authorization_error);

}

void MainWindow::loadSettings()
{
  QSettings settings;
  kSettings.user_email = settings.value("user_email", "").toString();
  kSettings.access_token = settings.value("access_token", "").toString();
  kSettings.refresh_token = settings.value("refresh_token", "").toString();
  kSettings.created = settings.value("created", 0).toInt();
  kSettings.expires_in = settings.value("expires_in", 3600).toInt();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("user_email", kSettings.user_email);
    settings.setValue("access_token", kSettings.access_token);
    settings.setValue("refresh_token", kSettings.refresh_token);
    settings.setValue("created", kSettings.created);
    settings.setValue("expires_in", kSettings.expires_in);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::on_actionNew_User_triggered() {
    kSettings = {"","","",0,0};
    ns.accounts.clear();

    loginD.ui->labelError->setText("");
    loginD.ui->pushButtonLogin->setEnabled(true);
    loginD.ui->lineEditEmail->setText("");
    loginD.ui->lineEditEmail->setEnabled(true);
    loginD.ui->lineEditPassword->setText("");
    loginD.ui->lineEditPassword->setEnabled(true);

    ui->nameInsertLabel->setText("");
    ui->accountComboBox->blockSignals(true);
    ui->accountComboBox->clear();
    ui->accountComboBox->blockSignals(false);
    ui->balanceInsertLabel->setText("");
    ui->tableWidget->clear();

    emit new_user();
}

void MainWindow::on_actionNew_transaction_triggered() {
    TransferDialog* transferD = new TransferDialog();
    transferD->open();
    ui->statusbar->showMessage("New transaction is under development.");
}

void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}
