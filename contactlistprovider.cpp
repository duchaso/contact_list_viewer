#include "contactlistprovider.h"

//temp
#include <QCheckBox>

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
        QCheckBox* cb = new QCheckBox();//*
        i->setBackground(brush);
        i->setTextAlignment(Qt::AlignCenter);
        i->setIcon(icon);
        widget->insertRow(row);
        widget->setItem(row, 0, i);
        widget->setCellWidget(row, 0, cb);//*
        row += 1;
    }

    //contact_widget->hideRow(0);
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

QCheckBox* ContactListProvider::get_cellWidget(int row, int col)
{
    auto cb_old = static_cast<QCheckBox*>(contact_widget->cellWidget(row, col));
    auto cb_new = new QCheckBox();

    cb_new->setCheckState(cb_old->checkState());
    contact_widget->removeCellWidget(row, col);

    return cb_new;
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
            auto cell_widget = get_cellWidget(i, 0);
            auto item = contact_widget->takeItem(i, 0);

            contact_widget->setCellWidget(row, col, cell_widget);
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
        while(!(contact_widget->item(contact_widget->rowCount()-1, 0))) contact_widget->removeRow(contact_widget->rowCount()-1);//////////// check in future
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
                auto cell_widget = get_cellWidget(row, col);

                contact_widget->setItem(row_to_set, 0, item);
                contact_widget->setCellWidget(row_to_set, 0, cell_widget);
                row_to_set -= 1;
            }

        while(!(contact_widget->item(contact_widget->rowCount()-1, 0))) contact_widget->removeRow(contact_widget->rowCount()-1);//delete empty row from end
        contact_widget->removeColumn(1);
        contact_widget->removeColumn(1);
        contact_widget->horizontalHeader()->resizeSection(0, 415);
    }
}

void ContactListProvider::swap_cells(int row_f, int col_f, int row_s, int col_s)
{
    auto cell_widget1 = get_cellWidget(row_f, col_f);
    auto item1 = contact_widget->takeItem(row_f, col_f);

    auto cell_widget2 = get_cellWidget(row_s, col_s);
    auto item2 = contact_widget->takeItem(row_s, col_s);

    contact_widget->setCellWidget(row_s, col_s, cell_widget1);
    contact_widget->setItem(row_s, col_s, item1);

    contact_widget->setCellWidget(row_f, col_f, cell_widget2);
    contact_widget->setItem(row_f, col_f, item2);
}

void ContactListProvider::show_favourites(bool show)
{
    if(type_of_clp == View::list)
    {
        if(show)
        {
            int row_count = contact_widget->rowCount();
            for(int row = 0; row < row_count; ++row)
            {
                auto isFav = static_cast<QCheckBox*>(contact_widget->cellWidget(row, 0))->isChecked();
                if(!isFav)
                    contact_widget->hideRow(row);
            }
            contact_widget->sortItems(0);
        } else {
            for(int row = 0; row < contact_widget->rowCount(); ++row)
                contact_widget->showRow(row);
        }
    } else {

        int count_emp_items = 0;
        while(!(contact_widget->item(contact_widget->rowCount()-1, contact_widget->columnCount()-1-count_emp_items)))
            ++count_emp_items;

        for(int row = 0; row < contact_widget->rowCount(); ++row)
            for(int col = 0; col < contact_widget->columnCount(); ++col)
            {
                if(!(contact_widget->item(row, col))) continue;
                auto isFav = static_cast<QCheckBox*>(contact_widget->cellWidget(row, col))->isChecked();
                if(isFav)
                {
                    for(int row_to_swap = row; row_to_swap>0; --row_to_swap)
                    {
                        for(int col_to_swap = col; col_to_swap>0; --col_to_swap)
                            swap_cells(row_to_swap, col_to_swap, row_to_swap, col_to_swap-1);
                    }
                }
                else {
                    continue;
                }
            }


//        QVector<QPair<QTableWidgetItem*, QCheckBox*>> v;
//        for(int row = 0; row < contact_widget->rowCount(); ++row)
//            for(int col = 0; col < contact_widget->columnCount(); ++col)
//            {
//                auto cell_widget = get_cellWidget(row, col);
//                auto item = contact_widget->takeItem(row, col);
//                QPair<QTableWidgetItem*, QCheckBox*> pair(item, cell_widget);
//                v.append(pair);
//            }
//        if(show)
//        {
//            int row = 0;
//            int col = 0;
//            for(auto i : v)
//            {
//                auto isFav = i.second->isChecked();
//                if(isFav)
//                {
//                    contact_widget->setItem(row, col, i.first);
//                    contact_widget->setCellWidget(row, col, i.second);
//                    if(col == 2)
//                    {
//                        ++row;
//                        col = 0;
//                    } else {
//                        ++col;
//                    }
//                }
//            }
//        } else {
//            auto widg = static_cast<QCheckBox*>(contact_widget->cellWidget(0,0));
//            auto item = contact_widget->takeItem(0,0);
//            QPair<QTableWidgetItem*, QCheckBox*> foo(item, widg);
//            qDebug() << v.indexOf(foo);
//            contact_widget->clear();
//            int row = 0;
//            int col = 0;
//            for(auto i : v)
//            {
//                if(contact_widget->item(row, col))
//                {


//                    auto cb_old = static_cast<QCheckBox*>(contact_widget->cellWidget(row, col));
//                    auto cb_new = new QCheckBox();
//                    cb_new->setCheckState(cb_old->checkState());
//                    contact_widget->takeItem(row, col);
//                    contact_widget->removeCellWidget(row, col);
//                }
//                contact_widget->setItem(row, col, i.first);
//                contact_widget->setCellWidget(row, col, i.second);
//                if(col == 2)
//                {
//                    ++row;
//                    col = 0;
//                } else {
//                    ++col;
//                }
//            }
//        }
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
