#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "RobotData.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class main_window;
}
QT_END_NAMESPACE

namespace FEITENG
{
    class RemoterGuard;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    private:
        Ui::main_window* m_ui_ptr;
        FEITENG::RemoterGuard* m_remoter_guard_ptr;

        void setupRobotRemoteAddr();
        void setupVideo();

    public:
        explicit MainWindow(QWidget* = nullptr);
        ~MainWindow();

        void setRemoterGuard(FEITENG::RemoterGuard*);

    public slots:
        void updateJoysticksList(const QStringList&, const int);
        void updateJoystickData(const QList<double>&,
                                const QList<bool>&,
                                const QList<int>&);
        void updateRobotData(const FEITENG::RobotData&);

        void onRobotRemoteIpEditingFinished();
        void onRobotRemotePortEditingFinished();
        void onVideoAddrEditingFinished();
};

#endif // MAINWINDOW_H
