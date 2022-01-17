#include "contactlistprovider.h"

QPixmap to_circle_icon(const QString& icon)
{
    QPixmap pix(icon);
    if (pix.isNull()) {
          qFatal("Failed to load.");
          return QPixmap();
    }
    QBitmap mask(pix.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), 100, 100);

    pix.setMask(mask);
    return pix;
}

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

    auto pix = to_circle_icon("D:/dev/contact_list_viewer/contact_list_viewer/data/icon.jpg");
    auto icon = QIcon(pix);

    widget->setIconSize(QSize(50,50));
    int row{0};
    while(!contacts_list.isEmpty())
    {
        auto contact = contacts_list.takeLast();
        QTableWidgetItem* i = new QTableWidgetItem(contact);
        i->setBackground(brush);
        i->setTextAlignment(Qt::AlignCenter);
        i->setIcon(icon);
        widget->insertRow(row);
        widget->setItem(row, 0, i);
        row += 1;
    }
}
