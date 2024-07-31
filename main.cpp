#include "mainwindow.h"

#include <QApplication>

#include "QJoysticks.h"
#include <QDebug>

#ifdef Q_OS_WIN
#   ifdef main
#      undef main
#   endif
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // QJoysticks* instance = QJoysticks::getInstance();
    // instance->setVirtualJoystickEnabled(false);

    // instance->updateInterfaces();
    // qDebug()<<instance->nonBlacklistedCount();
    // qDebug()<<instance->deviceNames();

    w.show();
    return a.exec();
}
