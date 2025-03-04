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

class MainWindow : public QMainWindow
{
        Q_OBJECT

    private:
        Ui::main_window* ui_ptr;

    public:
        MainWindow(QWidget* = nullptr);
        ~MainWindow();

        template<typename TypePtr>
            TypePtr findChildControl(const QString&);

    public slots:
        void updateJoysticksList(const QStringList&, const int);
        void updateJoystickData(const QList<double>&,
                                const QList<bool>&,
                                const QList<int>&);
        void updateRobotData(const FEITENG::RobotData&);
        void updateHostIp(const QString&);
        void updateHostPort(const quint16);

        void onVideoAddrEditingFinished();
};

template<typename TypePtr>
    TypePtr MainWindow::findChildControl(const QString& name)
    {
        return this->findChild<TypePtr>(name);
    }

#endif // MAINWINDOW_H
