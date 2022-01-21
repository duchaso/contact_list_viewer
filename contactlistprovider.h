#ifndef CONTACTLISTPROVIDER_H
#define CONTACTLISTPROVIDER_H
//for working with text
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
//for debug
#include <QDebug>
//for table
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
//for icon
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
//for call function
#include <QMessageBox>
//for favourites
#include <QCheckBox>

//enumeration for view of table
enum class View
{
    list,
    grid,
};

class ContactListProvider
{
public:
    ContactListProvider(QString& device, QTableWidget* main_table_widget);
public:
    View get_view();
    void set_view(View type);
    QCheckBox* get_cellWidget(int row, int col);
    void swap_cells(int row_first, int col_first, int row_second, int col_second);
public:
    void switch_view();
    void call(const QTableWidgetItem* id);
    void show_favourites(bool show);
    void text_filter(QString text_to_filter);
    void jump_to(QString letter);
private:
    QStringList contacts_list;
    QTableWidget* contact_widget;
    View type_of_clp;
};

#endif // CONTACTLISTPROVIDER_H
