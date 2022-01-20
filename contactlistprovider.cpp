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
        QCheckBox* cb = new QCheckBox();//*
        i->setBackground(brush);
        i->setTextAlignment(Qt::AlignCenter);
        i->setIcon(icon);
        widget->insertRow(row);
        widget->setItem(row, 0, i);
        widget->setCellWidget(row, 0, cb);//*
        row += 1;
    }
    contact_widget->setCurrentItem(nullptr);
    contact_widget->setWordWrap(true);
    contact_widget->sortItems(0);
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

        contact_widget->sortItems(0);

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
/******************* grid view favourites not implemented *****************************
        int row_count = contact_widget->rowCount();
        int col_count = contact_widget->columnCount();

        for(int i = 0; i < row_count*col_count-1; ++i)
            for(int j = 0; j < row_count*col_count-1-j; ++j)
            {
                int row = j/col_count;
                int column = j%col_count;
                int next_row = (j+1)/col_count;
                int next_column = (j+1)%col_count;
                auto isFav = static_cast<QCheckBox*>(contact_widget->cellWidget(row, column))->isChecked();
                auto isFav2 = static_cast<QCheckBox*>(contact_widget->cellWidget(next_row, next_column))->isChecked();
                if(isFav<isFav2)
                    swap_cells(row, column, next_row, next_column);
            }
**************************************************************************************/
    }
}

void ContactListProvider::text_filter(QString str)
{
    if(str=="")
        for(int row = 0; row < contact_widget->rowCount(); ++row)
            contact_widget->showRow(row);
    else {
        for(int row = 0; row < contact_widget->rowCount(); ++row)
            contact_widget->hideRow(row);
        auto finded_items = contact_widget->findItems(str, Qt::MatchContains);
        QTableWidgetItem* i = nullptr;
        foreach(i, finded_items)
        {
            auto row = contact_widget->row(i);
            if(row >= 0)
            {
                contact_widget->showRow(row);
            }
        }
    }
}

void ContactListProvider::jump_to(QString l)
{
    auto jump = contact_widget->findItems(l, Qt::MatchStartsWith);
    if(!jump.isEmpty())
    {
        auto first_item = jump.first();
        contact_widget->setCurrentItem(first_item);
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
