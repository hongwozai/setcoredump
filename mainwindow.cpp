#include "mainwindow.h"
#include "setcoredump.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QString>
#include <QToolBar>
#include <time.h>
#include <QMenu>
#include <QTextStream>
#include <string>

#include "setcoredump.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , only_xdesk_(true)
{
    ui->setupUi(this);
    ui->listView->setDragEnabled(false);
    connect(ui->closebtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->refresh_all_btn, SIGNAL(clicked()), this, SLOT(sltRefreshAll()));
    connect(ui->refresh_xdesk_btn, SIGNAL(clicked()), this, SLOT(sltRefreshXdesk()));
    connect(ui->killbtn, SIGNAL(clicked()), this, SLOT(sltKill()));

    model = new QStringListModel();

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Init()
{
    Refresh();

    ui->listView->setModel(model);
}

void MainWindow::Refresh()
{
    list.clear();
    ShowProcessInformation(list);

    QStringList strlist;

    for (auto info : list) {
        QString result;
        QTextStream(&result) << info.pid << " " << QString::fromStdString(info.name);

        printf("info.name: %s\n", info.name.c_str());
        if (only_xdesk_) {
            if (info.name.find("streamsvr") == string::npos &&
                info.name.find("xdesk") == string::npos) {
                continue;
            }
        }
        strlist.append(result);
    }
    model->setStringList(strlist);
}

void MainWindow::sltRefreshAll()
{
    only_xdesk_ = false;
    Refresh();
}

void MainWindow::sltRefreshXdesk()
{
    only_xdesk_ = true;
    Refresh();
}

void MainWindow::sltKill()
{
    /* »ñÈ¡pid */
    QModelIndex index = ui->listView->currentIndex();
    QString item = index.data(Qt::DisplayRole).toString();
    int pid;

    QTextStream(&item) >> pid;

    printf("pid: %d, item: %s\n", pid, item.toStdString().c_str());
    fflush(stdout);

    if (pid != 0) {
        KillProcess(pid);
    }

    Refresh();
}
