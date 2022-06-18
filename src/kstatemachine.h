#ifndef KSTATEMACHINE_H
#define KSTATEMACHINE_H

#include <QStateMachine>
#include <QHistoryState>

#include "backend.h"

class KStateMachine : public QStateMachine
{
private:
     Backend& bkd;

public:
    KStateMachine(Backend&, QObject *parent = nullptr);
    ~KStateMachine();
    QState *s1;
    QHistoryState *s1H;
    QState *s10NoAccess;
    QState *s11CheckTokens;
    QState *s12HasAccess;
    QState *s13HasAccounts;
    QState *s14HasBalance;
    QState *s15HasTransfers;
    QState *s16HasAllData;
    QState *s2;
    QState *s20ShowDialog;
    QState *s21TryAuth;
    QState *s22HasAccess;
    QState *s3;
    QState *s30ShowDialog;
    QState *s31CheckAccount;
    QState *s32ShowConfirm;
    QState *s33SendTransfer;
    void prepare_machine();
};

#endif // KSTATEMACHINE_H
