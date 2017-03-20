#ifndef NEWINVOICE_H
#define NEWINVOICE_H

#include <QMainWindow>
#include <QtSql>

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
    void prev();
    void next();

private:
    Ui::NewInvoice *ui;
    QSqlTableModel rcvModel;
    QSqlTableModel itmModel;

    void buttonsEnable();
};

#endif // NEWINVOICE_H
