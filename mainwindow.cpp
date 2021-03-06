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
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString device = "D:/dev/contact_list_viewer/contact_list_viewer/data/names.txt";
    clp = new ContactListProvider(device, ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//for calling function
void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{

    clp->call(ui->tableWidget->item(row, column));
}

//for switching view of table
void MainWindow::on_pushButton_clicked()
{
    clp->switch_view();
}

//for favourites
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool checked = arg1;
    clp->show_favourites(checked);
}

//for filtering contacts
void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    auto str = ui->lineEdit->text();
    clp->text_filter(str);
}

//for alphabetical jump
void MainWindow::on_tableWidget_2_cellClicked(int row, int column)
{
    auto letter = ui->tableWidget_2->item(row, column)->data(Qt::DisplayRole).toString();
    clp->jump_to(letter);
}

