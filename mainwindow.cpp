#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString device = "D:/dev/contact_list_viewer/contact_list_viewer/data/names.txt";
    clp = new ContactListProvider(device, ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

