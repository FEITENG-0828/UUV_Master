#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

        template<typename T>
            T findChildControl(const QString&);

    public slots:
        void updateJoysticksList(const QStringList&, const int);
        void updateJoystickData(const QList<double>&, const QList<bool>&);
        void updateRobotData(const QList<float>&);
};

template<typename T>
    T MainWindow::findChildControl(const QString& name)
    {
        return this->findChild<T>(name);
    }
#endif // MAINWINDOW_H
