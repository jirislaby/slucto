#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QModelIndex>
#include <QSqlTableModel>
#include <Qt>
#include <QVector>

class ItemModel : public QSqlTableModel
{
private:
    QVector<int> counts;
    QVector<QString> comments;
public:
    ItemModel() : counts(), comments() {};

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                            int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // ITEMMODEL_H
