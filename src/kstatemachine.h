#ifndef KSTATEMACHINE_H
#define KSTATEMACHINE_H

#include <QStateMachine>
#include <QHistoryState>

#include "mainwindow.h"

class KStateMachine : public QStateMachine
{
private:
     MainWindow& mw;

public:
    KStateMachine(MainWindow&, QObject *parent = nullptr);
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
    void prepare_machine();
};

#endif // KSTATEMACHINE_H
