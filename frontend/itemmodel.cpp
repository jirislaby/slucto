#include <QDebug>

#include "itemmodel.h"

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 3)
        return QSqlTableModel::data(index, role);

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    switch (index.column()) {
    case 3: {
        if (index.row() >= counts.size())
            return QVariant(0);
        QVariant res = counts[index.row()];
//        qDebug() << "getData 3" << res;
        return res;
    }
    case 4: {
        if (index.row() >= comments.size())
            return QVariant("");
        QVariant res = comments[index.row()];
//        qDebug() << "getData 4" << res;
        return res;
    }
    default:
        qWarning("unhandled column");
        return QVariant();
    }
}

bool ItemModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (index.column() < 3 || role != Qt::EditRole)
        return QSqlTableModel::setData(index, value, role);

    counts.resize(index.row() + 1);
    comments.resize(index.row() + 1);

    switch (index.column()) {
    case 3: {
        int val = value.toInt();
        if (val < 0)
            return false;
        counts[index.row()] = val;
        break;
    }
    case 4:
        comments[index.row()] = value.toString();
        break;
    default:
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

    if (index.column() >= 3) {
        flags |= Qt::ItemIsEditable;
    } else
        flags &= ~Qt::ItemIsEditable;

    return flags;
}
