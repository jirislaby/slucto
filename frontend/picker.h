#ifndef PICKER_H
#define PICKER_H

#include <QWidget>

#include "newinvoice.h"
#include "printinvoice.h"

namespace Ui {
class Picker;
}

class Picker : public QWidget
{
    Q_OBJECT

public:
    explicit Picker(QWidget *parent = 0);
    ~Picker();

private slots:
    void printInvoice();
    void newInvoice();

private:
    Ui::Picker *ui;

    PrintInvoice pi;
    NewInvoice su;
};

#endif // PICKER_H
