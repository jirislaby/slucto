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
    int first;

public:
    ItemModel() : counts(), comments(), first(0) {};

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                            int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setFirst(int first) { this->first = first; }
    const QVector<int> getCounts() { return counts; }
    const QString getComment(int i) { return comments[i]; }
};

#endif // ITEMMODEL_H
