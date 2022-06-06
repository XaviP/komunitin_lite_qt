#include "mainwindow.h"
#include "kstatemachine.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Komunitin");
    a.setOrganizationDomain("komunitin");
    a.setApplicationName("komunitin_lite_qt");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "komunitin_lite_qt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    KStateMachine machine(w);
    machine.prepare_machine();

    w.show();

    return a.exec();
}
