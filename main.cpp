#include "./mainwindow.h"
#include <QApplication>

#include "./lib/inc/RemoterGuard.h"

#ifdef Q_OS_WIN
#   ifdef main
#      undef main
#   endif
#endif

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    FEITENG::RemoterGuard remoter_guard(&w);

    w.show();
    return a.exec();
}
