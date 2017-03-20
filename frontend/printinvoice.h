#ifndef PRINTINVOICE_H
#define PRINTINVOICE_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class PrintInvoice;
}

class PrintInvoice : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrintInvoice(QWidget *parent = 0);
    ~PrintInvoice();

private slots:
    void printInvoice();

private:
    Ui::PrintInvoice *ui;
    QSqlQueryModel model;
};

#endif // PRINTINVOICE_H
