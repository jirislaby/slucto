#include <QDebug>

#include "newinvoice.h"
#include "ui_newinvoice.h"

NewInvoice::NewInvoice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewInvoice)
//  ,     sumItmModel(0, 4)
{
    ui->setupUi(this);

    buttonsEnable();

    rcvModel.setTable("receiver");
    rcvModel.select();
    itmModel.setTable("item");
    itmModel.select();
    relModel.setTable("invoice_item");
    relModel.select();

    rcvModel.setHeaderData(0, Qt::Horizontal, "IČ");
    rcvModel.setHeaderData(1, Qt::Horizontal, "Název");
    rcvModel.setHeaderData(2, Qt::Horizontal, "Ulice");
    rcvModel.setHeaderData(3, Qt::Horizontal, "Město");
    rcvModel.setHeaderData(4, Qt::Horizontal, "PSČ");

    itmModel.removeColumn(0);
    itmModel.setHeaderData(0, Qt::Horizontal, "Název");
    itmModel.setHeaderData(1, Qt::Horizontal, "Cena");

    const int first = 2;
    itmModel.setFirst(first);
    itmModel.insertColumn(first);
    itmModel.setHeaderData(first, Qt::Horizontal, "Počet");
    itmModel.insertColumn(first + 1);
    itmModel.setHeaderData(first + 1, Qt::Horizontal, "Poznámka");

    relModel.setHeaderData(0, Qt::Horizontal, "ID");
    relModel.setHeaderData(1, Qt::Horizontal, "Název");
    relModel.setHeaderData(2, Qt::Horizontal, "Cena");
    relModel.setHeaderData(3, Qt::Horizontal, "Počet");
    relModel.setHeaderData(4, Qt::Horizontal, "Poznámka");

/*    sumItmModel.setHeaderData(0, Qt::Horizontal, "Název");
    sumItmModel.setHeaderData(1, Qt::Horizontal, "Cena");
    sumItmModel.setHeaderData(2, Qt::Horizontal, "Počet");
    sumItmModel.setHeaderData(3, Qt::Horizontal, "Poznámka");
*/
    ui->rcvView->setModel(&rcvModel);
    ui->itmView->setModel(&itmModel);
    //ui->sumItemView->setModel(&sumItmModel);
 //   ui->sumItemView->setModel(&itmModel);
    ui->sumItemView->setModel(&relModel);

    ui->rcvView->resizeColumnsToContents();
    for (int c = 0; c <= first; c++)
        ui->itmView->resizeColumnToContents(c);
    ui->itmView->setColumnWidth(first, ui->itmView->columnWidth(first) + 20);
}

NewInvoice::~NewInvoice()
{
    delete ui;
}

void NewInvoice::copyToVS()
{
    ui->invVSEdit->setText(QString::number(ui->invNOBox->value()));
}

void NewInvoice::buttonsEnable()
{
    int idx = ui->tabWidget->currentIndex();

    ui->backButton->setDisabled(idx == 0);
    ui->fwdButton->setDisabled(idx == ui->tabWidget->count() - 1);
}

void NewInvoice::currentChanged()
{
    buttonsEnable();

    if (ui->tabWidget->currentWidget() == ui->tab_sum) {
        const QItemSelectionModel *selModel = ui->rcvView->selectionModel();
        if (!selModel || !selModel->selectedRows().count())
            return;
        int row = selModel->selectedRows()[0].row();
        const QSqlRecord &rec = rcvModel.record(row);
        int receiver = rec.value("ic").toInt();
        ui->ICEdit->setText(rec.value("ic").toString());
        ui->nameEdit->setText(rec.value("name").toString());
        ui->cityEdit->setText(rec.value("city").toString());

        QDate curr = QDate::currentDate();
        ui->issEdit->setDate(curr);
        ui->dueEdit->setDate(curr.addDays(14));

        QSqlQuery query;
        query.prepare("SELECT id + 1 FROM invoice WHERE id BETWEEN :min "
                        "AND :max ORDER BY id DESC LIMIT 1");
        int id = curr.year() * 100000 + curr.month() * 1000 + 1;
        query.bindValue(":min", id);
        query.bindValue(":max", id + 998);
        query.exec();
        while (query.next()) {
            id = query.value(0).toInt();
        }
        ui->invNOBox->setValue(id);
        ui->invVSEdit->setText(QString::number(id));

        query.prepare("INSERT OR IGNORE INTO invoice "
                      "(id, receiver, issuance, due) VALUES "
                      "(:id, :rcv, :iss, :due)");
        query.bindValue(":id", id);
        query.bindValue(":rcv", receiver);
        query.bindValue(":iss", QDateTime(curr).toLocalTime());
        query.bindValue(":due", QDateTime(curr.addDays(14)).toLocalTime());
        query.exec();
        query.prepare("INSERT OR REPLACE INTO invoice_item "
                      "(id_invoice, id_item, count, price, note) VALUES "
                      "(:inv, :itm, :cnt, :pr, :nt)");
        query.bindValue(":inv", id);
        query.bindValue(":itm", 0);
        query.bindValue(":cnt", 0);
        query.bindValue(":pr", 0);
        query.bindValue(":note", "");
        query.exec();
        relModel.setFilter(QString("id_invoice=%1").arg(id));
#if 0
        //itmModel.setFilter("");
        sumItmModel.setRowCount(0);
        QVector<int> cnts = itmModel.getCounts();
        for (int i = 0, row = 0; i < cnts.count(); i++)  {
            if (!cnts[i])
                continue;
            qDebug() << cnts[i];

            sumItmModel.insertRow(row);
            itmModel.filter()
            const QSqlRecord &rec = itmModel.record(row);
            sumItmModel.setItem(row, 0, new QStandardItem(rec.value("name").toString()));
            sumItmModel.setItem(row, 1, new QStandardItem(rec.value("price").toString()));
            sumItmModel.setItem(row, 2, new QStandardItem(QString::number(cnts[i])));
            sumItmModel.setItem(row, 3, new QStandardItem(itmModel.getComment(i)));
            row++;
        }
#endif
        ui->sumItemView->resizeColumnsToContents();
    }
}

void NewInvoice::prev()
{
    int idx = ui->tabWidget->currentIndex();
//  qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx - 1);
    buttonsEnable();
}

void NewInvoice::next()
{
    int idx = ui->tabWidget->currentIndex();
//  qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx + 1);
    buttonsEnable();
}
