#ifndef CONTACTLISTPROVIDER_H
#define CONTACTLISTPROVIDER_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>

class ContactListProvider
{
public:
    ContactListProvider(QString& device, QTableWidget* main_table_widget);
private:
    QStringList contacts_list;
    QTableWidget* contact_widget;
};

#endif // CONTACTLISTPROVIDER_H
