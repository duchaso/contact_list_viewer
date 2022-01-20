#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "contactlistprovider.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

//    void on_tableWidget_cellChanged(int row, int column);

//    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;
    ContactListProvider* clp;
};
#endif // MAINWINDOW_H
