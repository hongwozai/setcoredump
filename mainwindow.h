#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QStringListModel>

#include "setcoredump.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Init();
    void Refresh();

public slots:

    void sltRefreshAll();
    void sltRefreshXdesk();
    void sltKill();

private:
    Ui::MainWindow *ui;

    std::list<ProcessInfo> list;

    QStringListModel *model;

    bool only_xdesk_;
};
#endif // MAINWINDOW_H
