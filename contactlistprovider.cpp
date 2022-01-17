#include "contactlistprovider.h"

ContactListProvider::ContactListProvider(QString& path_to_device, QTableWidget* widget)
    : contact_widget(widget)
{
    QFile file(path_to_device);
    QString errMsg;
    QFileDevice::FileError err = QFileDevice::NoError;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errMsg = file.errorString();
        err = file.error();
        qDebug() << errMsg << err;
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        contacts_list.append(in.readLine());
    }

    auto color = QColor(0,97,109);
    auto brush = QBrush(color);

    int row{0};
    while(!contacts_list.isEmpty())
    {
        auto contact = contacts_list.takeLast();
        QTableWidgetItem* i = new QTableWidgetItem(contact);
        i->setBackground(brush);
        i->setTextAlignment(Qt::AlignCenter);
        widget->insertRow(row);
        widget->setItem(row, 0, i);
        row += 1;
    }
}
