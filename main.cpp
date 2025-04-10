#include "mainwindow.h"

#include <QApplication>

#include "RemoterGuard.h"

#ifdef Q_OS_WIN
#ifdef main
#undef main
#endif
#endif

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow main_window;

    FEITENG::RemoterGuard* remoter_guard = new FEITENG::RemoterGuard(&main_window);
    main_window.setRemoterGuard(remoter_guard);

    main_window.show();
    return app.exec();
}
