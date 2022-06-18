#include "kstatemachine.h"

KStateMachine::KStateMachine (Backend& backend, QObject *parent) :
    QStateMachine(parent),
    bkd(backend),

    // group s1: flow with valid tokens
    s1(new QState()),
    s1H(new QHistoryState(s1)),
    s10NoAccess(new QState(s1)),
    s11CheckTokens(new QState(s1)),
    s12HasAccess(new QState(s1)),
    s13HasAccounts(new QState(s1)),
    s14HasBalance(new QState(s1)),
    s15HasTransfers(new QState(s1)),
    s16HasAllData(new QState(s1)),

    // group s2: email/passwd authentication
    s2(new QState()),
    s20ShowDialog(new QState(s2)),
    s21TryAuth(new QState(s2)),

    // group s3: send transaction
    s3(new QState()),
    s30ShowDialog(new QState(s3)),
    s31CheckAccount(new QState(s3)),
    s32ShowConfirm(new QState(s3)),
    s33SendTransfer(new QState(s3))

{}

KStateMachine::~KStateMachine() {
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
    delete s3;
    delete s30ShowDialog;
    delete s31CheckAccount;
    delete s32ShowConfirm;
    delete s33SendTransfer;
}

void KStateMachine::prepare_machine() {

    s1->setInitialState(s10NoAccess);
    s2->setInitialState(s20ShowDialog);
    s3->setInitialState(s30ShowDialog);

    // group s1
    s10NoAccess->addTransition(&bkd, SIGNAL(app_is_ready()), s11CheckTokens);
    s11CheckTokens->addTransition(&bkd.ns.oauth2, SIGNAL(has_access()), s12HasAccess);
    s12HasAccess->addTransition(&bkd.ns, SIGNAL(has_accounts()), s13HasAccounts);
    s13HasAccounts->addTransition(&bkd.ns, SIGNAL(has_balance()), s14HasBalance);
    s14HasBalance->addTransition(&bkd.ns, SIGNAL(has_transfers()), s15HasTransfers);
    s15HasTransfers->addTransition(&bkd.ns, SIGNAL(has_all_data()), s16HasAllData);
    // transition when reload transfer is triggered or when change account in accountComboBox
    s16HasAllData->addTransition(&bkd, SIGNAL(reload_transfers()), s13HasAccounts);
    // transition when new user is triggered
    s16HasAllData->addTransition(&bkd, SIGNAL(new_user()), s11CheckTokens);

    // group s2
    s20ShowDialog->addTransition(&bkd, SIGNAL(send_authorization()), s21TryAuth);
    s21TryAuth->addTransition(&bkd.ns.oauth2, SIGNAL(error_auth()), s20ShowDialog);

    // group s3
//    s30ShowDialog->addTransition(&bkd.transD, SIGNAL(check_account()), s31CheckAccount);
//    s31CheckAccount->addTransition(&bkd.ns, SIGNAL(confirm_transfer()), s32ShowConfirm);
//    s32ShowConfirm->addTransition(&bkd.transD, SIGNAL(send_transfer()), s33SendTransfer);
//    s32ShowConfirm->addTransition(&bkd.transD, SIGNAL(edit_transfer()), s30ShowDialog);

    // transitions between groups s1, s2, and s3
    s1->addTransition(&bkd.ns.oauth2, SIGNAL(new_auth()), s2);
//    s1->addTransition(&bkd, SIGNAL(new_transfer()), s3);
    s2->addTransition(&bkd.ns.oauth2, SIGNAL(has_access()), s1H);
//    s3->addTransition(&bkd.transD, SIGNAL(exit_s3()), s1H);
//    s3->addTransition(&bkd.ns, SIGNAL(transfer_done()), s1H);

    addState(s1);
    addState(s2);
    addState(s3);
    setInitialState(s1);

    start(); // start machine

    // s1
    QObject::connect(s11CheckTokens, &QState::entered, &bkd.ns.oauth2, &Oauth2::check_tokens);
    QObject::connect(s12HasAccess, &QState::entered, &bkd.ns, &netServices::get_accounts);
    QObject::connect(s13HasAccounts, &QState::entered, &bkd, &Backend::show_accounts_data);
    QObject::connect(s13HasAccounts, &QState::entered, &bkd.ns, &netServices::get_account_balance);
    QObject::connect(s14HasBalance, &QState::entered, &bkd, &Backend::show_account_balance);
    QObject::connect(s14HasBalance, &QState::entered, &bkd.ns, &netServices::get_account_transfers);
    QObject::connect(s15HasTransfers, &QState::entered, &bkd.ns, &netServices::get_unknown_accounts);
    QObject::connect(s16HasAllData, &QState::entered, &bkd, &Backend::show_account_transfers);

    // s2
    QObject::connect(s20ShowDialog, &QState::entered, &bkd, &Backend::ask_for_new_auth);
//    QObject::connect(&bkd, &Backend::send_authorization, &bkd, &Backend::try_authorization);
//    QObject::connect(&bkd.ns.oauth2, &Oauth2::error_auth, &bkd, &Backend::authorization_error);

    // s3
//    QObject::connect(s31CheckAccount, &QState::entered, &bkd, &Backend::check_account);
//    QObject::connect(s32ShowConfirm, &QState::entered, &bkd, &Backend::confirm_transfer);
//    QObject::connect(s33SendTransfer, &QState::entered, &bkd.ns, &netServices::post_new_transfer);
//    QObject::connect(&bkd.ns, &netServices::transfer_done, &bkd, &Backend::transfer_done);
}
