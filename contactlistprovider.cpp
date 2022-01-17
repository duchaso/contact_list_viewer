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
    : contact_widget(widget), type_of_clp(View::list)
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

void ContactListProvider::call(const QTableWidgetItem* id)
{
    if(!id) return;
    auto name = id->data(Qt::DisplayRole).toString().toUpper();
    QMessageBox call;
    call.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    call.setText("Calling");
    call.setInformativeText(name);
    call.setStandardButtons(QMessageBox::Discard);
    call.exec();
}

void ContactListProvider::switch_view()
{
    if(type_of_clp == View::list)
    {
        type_of_clp = View::grid;

        for(int i = 1; i < 3; ++i)
            contact_widget->insertColumn(i);
        for(int i = 0; i < 3; ++i)
            contact_widget->horizontalHeader()->resizeSection(i, 137);

        int row = 0;
        int col = 0;
        for(int i = 0; i < contact_widget->rowCount(); ++i)
        {
            auto item = contact_widget->takeItem(i, 0);
            contact_widget->setItem(row, col, item);

            if(col == 2)
            {
                ++row;
                col = 0;
            } else {
                ++col;
            }
        }
        for(int i = contact_widget->rowCount(); i > row; --i)
        {
            contact_widget->removeRow(i);
        }
    } else {
        type_of_clp = View::list;

        int column_to_add = (contact_widget->columnCount()*contact_widget->rowCount())-contact_widget->rowCount();
        int row_count_old = contact_widget->rowCount();
        for(int i = row_count_old; i < row_count_old+column_to_add; ++i)
        {
            contact_widget->insertRow(i);
        }

        int row_to_set = contact_widget->rowCount()-1;
        for(int row = row_count_old-1; row >= 0; --row)
            for(int col = contact_widget->columnCount()-1; col >= 0; --col)
            {

                if(contact_widget->item(row, col)==nullptr)
                {
                    --row_to_set;
                    continue;
                }
                auto item = contact_widget->takeItem(row, col);
                contact_widget->setItem(row_to_set--, 0, item);
            }

        while(!(contact_widget->item(contact_widget->rowCount()-1, 0))) contact_widget->removeRow(contact_widget->rowCount()-1);//delete empty row from end
        contact_widget->removeColumn(1);
        contact_widget->removeColumn(1);
        contact_widget->horizontalHeader()->resizeSection(0, 415);
    }
}

View ContactListProvider::get_view()
{
    return type_of_clp;
}
void ContactListProvider::set_view(View type)
{
    type_of_clp = type;
}
