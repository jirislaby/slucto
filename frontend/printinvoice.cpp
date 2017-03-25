#include <QDebug>
#include <QMessageBox>

#include "printinvoice.h"
#include "ui_printinvoice.h"

PrintInvoice::PrintInvoice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrintInvoice)
{
    ui->setupUi(this);
    model.setQuery("SELECT invoice.id, receiver.name, "
                   "strftime('%d. %m. %Y', invoice.issuance, 'unixepoch', 'localtime') issuance, "
                   "strftime('%d. %m. %Y', invoice.due, 'unixepoch', 'localtime') due "
                   "FROM invoice, receiver ON invoice.receiver = receiver.ic "
                   "ORDER BY invoice.id");
    model.setHeaderData(0, Qt::Horizontal, "Číslo");
    model.setHeaderData(1, Qt::Horizontal, "Příjemce");
    model.setHeaderData(2, Qt::Horizontal, "Datum vystavení");
    model.setHeaderData(3, Qt::Horizontal, "Datum splatnosti");
    ui->invView->setModel(&model);
    ui->invView->resizeColumnsToContents();
}

PrintInvoice::~PrintInvoice()
{
    delete ui;
}

void PrintInvoice::printInvoice()
{
    QItemSelectionModel *sm = ui->invView->selectionModel();
    if (!sm->hasSelection()) {
        QMessageBox::critical(this, "Chyba", "Není zvolena žádná faktura k tisku!");
        return;
    }
    for (QModelIndex &i : sm->selectedRows()) {
        const QSqlQueryModel *sqlModel = dynamic_cast<const QSqlQueryModel *>(i.model());
        qulonglong id = sqlModel->record(i.row()).value("id").toULongLong();
        qDebug() << "row" << i.row() << "id" << id;

        QProcess process;
        process.start("slucto_cli", QStringList() << QString::number(id));
        process.waitForFinished(-1);
        qDebug() << "exit code", process.exitCode();
    }
}
