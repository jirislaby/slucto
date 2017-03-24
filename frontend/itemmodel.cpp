#include <QDebug>

#include "itemmodel.h"

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < first)
        return QSqlTableModel::data(index, role);

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    int col = index.column();
    if (col == first) {
        if (index.row() >= counts.size())
            return QVariant(0);
        QVariant res = counts[index.row()];
//        qDebug() << "getData 3" << res;
        return res;
    } else if (col == first + 1) {
        if (index.row() >= comments.size())
            return QVariant("");
        QVariant res = comments[index.row()];
//        qDebug() << "getData 4" << res;
        return res;
    } else {
        qWarning("unhandled column");
        return QVariant();
    }
}

bool ItemModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (index.column() < first || role != Qt::EditRole)
        return QSqlTableModel::setData(index, value, role);

    if (index.row() >= counts.count()) {
        counts.resize(index.row() + 1);
        comments.resize(index.row() + 1);
    }

    int col = index.column();
    if (col == first) {
        int val = value.toInt();
        if (val < 0)
            return false;
        counts[index.row()] = val;
    } else if (col == first + 1) {
        comments[index.row()] = value.toString();
    } else {
        qWarning("unhandled column");
        return false;
    }

    emit dataChanged(index, index);

//    qDebug() << __func__ << index << value << role;
    return true;
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlTableModel::flags(index);

    if (index.column() >= first)
        flags |= Qt::ItemIsEditable;
    else
        flags &= ~Qt::ItemIsEditable;

    return flags;
}
