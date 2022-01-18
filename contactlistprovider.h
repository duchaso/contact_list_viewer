#ifndef CONTACTLISTPROVIDER_H
#define CONTACTLISTPROVIDER_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QIcon> // for icon
#include <QPixmap> // for icon
#include <QBitmap> // for icon
#include <QPainter> // for icon
#include <QMessageBox> // for call

#include <QVector>
#include <QMap>
#include <QPair>

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
    void call(const QTableWidgetItem* id);
    void switch_view();
public:
    View get_view();
    void set_view(View type);
    QCheckBox* get_cellWidget(int row, int col);
    void show_favourites(bool show);
    void swap_cells(int row_first, int col_first, int row_second, int col_second);
private:
    QStringList contacts_list;
    QTableWidget* contact_widget;
    View type_of_clp;
};

#endif // CONTACTLISTPROVIDER_H
