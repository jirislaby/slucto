#ifndef NEWINVOICE_H
#define NEWINVOICE_H

#include <QMainWindow>
#include <QtSql>
#include <QString>

#include "itemmodel.h"

namespace Ui {
class NewInvoice;
}

class NewInvoice : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewInvoice(QWidget *parent = 0);
    ~NewInvoice();

private slots:
    void setupModels();

    void currentChanged();
    void prev();
    void next();

    void newRcv();
    void filterRcv(QString);

    void copyToVS();

private:
    Ui::NewInvoice *ui;
    QSqlTableModel rcvModel;
    ItemModel itmModel;
    QSqlRelationalTableModel relModel;

    void buttonsEnable();
};

#endif // NEWINVOICE_H
