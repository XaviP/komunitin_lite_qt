#include <QStateMachine>
#include <QSettings>
#include <QCloseEvent>
#include "mainwindow.h"
#include "transfer.h"
#include "./ui_mainwindow.h"
#include "ui_logindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ns(new netServices),
      loginD(),
      machine()
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    create_state_machine();
    loadSettings();
    connect(ui->accountComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(changeAccount(int)));
    ui->statusbar->showMessage("Enter credentials to try authorization.");
    loginD.open();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ns;
    delete state0NoAccess;
    delete state1TryAccess;
    delete state2HasAccess ;
    delete state3HasAccounts;
    delete state4HasBalance;
    delete state5HasTransfers;
    delete state6HasAllData;
}

void MainWindow::try_authorization() {
    ui->statusbar->showMessage("Trying authorization...");
    kSettings.user_email = QString::fromStdString(loginD.get_email());
    ns->get_access(loginD.get_email(), loginD.get_password());
}

void MainWindow::authorization_error() {
    loginD.ui->labelError->setText("Authentication error.");
    loginD.ui->pushButtonLogin->setEnabled(true);
    loginD.ui->lineEditEmail->setEnabled(true);
    loginD.ui->lineEditPassword->setEnabled(true);
    ui->statusbar->showMessage("Enter credentials to try again.");
}

void MainWindow::show_accounts_data() {
    if (loginD.isVisible()) {
        loginD.accept();
    }
    if (ui->accountComboBox->count() == 0) {
        for (int i=0; i  < (int)ns->accounts.size(); i++) {
            ui->accountComboBox->addItem(QString::fromStdString(ns->accounts[i].account_code));
        }
    }
    ui->accountComboBox->setCurrentIndex(ns->index_current_acc);
    ui->nameInsertLabel->setText(QString::fromStdString(ns->accounts[ns->index_current_acc].member_name));
    ui->statusbar->showMessage("Loading balance data...");
}

void MainWindow::show_account_balance() {
    ui->balanceInsertLabel->setText(QString::fromStdString(ns->accounts[ns->index_current_acc].print_balance()));
    ui->statusbar->showMessage("Loading transfers data...");
}

void MainWindow::show_account_transfers() {
    ui->statusbar->showMessage("All data is loaded.");
    int rows = ns->accounts[ns->index_current_acc].transfers.size();
    ui->tableWidget->setRowCount(rows);
    for (int row = 0; row < rows; row++) {
        transfer trans = ns->accounts[ns->index_current_acc].transfers[row];
        string data[6] = {
            trans.created,
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
    if (index != ns->index_current_acc) {
        ns->index_current_acc = index;
        emit change_account();
    }
}

void MainWindow::create_state_machine() {
    QState *state0NoAccess = new QState();
    QState *state1TryAccess = new QState();
    QState *state2HasAccess = new QState();
    QState *state3HasAccounts = new QState();
    QState *state4HasBalance = new QState();
    QState *state5HasTransfers = new QState();
    QState *state6HasAllData = new QState();

    state0NoAccess->addTransition(&loginD, SIGNAL(send_authorization()), state1TryAccess);
    state1TryAccess->addTransition(ns, SIGNAL(error_auth()), state0NoAccess);
    state1TryAccess->addTransition(ns, SIGNAL(has_access()), state2HasAccess);
    state2HasAccess->addTransition(ns, SIGNAL(has_accounts()), state3HasAccounts);
    state3HasAccounts->addTransition(ns, SIGNAL(has_balance()), state4HasBalance);
    state4HasBalance->addTransition(ns, SIGNAL(has_transfers()), state5HasTransfers);
    state5HasTransfers->addTransition(ns, SIGNAL(has_all_data()), state6HasAllData);
    state6HasAllData->addTransition(this, SIGNAL(change_account()), state3HasAccounts);

    machine.addState(state0NoAccess);
    machine.addState(state1TryAccess);
    machine.addState(state2HasAccess);
    machine.addState(state3HasAccounts);
    machine.addState(state4HasBalance);
    machine.addState(state5HasTransfers);
    machine.addState(state6HasAllData);
    machine.setInitialState(state0NoAccess);
    machine.start();

    QObject::connect(state1TryAccess, &QState::entered, this, &MainWindow::try_authorization);
    QObject::connect(ns, &netServices::error_auth, this, &MainWindow::authorization_error);
    QObject::connect(state2HasAccess, &QState::entered, ns, &netServices::get_accounts);
    QObject::connect(state3HasAccounts, &QState::entered, this, &MainWindow::show_accounts_data);
    QObject::connect(state3HasAccounts, &QState::entered, ns, &netServices::get_account_balance);
    QObject::connect(state4HasBalance, &QState::entered, this, &MainWindow::show_account_balance);
    QObject::connect(state4HasBalance, &QState::entered, ns, &netServices::get_account_transfers);
    QObject::connect(state5HasTransfers, &QState::entered, ns, &netServices::get_unknown_accounts);
    QObject::connect(state6HasAllData, &QState::entered, this, &MainWindow::show_account_transfers);
}

void MainWindow::loadSettings()
{
  QSettings settings;
  kSettings.user_email = settings.value("user_email", "").toString();
  kSettings.access_token = settings.value("access_token", "").toString();
  kSettings.refresh_token = settings.value("refresh_token", "").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("user_email", kSettings.user_email);
    settings.setValue("access_token", kSettings.access_token);
    settings.setValue("refresh_token", kSettings.refresh_token);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}
