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


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{

    clp->call(ui->tableWidget->item(row, column));
}


void MainWindow::on_pushButton_clicked()
{
    clp->switch_view();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool checked = arg1;
    clp->show_favourites(checked);
}


//void MainWindow::on_tableWidget_cellChanged(int row, int column)
//{

//}


//void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
//{
//    if(ui->checkBox->isChecked())
//        ui->tableWidget->insertRow(1);
//}


void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    auto str = ui->lineEdit->text();
    //qDebug() << str;
    clp->text_filter(str);
}

