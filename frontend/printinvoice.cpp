#include "printinvoice.h"
#include "ui_printinvoice.h"

PrintInvoice::PrintInvoice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrintInvoice)
{
    ui->setupUi(this);
    model.setQuery("SELECT printf('%.10d', faktura.id) var, prijemce.nazev, "
                   "strftime('%d. %m. %Y', faktura.vystaveni, 'unixepoch', 'localtime') vystaveni, "
                   "strftime('%d. %m. %Y', faktura.splatnost, 'unixepoch', 'localtime') splatnost "
                   "FROM faktura, prijemce ON faktura.prijemce = prijemce.ic");
//    model.setRelation(1, QSqlRelation("prijemce", "ic", "nazev"));
    model.setHeaderData(0, Qt::Horizontal, "Číslo");
    model.setHeaderData(1, Qt::Horizontal, "Příjemce");
    model.setHeaderData(2, Qt::Horizontal, "Datum vystavení");
    model.setHeaderData(3, Qt::Horizontal, "Datum splatnosti");
//    model.select();
    ui->invView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->invView->setModel(&model);
}

PrintInvoice::~PrintInvoice()
{
    delete ui;
}
